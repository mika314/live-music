#include "s_synth.hpp"
#include "s_sink.hpp"
#include <cmath>
#include <log/log.hpp>
#include <shared/consts.hpp>

Synth::Synth(const float &aBpm, class Sink &sink) : Source(sink), bpm(aBpm) {}

auto Synth::pull(int samples) -> std::vector<int16_t>
{
  auto r = std::vector<int16_t>{};
  r.reserve(samples * ChN);
  for (auto i = 0; i < samples; ++i)
  {
    const auto a = [this]() {
      int16_t a = 0;
      for (const auto &n : notes)
        a +=
          static_cast<int16_t>(n.second.vel * 0x7fff *
                               sin(220 * powf(2, (n.first + 3) / 12.f) * pos / SampleRate * 2 * 3.1415));
      return a;
    }();
    ++pos;
    for (auto ch = 0; ch < ChN; ++ch)
      r.push_back(a);

    for (auto it = std::begin(notes); it != std::end(notes);)
      if (pos > it->second.end)
        it = notes.erase(it);
      else
        ++it;
  }
  return r;
}

auto Synth::operator()(Note n) -> void
{
  sink.get().lock();
  notes[n.n] = N{.vel = n.vel, .end = pos + static_cast<int>(SampleRate * n.dur * 60 / bpm.get())};
  sink.get().unlock();
}
