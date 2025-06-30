#include "uv.hpp"
#include <log/log.hpp>
#include <sstream>
#include <stdexcept>

namespace uv
{
  Tcp::Tcp(uv_loop_t *loop) : socket(std::make_unique<uv_tcp_t>())
  {
    uv_tcp_init(loop, socket.get());
    socket->data = this;
  }

  auto Tcp::readStart(ReadCb cb) -> int
  {
    if (!socket)
    {
      LOG(this, "Uninitialized TCP connection");
      cb(static_cast<int>(-1), std::string{});
      return -1;
    }
    readCb = std::move(cb);
    socket->data = this;
    return uv_read_start((uv_stream_t *)socket.get(),
                         [](uv_handle_t *handle, size_t suggestedSize, uv_buf_t *aBuf) {
                           auto self = static_cast<Tcp *>(handle->data);
                           if (!self)
                           {
                             LOG("self is nullptr");
                             return;
                           }
                           self->buf.resize(suggestedSize);
                           aBuf->base = self->buf.data();
                           aBuf->len = static_cast<decltype(aBuf->len)>(suggestedSize);
                         },
                         [](uv_stream_t *stream, ssize_t nread, const uv_buf_t *aBuf) {
                           auto self = static_cast<Tcp *>(stream->data);
                           if (!self)
                           {
                             LOG("self is nullptr");
                             return;
                           }
                           self->onRead(nread, aBuf);
                         });
  }

  namespace
  {
    struct WriteCtx
    {
      Tcp *tcp;
      std::string buf;
      uv_buf_t uvBuf;
      Tcp::WriteCb cb;
    };
  } // namespace

  auto Tcp::write(std::string val, WriteCb cb) -> int
  {
    if (!socket)
    {
      LOG(this, "Uninitialized TCP connection");
      cb(-1);
      return -1;
    }
    // TODO-Mika Can we combine two memory allocations into one?
    socket->data = this;
    auto req = new uv_write_t;
    auto writeCtx = new WriteCtx;
    req->data = writeCtx;
    writeCtx->tcp = this;
    const auto sz = static_cast<int>(val.size());
    writeCtx->buf = std::move(val);
    writeCtx->uvBuf = uv_buf_init(writeCtx->buf.data(), sz);
    writeCtx->cb = std::move(cb);
    return uv_write(
      req, (uv_stream_t *)socket.get(), &writeCtx->uvBuf, 1, [](uv_write_t *aReq, int status) {
        auto ctx = static_cast<WriteCtx *>(aReq->data);
        ctx->tcp->onWrite(status, std::move(ctx->cb));
        delete ctx;
        delete aReq;
      });
  }
  auto Tcp::bind(const std::string &addr, int port, int flags) -> int
  {
    struct sockaddr_in in;
    uv_ip4_addr(addr.c_str(), port, &in);
    return uv_tcp_bind(socket.get(), (struct sockaddr *)(&in), flags);
  }

  auto Tcp::listen(int backlog, ListenCb cb) -> int
  {
    listenCb = std::move(cb);
    return uv_listen((uv_stream_t *)socket.get(), backlog, [](uv_stream_t *stream, int status) {
      auto self = static_cast<Tcp *>(stream->data);
      if (!self)
      {
        LOG("self is nullptr");
        return;
      }
      if (!self->listenCb)
      {
        LOG("listen callback is nullptr");
        return;
      }
      self->onNewConn(status);
    });
  }

  auto Tcp::onNewConn(int status) -> void
  {
    if (status < 0)
    {
      LOG("New connection error:", uv_strerror(status));
      listenCb(status, Tcp{});
      return;
    }

    auto conn = Tcp{socket->loop};
    if (auto r = uv_accept((uv_stream_t *)socket.get(), (uv_stream_t *)conn.socket.get()); r != 0)
    {
      LOG("Accept failed:", uv_strerror(r));
      uv_close((uv_handle_t *)conn.socket.get(), nullptr);
      return;
    }
    listenCb(status, std::move(conn));
  }

  auto Tcp::onRead(ssize_t nread, const uv_buf_t *aBuf) -> void
  {
    if (nread < 0)
    {
      if (nread != UV_EOF)
        LOG(__func__, "error:", uv_err_name(static_cast<int>(nread)), nread);
      readCb(static_cast<int>(nread), std::string{});
      return;
    }
    if (!readCb)
    {
      LOG("The TCP read callback is not set up");
      return;
    }
    readCb(0, std::string{aBuf->base, aBuf->base + nread});
  }

