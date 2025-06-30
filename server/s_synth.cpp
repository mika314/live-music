#include "s_synth.hpp"
#include "s_sink.hpp"
#include <cmath>
#include <log/log.hpp>
#include <shared/consts.hpp>

Synth::Synth(const float &aBpm, class Sink &sink) : Source(sink), bpm(aBpm) {}

auto Synth::pull(int samples) -> std::vector<float>
{
  auto r = std::vector<float>{};
  r.reserve(samples * ChN);
  for (auto i = 0; i < samples; ++i)
  {
    const auto a = [this]() {
      float a = 0;
      for (const auto &n : notes)
        a += static_cast<float>(
          pow(10, n.vel / 20) *
          envelope.amp(1.f * (pos - n.start) / SampleRate, 1.f * (n.end - n.start) / SampleRate) *
          sin(440 * powf(2, (n.note - 69) / 12.f) * pos / SampleRate * 2 * 3.1415));
      return a;
    }();
    ++pos;
    for (auto ch = 0; ch < ChN; ++ch)
      r.push_back(a);

    for (auto it = std::begin(notes); it != std::end(notes);)
      if (pos > it->end + envelope.release * SampleRate)
        it = notes.erase(it);
      else
        ++it;
  }
  return r;
}

auto Synth::operator()(Note n) -> void
{
  sink.get().lock();
  notes.emplace_back(N{.note = n.n,
                       .vel = n.vel,
                       .start = pos,
                       .end = pos + static_cast<int>(SampleRate * n.dur * 60 / bpm.get())});
  sink.get().unlock();
}

auto Synth::operator()(Envelope v) -> void
{
  envelope = std::move(v);
}
