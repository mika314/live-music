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
  auto setDur(double) const -> Note;
  auto setVel(double) const -> Note;
};

auto operator+(Note, double) -> Note;
auto operator+(double, Note) -> Note;
auto operator+=(Note &, double) -> Note &;
auto operator-(Note, double) -> Note;
auto operator-=(Note &, double) -> Note &;

// clang-format off
constexpr double O0 = 0;
constexpr double O1 = 12;
constexpr double O2 = 24;
constexpr double O3 = 36;
constexpr double O4 = 48;
constexpr double O5 = 60;
constexpr double O6 = 72;
constexpr double O7 = 84;
constexpr double O8 = 96;
constexpr double O9 = 108;

constexpr Note C      = {0};
constexpr Note Cs     = {1};
constexpr Note Db     = {1};
constexpr Note D      = {2};
constexpr Note Ds     = {3};
constexpr Note Eb     = {3};
constexpr Note E      = {4};
constexpr Note F      = {5};
constexpr Note Fs     = {6};
constexpr Note Gb     = {6};
constexpr Note G      = {7};
constexpr Note Gs     = {8};
constexpr Note Ab     = {8};
constexpr Note A      = {9};
constexpr Note As     = {10};
constexpr Note Bb     = {10};
constexpr Note B      = {11};

constexpr int I       = 0;
constexpr int ii      = 1;
constexpr int II      = 2;
constexpr int iii     = 3;
constexpr int III     = 4;
constexpr int IV      = 5;
constexpr int V       = 7;
constexpr int vi      = 8;
constexpr int VI      = 9;
constexpr int vii     = 10;
constexpr int VII     = 11;
// clang-format on
