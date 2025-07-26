#include "envelope.hpp"
#include "consts.hpp"
#include <cmath>

auto Envelope::amp(double t, double duration) const -> double
{
  if (t < attack && t < duration)
    return t / attack;
  if (t < attack + decay && t < duration)
    return (1. - (t - attack) / decay) * (1 - sustain) + sustain;
  if (t < duration)
    return sustain;
  if (t < duration + release)
  {
    auto a = [=]() {
      if (duration < attack)
        return duration / attack;
      if (duration < attack + decay)
        return (1. - (duration - attack) / decay) * (1 - sustain) + sustain;
      return sustain;
    }();
    return a * (1. - (t - duration) / release);
  }
  return 0;
}