  Uv::Uv() : loop_(uv_default_loop())
  {
    loop_->data = this;
  }

  auto Uv::tick() -> int
  {
    return uv_run(loop_, UV_RUN_ONCE);
  }

  namespace
  {
    struct ConnectCtx
    {
      Uv *uv;
      Uv::ConnectCb cb;
    };
    struct ConnectCtx2
    {
      Uv *uv;
      Uv::ConnectCb cb;
      Tcp tcp;
    };
  } // namespace

  auto Uv::connect(const std::string &domain, const std::string &port, ConnectCb cb) -> int
  {
    LOG("connect ", domain, ":", port);
    // TODO-Mika Can we combine two memory allocations into one?
    auto resolver = new uv_getaddrinfo_t;
    auto connectCtx = new ConnectCtx;
    resolver->data = connectCtx;
    connectCtx->uv = this;
    connectCtx->cb = std::move(cb);
    LOG("Resolve:", domain, port);
    struct addrinfo hints;
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;
    return uv_getaddrinfo(
      loop_,
      resolver,
      [](uv_getaddrinfo_t *req, int status, struct addrinfo *res) -> void {
        auto ctx = static_cast<ConnectCtx *>(req->data);
        ctx->uv->onResolved(status, res, std::move(ctx->cb));
        delete ctx;
        delete req;
      },
      domain.c_str(),
      port.c_str(),
      &hints);
  }

  auto Uv::onResolved(int status, struct addrinfo *res, ConnectCb cb) -> void
  {
    if (status < 0)
    {
      LOG(__func__, "error:", uv_err_name(status));
      cb(status, Tcp{});
      return;
    }

    char addr[17] = {'\0'};
    uv_ip4_name(reinterpret_cast<struct sockaddr_in *>(res->ai_addr), addr, 16);
    LOG("Resolved:", addr);

    // TODO-Mika Can we combine two memory allocations into one?
    auto connectReq = new uv_connect_t;
    auto connectCtx = new ConnectCtx2;
    connectReq->data = connectCtx;
    connectCtx->uv = this;
    connectCtx->tcp = Tcp{loop_};

    auto s = uv_tcp_connect(connectReq,
                            connectCtx->tcp.socket.get(),
                            (const struct sockaddr *)res->ai_addr,
                            [](uv_connect_t *req, int aStatus) {
                              LOG("Connected");
                              auto ctx = static_cast<ConnectCtx2 *>(req->data);
                              ctx->uv->onConnected(aStatus, std::move(ctx->tcp), std::move(ctx->cb));
                              delete ctx;
                              delete req;
                            });
    if (s < 0)
    {
      LOG(__func__, "error:", uv_err_name(s));
      cb(s, Tcp{});
      uv_freeaddrinfo(res);
      return;
    }
    connectCtx->cb = std::move(cb);
    uv_freeaddrinfo(res);
  }

  auto Uv::onConnected(int status, Tcp tcp, ConnectCb cb) -> void
  {
    cb(status, std::move(tcp));
  }

  auto Uv::createTimer() -> Timer
  {
    return Timer{loop_};
  }

  auto Uv::createIdle() -> Idle
  {
    return Idle{loop_};
  }

  auto Uv::createPrepare() -> Prepare
  {
    return Prepare{loop_};
  }

  auto Uv::createAsync(Async::Cb cb) -> Async
  {
    return Async{loop_, std::move(cb)};
  };

  Tcp::Tcp(Tcp &&other)
    : socket(std::move(other.socket)), readCb(std::move(other.readCb)), buf(std::move(other.buf))
  {
    if (socket)
      socket->data = this;
    other.socket = nullptr;
  }

  auto Tcp::deinit() -> void
  {
    if (socket)
    {
      socket->data = nullptr;
      auto rawSocket = socket.release();
      uv_read_stop((uv_stream_t *)rawSocket);
      auto req = new uv_shutdown_t;
      req->data = rawSocket;
      uv_shutdown(req, (uv_stream_t *)rawSocket, [](uv_shutdown_t *handle, int status) {
        LOG("Disconnected");
        if (status < 0)
          LOG(__func__, "error:", uv_err_name(status));
        delete static_cast<uv_tcp_t *>(handle->data);
        delete handle;
      });
    }
  }

