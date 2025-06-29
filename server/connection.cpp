#include "connection.hpp"
#include "speaker.hpp"
#include "synth.hpp"
#include <log/log.hpp>

Connection::Connection(uv::Tcp &&aTcp, std::function<auto(Connection *self)->void> destroy)
  : tcp(std::move(aTcp))
{
  tcp.readStart([this, destroy = std::move(destroy)](int status, std::string aBuf) {
    if (status < 0)
    {
      LOG(status);
      destroy(this);
      return;
    }
    buf += std::move(aBuf);
    for (;;)
    {
      if (sz < 0 && buf.size() >= sizeof(sz))
      {
        sz = *reinterpret_cast<const int32_t *>(buf.data());
        buf.erase(std::begin(buf), std::begin(buf) + sizeof(int32_t));
        continue;
      }
      if (sz > 0 && static_cast<int>(buf.size()) >= sz)
      {
        processPack({std::begin(buf), std::begin(buf) + sz});
        buf.erase(std::begin(buf), std::begin(buf) + sz);
        sz = -1;
        continue;
      }
      break;
    }
  });
}

auto Connection::processPack(std::string_view pck) -> void
{
  ProtoC2S::deser(pck.data(), pck.data() + pck.size(), *this);
}

auto Connection::operator()(msg::Log v) -> void
{
  std::cout << v.msg << std::endl;
}

auto Connection::send(auto id, auto rsp)
{
  std::array<char, 0x10000> buf;
  auto strm = OStrm{buf.begin(), buf.end()};
  ::ser(strm, std::move(rsp));
  const auto sz = static_cast<int32_t>(strm.size() + sizeof(int32_t));
  tcp.write(
    std::string{reinterpret_cast<const char *>(&sz), reinterpret_cast<const char *>(&sz) + sizeof(sz)},
    [](int /*status*/) {});
  tcp.write(
    std::string{reinterpret_cast<const char *>(&id), reinterpret_cast<const char *>(&id) + sizeof(id)},
    [](int /*status*/) {});
  tcp.write(std::string{buf.data(), buf.data() + strm.size()}, [](int /*status*/) {});
}

auto Connection::operator()(msg::NowReq v) -> void
{
  LOG("now request", v.id);
  send(v.id, msg::NowRsp{.samples = 314});
}

template <typename E, typename... Args>
auto Connection::ctor(int32_t rspId, Args &&...args)
{
  auto entity = std::make_unique<E>(std::forward<Args>(args)...);
  const auto id = entity->id;
  entities.emplace(id, std::move(entity));
  send(rspId, msg::CtorRsp{.id = id});
}

auto Connection::operator()(msg::Speaker_CtorReq v) -> void
{
  LOG("speaker ctor", v.id);
  ctor<Speaker>(v.id);
}

auto Connection::operator()(msg::Synth_CtorReq v) -> void
{
  LOG("synth ctor", v.id);
  auto sink = dynamic_cast<Sink *>(entities[v.sinkId].get());
  ctor<Synth>(v.id, *sink);
}
