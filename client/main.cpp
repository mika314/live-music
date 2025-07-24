#include "live-music.hpp"

auto main() -> int
{
  setBpm(120);
  auto master = Speaker{};
  auto reverb = Reverb{master};
  reverb.wet(1.0);
  runForever();
}
