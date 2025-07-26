#include "live-music.hpp"

auto main() -> int
{
  setBpm(120);
  auto master = Speaker{-12};
  auto reverb = Reverb{master};
  reverb.wet(0.9);
  runForever();
}
