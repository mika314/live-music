#include "live-music.hpp"
#include "conn.hpp"
#include "live-music-internal.hpp"
#include "reverb.hpp"
#include "sample.hpp"
#include "send.hpp"
#include "speaker.hpp"
#include "synth.hpp"
#include <chrono>
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

auto delay(const dur &v) -> void
{
  delay(v.v);
}

auto getCurTime() -> decltype(std::chrono::steady_clock::now())
{
  return curTime;
}

auto createPluck(Sink &v, double gain, double pan) -> Synth
{
  return Synth{
    v,
    SynthParams{.gain = gain,
                .pan = pan,
                .oscType = OscType::sin,
                .envelope = Envelope{.attack = .01, .decay = .02, .sustain = -30, .release = 1}}};
}

auto createBass(Sink &v, double gain, double pan) -> Synth
{
  return Synth{
    v,
    SynthParams{.gain = gain,
                .pan = pan,
                .oscType = OscType::triangle,
                .envelope = Envelope{.attack = .01, .decay = 0, .sustain = 0, .release = .5}}};
}

auto createPad(Sink &v, double gain, double pan) -> Synth
{
  return Synth{
    v,
    SynthParams{.gain = gain,
                .pan = pan,
                .oscType = OscType::sin,
                .envelope = Envelope{.attack = .25, .decay = 0, .sustain = 0, .release = .25}}};
}

auto createSynth(Sink &v, double gain, double pan) -> Synth
{
  return Synth{
    v,
    SynthParams{.gain = gain,
                .pan = pan,
                .oscType = OscType::sin,
                .envelope = Envelope{.attack = .01, .decay = .01, .sustain = -10, .release = .1}}};
}

auto isLate() -> bool
{
  return std::chrono::steady_clock::now() > getCurTime() + std::chrono::milliseconds(100);
}

namespace internal
{
  auto thread(BaseFunc &v) -> void
  {
    new std::thread([&v]() {
      newThread();
      v.invoke();
    });
  }
} // namespace internal

auto runForever() -> void
{
  for (;;)
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

auto rnd() -> int
{
  return rand();
}
