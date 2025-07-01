#include "s_speaker.hpp"
#include "master_speaker.hpp"

Speaker::Speaker(MasterSpeaker &aMasterSpeaker) : Source(aMasterSpeaker), masterSpeaker(aMasterSpeaker)
{
}

auto Speaker::lock() const -> void
{
  masterSpeaker.get().lock();
}

auto Speaker::unlock() const -> void
{
  masterSpeaker.get().unlock();
}

auto Speaker::pull(int samples) -> std::vector<float>
{
  auto r = std::vector<float>{};
  for (auto &s : sources)
  {
    if (!s->isBusy())
      continue;
    r.resize(samples * ChN);
    const auto chunk = s->pull(samples);
    for (auto i = 0; i < samples * ChN; ++i)
      r[i] += chunk[i];
  }
  return r;
}

auto Speaker::isBusy() const -> bool
{
  return !sources.empty();
}
