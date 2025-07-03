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
  return mix(samples);
}

auto Speaker::isBusy() const -> bool
{
  return !sources.empty();
}
