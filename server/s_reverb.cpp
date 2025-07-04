#include "s_reverb.hpp"
#include <cmath>

Reverb::Reverb(Sink &aSink, double aGain, double aPan)
  : Source(aSink),
    sink(aSink),
    combsL({// Four combs with prime-ish delays for decorrelation
            {CombFilter(1557, 0.773f)},
            {CombFilter(1617, 0.802f)},
            {CombFilter(1491, 0.753f)},
            {CombFilter(1422, 0.733f)}}),
    combsR({// Four combs with prime-ish delays for decorrelation
            {CombFilter(1557, 0.773f)},
            {CombFilter(1617, 0.802f)},
            {CombFilter(1491, 0.753f)},
            {CombFilter(1422, 0.733f)}}),
    allpassesL({// Two all-pass stages for diffusion
                {AllpassFilter(225, 0.5f)},
                {AllpassFilter(556, 0.5f)}}),
    allpassesR({// Two all-pass stages for diffusion
                {AllpassFilter(225, 0.5f)},
                {AllpassFilter(556, 0.5f)}})
{
  gain = aGain;
  pan = aPan;
}

auto Reverb::lock() const -> void
{
  sink.get().lock();
}

auto Reverb::unlock() const -> void
{
  sink.get().unlock();
}

auto Reverb::internalPull(int samples) -> std::vector<float>
{
  auto r = mix(samples);
  for (auto i = 0U; i < r.size(); ++i)
    r[i] = i % 2 == 0 ? processL(r[i]) : processR(r[i]);
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

float Reverb::processL(float in)
{
  // 1) send input into each comb, sum outputs
  float sum = 0.f;
  for (auto &c : combsL)
    sum += c.process(in);

  // 2) feed through all-pass chain
  float y = sum;
  for (auto &a : allpassesL)
    y = a.process(y);

  // 3) wet/dry mix
  return in * dry + y * wet;
}

float Reverb::processR(float in)
{
  // 1) send input into each comb, sum outputs
  float sum = 0.f;
  for (auto &c : combsR)
    sum += c.process(in);

  // 2) feed through all-pass chain
  float y = sum;
  for (auto &a : allpassesR)
    y = a.process(y);

  // 3) wet/dry mix
  return in * dry + y * wet;
}

void Reverb::setMix(float wetLevel)
{
  wet = wetLevel;
  dry = 1.f - wetLevel;
}
