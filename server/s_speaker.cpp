#include "s_speaker.hpp"
#include "master_speaker.hpp"

Speaker::Speaker(MasterSpeaker &aMasterSpeaker, double aGain, double aPan)
  : Source(aMasterSpeaker), masterSpeaker(aMasterSpeaker)
{
  gain = aGain;
  pan = aPan;
}

auto Speaker::lock() const -> void
{
  masterSpeaker.get().lock();
}

auto Speaker::unlock() const -> void
{
  masterSpeaker.get().unlock();
}

auto Speaker::internalPull(int samples) -> std::vector<float>
{
  return mix(samples);
}

auto Speaker::isBusy() const -> bool
{
  return !sources.empty();
}
