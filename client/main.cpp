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

class Speaker
{
public:
  Speaker()
    : id(reqRsp<msg::CtorRsp>(msg::Speaker_CtorReq{.id = ConnectionThread::getInst().getNextId()}).id)
  {
  }
  int32_t id;
};

class Synth
{
public:
  Synth(Speaker &speaker)
    : id(reqRsp<msg::CtorRsp>(
           msg::Synth_CtorReq{.id = ConnectionThread::getInst().getNextId(), .sinkId = speaker.id})
           .id)
  {
  }
  int32_t id;
};

auto main() -> int
{
  ConnectionThread::init();

  auto t = std::thread{[]() {
    for (auto i = 0; i < 10; ++i)
      log("Hello world! Secondary thread! " + std::to_string(i));
  }};

  for (auto i = 0; i < 10; ++i)
    log("Hello world! Main thread! " + std::to_string(i));

  auto speaker = Speaker{};
  auto synth = Synth{speaker};
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);

  t.join();

  LOG("now: ", now());
}
