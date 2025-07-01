#include "s_sample.hpp"
#include "load-audio.hpp"
#include "s_sink.hpp"
#include <cmath>
#include <shared/consts.hpp>

Sample::Sample(const double &aBpm, class Sink &sink, std::filesystem::path path)
  : Source(sink), bpm(aBpm), sample(loadAudio(std::move(path)))
{
}

auto Sample::isBusy() const -> bool
{
  sink.get().lock();
  return !notes.empty();
  sink.get().unlock();
}

auto Sample::pull(int samples) -> std::vector<float>
{
  auto r = std::vector<float>{};
  r.reserve(samples * ChN);
  for (auto i = 0; i < ChN * samples; ++i)
  {
    const auto a = [this]() {
      auto a = 0.0f;
      for (const auto &n : notes)
      {
        const auto idx = pos - n.start;
        a += (idx >= 0 && idx < static_cast<int>(sample.size())) ? powf(10.f, n.vel / 20.f) * sample[idx]
                                                                 : 0.0f;
      }
      return a;
    }();
    ++pos;
    r.push_back(a);

    for (auto it = std::begin(notes); it != std::end(notes);)
      if (pos - it->start >= static_cast<int>(sample.size()))
        it = notes.erase(it);
      else
        ++it;
  }
  return r;
}

auto Sample::operator()(double vel) -> void
{
  sink.get().lock();
  notes.emplace_back(N{.vel = vel, .start = pos});
  sink.get().unlock();
}
