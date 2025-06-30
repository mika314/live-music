#pragma once
#include <ser/macro.hpp>

struct Envelope
{
  float attack = 0.01;                                   // sec
  float decay = 0.1;                                     // sec
  float sustain = -20;                                   // dB
  float release = 0.1;                                   // sec
  auto amp(float sample, float duration) const -> float; // 0 - 1
  SER_PROPS(attack, decay, sustain, release);
};
