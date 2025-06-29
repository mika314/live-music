#include "live-music.hpp"
#include <shared/consts.hpp>
#include <shared/proto.hpp>
#include <thread>

static float bpm = DefaultBpm;

auto log(std::string v) -> void
{
  send(msg::Log{.msg = std::move(v)});
}

auto now() -> int
{
  return Conn::getInst().reqRsp<msg::NowRsp>(msg::NowReq{}).samples;
}

auto setBpm(float v) -> void
{
  send(msg::SetBpm{.v = v});
  bpm = v;
}

auto delay(float v) -> void
{
  std::this_thread::sleep_for(std::chrono::microseconds{static_cast<long>(1'000'000 * v * 60 / bpm)});
}
