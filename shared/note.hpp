#pragma once
#include <cstdint>
#include <ser/macro.hpp>

struct Note
{
  int32_t n = 60;
  float dur = 1;
  float vel = -6;
  SER_PROPS(n, dur, vel);
};
