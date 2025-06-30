#pragma once
#include <cstdint>
#include <ser/macro.hpp>

struct Note
{
  double n = 60;
  double dur = 1;
  double vel = -6;
  SER_PROPS(n, dur, vel);
};
