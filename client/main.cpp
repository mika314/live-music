#include "connection_thread.hpp"
#include <log/log.hpp>
#include <shared/proto.hpp>

auto send(auto v)
{
  auto &inst = ConnectionThread::getInst();
  std::array<char, 0x10000> buf;
  const auto sz = static_cast<int32_t>(ProtoC2S::ser(std::move(v), buf.data(), buf.data() + buf.size()));
  inst.req(std::string{buf.data(), buf.data() + sz});
}

auto log(std::string v) -> void
{
  send(msg::Log{.msg = std::move(v)});
}

template <typename Rsp>
auto reqRsp(auto req) -> Rsp
{
  auto &inst = ConnectionThread::getInst();
  std::array<char, 0x10000> buf;
  const auto id = req.id;
  const auto sz =
    static_cast<int32_t>(ProtoC2S::ser(std::move(req), buf.data(), buf.data() + buf.size()));
  inst.req(std::string{buf.data(), buf.data() + sz});
  auto payload = inst.waitForRspId(id);
  auto st = IStrm{payload.data(), payload.data() + payload.size()};
  Rsp r;
  ::deser(st, r);
  return r;
}

auto now() -> int
{
  return reqRsp<msg::NowRsp>(msg::NowReq{.id = ConnectionThread::getInst().getNextId()}).samples;
}

auto main() -> int
{
  ConnectionThread::init();

  auto t = std::thread{[]() {
    for (auto i = 0; i < 10; ++i)
      log("Hello world! Secondary thread! " + std::to_string(i));
  }};

  for (auto i = 0; i < 10; ++i)
    log("Hello world! Main thread! " + std::to_string(i));

  t.join();

  LOG("now: ", now());
}
