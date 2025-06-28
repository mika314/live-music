#include "connection.hpp"
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

auto Connection::operator()(msg::NowReq v) -> void
{
  LOG("now request", v.id);
  auto m = msg::NowRsp{.samples = 314};
  std::array<char, 0x10000> buf;
  auto strm = OStrm{buf.begin(), buf.end()};
  ::ser(strm, std::move(m));
  const auto sz = static_cast<int32_t>(strm.size() + sizeof(int32_t));
  tcp.write(
    std::string{reinterpret_cast<const char *>(&sz), reinterpret_cast<const char *>(&sz) + sizeof(sz)},
    [](int /*status*/) {});
  tcp.write(std::string{reinterpret_cast<const char *>(&v.id),
                        reinterpret_cast<const char *>(&v.id) + sizeof(v.id)},
            [](int /*status*/) {});
  tcp.write(std::string{buf.data(), buf.data() + strm.size()}, [](int /*status*/) {});
}
