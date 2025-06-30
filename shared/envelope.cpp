#include "envelope.hpp"
#include "consts.hpp"
#include <cmath>

auto Envelope::amp(float t, float duration) const -> float
{
  if (t < attack && t < duration)
    return t / attack;
  const auto sus = pow(10, sustain / 20);
  if (t < attack + decay && t < duration)
    return (1. - (t - attack) / decay) * (1 - sus) + sus;
  if (t < duration)
    return sus;
  if (t < duration + release)
    return sus * (1. - (t - duration) / release);
  return 0;
}
