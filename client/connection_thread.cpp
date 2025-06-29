#include "connection_thread.hpp"
#include <log/log.hpp>

ConnectionThread::ConnectionThread()
  : thread([this]() {
      {
        uv::Uv loop;
        loop.connect("localhost", "7000", [&loop, this](int status, uv::Tcp aConnection) {
          if (status != 0)
          {
            LOG(status);
            return;
          }
          LOG("connected");
          connection = std::move(aConnection);
          q = std::make_unique<Q>(loop, [this](std::string v) {
            if (v.empty())
              return;
            const auto sz = static_cast<int32_t>(v.size());
            connection.write(std::string{reinterpret_cast<const char *>(&sz),
                                         reinterpret_cast<const char *>(&sz) + sizeof(sz)},
                             [](int /*status*/) {});
            connection.write(std::string{v.data(), v.data() + sz}, [](int /*status*/) {});
          });
          isReady = true;
          connection.readStart([this](int status, std::string v) { onRead(status, std::move(v)); });
          startupCv.notify_all();
        });
        while (!done)
          loop.tick();
      }
    })
{
  auto lock = std::unique_lock<std::mutex>(startupMutex);
  startupCv.wait(lock, [this]() { return isReady; });
}

ConnectionThread::~ConnectionThread()
{
  done = true;
  req("");
  thread.join();
}

auto ConnectionThread::req(std::string v) -> void
{
  q->pushToReqQ(std::move(v));
}

auto ConnectionThread::waitForRspId(int id) -> std::string
{
  return q->waitForRspId(id);
}

auto ConnectionThread::getInst() -> ConnectionThread &
{
  static ConnectionThread ints;
  return ints;
}

auto ConnectionThread::init() -> void
{
  getInst();
}

auto ConnectionThread::getNextId() -> int
{
  return ++nextId;
}

auto ConnectionThread::onRead(int status, std::string v) -> void
{
  if (status != 0)
  {
    LOG("read error:", status);
    return;
  }
  buf += std::move(v);
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
      const auto id = *reinterpret_cast<int32_t *>(buf.data());
      q->pushToRspQ(id, std::string{std::begin(buf) + sizeof(int32_t), std::begin(buf) + sz});
      buf.erase(std::begin(buf), std::begin(buf) + sz);
      sz = -1;
      continue;
    }
    break;
  }
}
