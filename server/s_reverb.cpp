#include "s_reverb.hpp"
#include <cmath>

Reverb::Reverb(Sink &aSink)
  : Source(aSink),
    sink(aSink),
    combs({// Four combs with prime-ish delays for decorrelation
           {CombFilter(1557, 0.773f)},
           {CombFilter(1617, 0.802f)},
           {CombFilter(1491, 0.753f)},
           {CombFilter(1422, 0.733f)}}),
    allpasses({// Two all-pass stages for diffusion
               {AllpassFilter(225, 0.5f)},
               {AllpassFilter(556, 0.5f)}})
{
}

auto Reverb::lock() const -> void
{
  sink.get().lock();
}

auto Reverb::unlock() const -> void
{
  sink.get().unlock();
}

auto Reverb::pull(int samples) -> std::vector<float>
{
  auto r = mix(samples);
  for (auto &v : r)
    v = process(v);
  return r;
}

auto Reverb::isBusy() const -> bool
{
  return !sources.empty();
}

auto Reverb::setWet(double v) -> void
{
  setMix(v);
}

float Reverb::process(float in)
{
  // 1) send input into each comb, sum outputs
  float sum = 0.f;
  for (auto &c : combs)
    sum += c.process(in);

  // 2) feed through all-pass chain
  float y = sum;
  for (auto &a : allpasses)
    y = a.process(y);

  // 3) wet/dry mix
  return in * dry + y * wet;
}

void Reverb::setMix(float wetLevel)
{
  wet = wetLevel;
  dry = 1.f - wetLevel;
}
