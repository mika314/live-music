#include "note.hpp"

auto operator+(Note x, double y) -> Note
{
  auto r = x;
  r.n += y;
  return r;
}

auto operator+(double y, Note x) -> Note
{
  auto r = x;
  r.n += y;
  return r;
}

auto operator+=(Note &x, double y) -> Note &
{
  x.n += y;
  return x;
}

auto operator-(Note x, double y) -> Note
{
  auto r = x;
  r.n -= y;
  return r;
}

auto operator-=(Note &x, double y) -> Note &
{
  x.n -= y;
  return x;
}
