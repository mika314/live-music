#include "s_sample.hpp"
#include "s_sink.hpp"
#include "sample-lib.hpp"
#include <cmath>
#include <log/log.hpp>
#include <shared/consts.hpp>

Sample::Sample(const double &aBpm,
               class Sink &sink,
               std::filesystem::path aPath,
               double aGain,
               double aPan,
               Note aNote)
  : Source(sink),
    bpm(aBpm),
    path(std::move(aPath)),
    sample(SampleLib::getInst()(path)),
    sampleFreq(440. * powf(2., (aNote.n - 57.) / 12.))
{
  gain = aGain;
  pan = aPan;
  isReady = true;
}

auto Sample::isBusy() const -> bool
{
  sink.get().lock();
  return !notes.empty() && isReady;
  sink.get().unlock();
}

auto Sample::internalPull(int samples) -> std::vector<float>
{
  if (!isReady)
    return {};
  auto r = std::vector<float>{};
  r.reserve(samples * ChN);
  for (auto i = 0; i < ChN * samples; ++i)
  {
    const auto a = [this]() {
      auto a = 0.0f;
      for (const auto &n : notes)
      {
        const auto p = n.freq / sampleFreq;
        const auto idx_ = static_cast<double>(pos - n.start) * p;
        const auto idx = static_cast<int>(idx_);
        const auto frac = idx_ - idx;
        const auto v = envelope.amp(static_cast<double>(pos - n.start) / SampleRate, n.dur);
        a += (idx >= 0 && idx + 1 < static_cast<int>(sample.get().size()))
               ? n.vel * v * (sample.get()[idx] * (1 - frac) + sample.get()[idx + 1] * frac)
               : 0.0f;
      }
      return a;
    }();
    ++pos;
    r.push_back(a);

    for (auto it = std::begin(notes); it != std::end(notes);)
    {
      const auto p = it->freq / sampleFreq;
      if (static_cast<double>(pos - it->start) * p >= sample.get().size())
        it = notes.erase(it);
      else
        ++it;
    }
  }
  return r;
}

auto Sample::play(Note aNote) -> void
{
  LOG(path.filename(), aNote.n);
  sink.get().lock();
  notes.emplace_back(N{.freq = 440. * powf(2., (aNote.n - 57.) / 12.),
                       .dur = aNote.dur,
                       .vel = pow(10., aNote.vel / 20.),
                       .start = pos});
  sink.get().unlock();
}

auto Sample::set(Envelope e) -> void
{
  envelope = e;
}
