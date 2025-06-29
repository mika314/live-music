#pragma once
#include <cstdint>
#include <ser/macro.hpp>

struct Note
{
  int32_t n;
  float dur;
  float vel;
  SER_PROPS(n, dur, vel);
};