  Tcp &Tcp::operator=(Tcp &&other)
  {
    deinit();

    socket = std::move(other.socket);
    readCb = std::move(other.readCb);
    buf = std::move(other.buf);
    if (socket)
      socket->data = this;
    other.socket = nullptr;
    return *this;
  }

  Tcp::~Tcp()
  {
    deinit();
  }

  Timer::Timer(uv_loop_t *loop) : timer(std::make_unique<uv_timer_t>())
  {
    uv_timer_init(loop, timer.get());
    timer->data = this;
  }

  auto Timer::start(Cb aCb, uint64_t timeout, uint64_t repeat) -> int
  {
    cb = std::move(aCb);
    timer->data = this;
    return uv_timer_start(
      timer.get(),
      [](uv_timer_t *handle) {
        auto self = static_cast<Timer *>(handle->data);
        if (!self->cb)
        {
          LOG("The Timer callback is not set up");
          return;
        }
        self->cb();
      },
      timeout,
      repeat);
  }

  auto Timer::stop() -> int
  {
    if (!timer)
    {
      LOG("timer is nullptr");
      return -1;
    }
    return uv_timer_stop(timer.get());
  }

  Timer::~Timer()
  {
    stop();
  }

  auto Uv::loop() const -> decltype(uv_default_loop())
  {
    return loop_;
  }

  Idle::Idle(uv_loop_t *loop) : idle(std::make_unique<uv_idle_t>())
  {
    uv_idle_init(loop, idle.get());
    idle->data = this;
  }

  Idle::~Idle()
  {
    stop();
  }

  auto Idle::start(Cb aCb) -> int
  {
    cb = std::move(aCb);
    idle->data = this;
    return uv_idle_start(idle.get(), [](uv_idle_t *handle) {
      auto self = static_cast<Idle *>(handle->data);
      if (!self->cb)
      {
        LOG("The Idle callback is not set up");
        return;
      }
      self->cb();
    });
  }

  auto Idle::stop() -> int
  {
    if (!idle)
    {
      LOG("idle is nullptr");
      return -1;
    }
    return uv_idle_stop(idle.get());
  }

  Prepare::Prepare(uv_loop_t *loop) : prepare(std::make_unique<uv_prepare_t>())
  {
    uv_prepare_init(loop, prepare.get());
    prepare->data = this;
  }

  Prepare::~Prepare()
  {
    stop();
  }

  auto Prepare::start(Cb aCb) -> int
  {
    cb = std::move(aCb);
    prepare->data = this;
    return uv_prepare_start(prepare.get(), [](uv_prepare_t *handle) {
      auto self = static_cast<Prepare *>(handle->data);
      if (!self->cb)
      {
        LOG("The Prepare callback is not set up");
        return;
      }
      self->cb();
    });
  }

  auto Prepare::stop() -> int
  {
    if (!prepare)
    {
      LOG("prepare is nullptr");
      return -1;
    }
    return uv_prepare_stop(prepare.get());
  }

  FsEvent::FsEvent(uv_loop_t *loop) : event(std::make_unique<uv_fs_event_t>())
  {
    uv_fs_event_init(loop, event.get());
    event->data = this;
  }

  FsEvent::~FsEvent()
  {
    stop();
  }

  auto FsEvent::start(Cb aCb, const std::string &path, unsigned flags) -> int
  {
    cb = std::move(aCb);
    event->data = this;
    return uv_fs_event_start(
      event.get(),
      [](uv_fs_event_t *handle, const char *filename, int events, int status) {
        auto self = static_cast<FsEvent *>(handle->data);
        if (!self->cb)
        {
          LOG("The FsEvent callback is not set up");
          return;
        }
        self->cb(filename, events, status);
      },
      path.c_str(),
      flags);
  }

  auto FsEvent::stop() -> int
  {
    if (!event)
    {
      LOG("event is nullptr");
      return -1;
    }
    return uv_fs_event_stop(event.get());
  }

  auto Uv::createFsEvent() -> FsEvent
  {
    return FsEvent{loop_};
  }

  auto Async::send() -> void
  {
    uv_async_send(handle.get());
  }

  Async::Async(uv_loop_t *loop, Cb aCb) : handle(std::make_unique<uv_async_t>()), cb(std::move(aCb))
  {
    uv_async_init(loop, handle.get(), [](uv_async_t *h) { static_cast<Async *>(h->data)->cb(); });
    handle->data = this;
  }
} // namespace uv
