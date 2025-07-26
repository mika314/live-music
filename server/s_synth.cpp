#include "s_synth.hpp"
#include "s_sink.hpp"
#include <cmath>
#include <log/log.hpp>
#include <shared/consts.hpp>

Synth::Synth(const double &aBpm,
             class Sink &sink,
             double aGain,
             double aPan,
             OscType aOscType,
             Envelope aEnvelope)
  : Source(sink), bpm(aBpm), oscType(aOscType), envelope(aEnvelope)
{
  gain = aGain;
  pan = aPan;
}

auto Synth::internalPull(int samples) -> std::vector<float>
{
  auto r = std::vector<float>{};
  r.reserve(samples * ChN);
  for (auto i = 0; i < samples; ++i)
  {
    const auto a = [this]() {
      auto a = 0.0f;
      for (const auto &n : notes)
        a += n.vel *
             envelope.amp(1.f * (pos - n.start) / SampleRate, 1.f * (n.end - n.start) / SampleRate) *
             osc(n.freq);
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

  notes.emplace_back(N{.freq = 440.f * pow(2.f, (n.n - 57.) / 12.),
                       .vel = pow(10., n.vel / 20.),
                       .start = pos,
                       .end = pos + static_cast<int>(SampleRate * n.dur * 60 / bpm.get())});
  sink.get().unlock();
}

auto Synth::operator()(Envelope v) -> void
{
  envelope = std::move(v);
}

auto Synth::osc(float freq) -> float
{
  const auto phase = freq * pos / SampleRate;
  const auto frac = phase - std::floor(phase);

  switch (oscType)
  {
  case OscType::sin: return std::sin(frac * 2.f * M_PI);
  case OscType::saw: return 2.f * frac - 1.0f; break;
  case OscType::square: return (frac < .5f) ? 1.f : -1.f;
  case OscType::triangle: return 1.f - 4.f * std::fabs(frac - .5f);
  }
}

auto Synth::isBusy() const -> bool
{
  return !notes.empty();
}
