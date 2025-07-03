#pragma once
#include <cstdint>
#include <ser/macro.hpp>

struct Note
{
  double n = 60;
  double dur = 1;
  double vel = -12;
  SER_PROPS(n, dur, vel);
  auto setDur(double) -> Note &;
  auto setVel(double) -> Note &;
};

auto operator+(Note, double) -> Note;
auto operator+(double, Note) -> Note;
auto operator+=(Note &, double) -> Note &;
auto operator-(Note, double) -> Note;
auto operator-=(Note &, double) -> Note &;
