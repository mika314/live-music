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
               double aPan)
  : Source(sink), bpm(aBpm), path(std::move(aPath)), sample(SampleLib::getInst()(path))
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
        const auto idx = pos - n.start;
        a += (idx >= 0 && idx < static_cast<int>(sample.get().size()))
               ? powf(10.f, n.vel / 20.f) * sample.get()[idx]
               : 0.0f;
      }
      return a;
    }();
    ++pos;
    r.push_back(a);

    for (auto it = std::begin(notes); it != std::end(notes);)
      if (pos - it->start >= static_cast<int>(sample.get().size()))
        it = notes.erase(it);
      else
        ++it;
  }
  return r;
}

auto Sample::operator()(double vel) -> void
{
  LOG(path.filename(), vel);
  sink.get().lock();
  notes.emplace_back(N{.vel = vel, .start = pos});
  sink.get().unlock();
}
