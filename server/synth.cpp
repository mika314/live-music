#include "synth.hpp"
#include <cmath>
#include <shared/consts.hpp>

Synth::Synth(class Sink &sink) : Source(sink) {}

auto Synth::pull(int samples) -> std::vector<int16_t>
{
  auto r = std::vector<int16_t>{};
  r.reserve(samples * ChN);
  for (auto i = 0; i < samples; ++i)
  {
    const auto a = static_cast<int16_t>(0x4000 * sin(220. * pos++ / SampleRate * 2 * 3.1415));
    for (auto ch = 0; ch < ChN; ++ch)
      r.push_back(a);
  }
  return r;
}
