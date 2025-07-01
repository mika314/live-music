#include "live-music.hpp"
#include <shared/consts.hpp>
#include <shared/proto.hpp>
#include <thread>

static double bpm = DefaultBpm;

auto log(std::string v) -> void
{
  send(msg::Log{.msg = std::move(v)});
}

auto now() -> int
{
  return Conn::getInst().reqRsp<msg::NowRsp>(msg::NowReq{}).samples;
}

auto setBpm(double v) -> void
{
  send(msg::SetBpm{.v = v});
  bpm = v;
}

static thread_local decltype(std::chrono::steady_clock::now()) curTime;

auto newThread() -> void
{
  const auto n = std::chrono::microseconds(static_cast<int>(
    (now() % static_cast<int>(16 * 60 * SampleRate / bpm)) * 1. / SampleRate * 1'000'000));
  curTime = std::chrono::steady_clock::now() - n;
}

auto delay(double v) -> void
{
  curTime += std::chrono::microseconds{static_cast<long>(1'000'000 * v * 60 / bpm)};
  std::this_thread::sleep_for(curTime - std::chrono::steady_clock::now());
}

auto getCurTime() -> decltype(std::chrono::steady_clock::now())
{
  return curTime;
}

auto thread(std::function<auto()->void> v) -> std::thread *
{
  return new std::thread([v = std::move(v)]() {
    newThread();
    v();
  });
}

auto createPluck(Sink &v) -> Synth
{
  return Synth{
    v,
    Synth::CtorParams{.oscType = OscType::sin,
                      .envelope = Envelope{.attack = .01, .decay = .02, .sustain = -30, .release = 1}}};
}

auto createBass(Sink &v) -> Synth
{
  return Synth{
    v,
    Synth::CtorParams{.oscType = OscType::triangle,
                      .envelope = Envelope{.attack = .01, .decay = 0, .sustain = 0, .release = .5}}};
}

auto createPad(Sink &v) -> Synth
{
  return Synth{
    v,
    Synth::CtorParams{.oscType = OscType::sin,
                      .envelope = Envelope{.attack = .25, .decay = 0, .sustain = 0, .release = .25}}};
}
