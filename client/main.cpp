#include "live-music.hpp"

auto main() -> int
{
  setBpm(130);
  auto speaker = Speaker{};
  thread([&]() {
    auto synth = createPluck(speaker);

    for (auto i = 0;; i = (i + 7) % 12)
      for (auto k = 0; k < 8; ++k)
        for (auto j : {C, E, G, C + O, C, E, G, C + O})
        {
          j.dur = Eight;
          j.vel = -12 - (rand() % 24);
          synth(j + O4 + i);
          delay(Eight);
        }
  });

  thread([&]() {
    auto synth = createBass(speaker);

    for (auto i = 0;; i = (i + 7) % 12)
      for (auto k = 0; k < 8; ++k)
      {
        auto n = O2 + Note{.n = (i + k % 2 * 7) % 12 * 1., .dur = .75 * Quarter};
        n.vel = -20;
        synth(n);
        delay(Quarter);
        n.vel = -25;
        synth(n);
        delay(Quarter);
        n.vel = -23;
        synth(n);
        delay(Quarter);
        n += O;
        n.vel = -25;
        synth(n);
        delay(Eight);
        n.vel = -30;
        synth(n);
        delay(Eight);
      }
  });

  thread([&]() {
    auto synth = createPad(speaker);

    for (auto i = 0;; i = (i + 7) % 12)
      for (auto k = 0; k < 8; ++k)
      {
        auto n1 = O3 + Note{.n = (i + k % 2 * 7) % 12 * 1., .dur = Quarter};
        auto n2 = n1 + III;
        auto n3 = n1 + V;
        n1.vel = -30;
        n2.vel = n1.vel;
        n3.vel = n1.vel;
        synth(n1);
        synth(n2);
        synth(n3);
        delay(Quarter);
        n1.vel = -35;
        n2.vel = n1.vel;
        n3.vel = n1.vel;
        synth(n1);
        synth(n2);
        synth(n3);
        delay(Quarter);
        n1.vel = -33;
        n2.vel = n1.vel;
        n3.vel = n1.vel;
        synth(n1);
        synth(n2);
        synth(n3);
        delay(Quarter);
        n1.vel = -35;
        n2.vel = n1.vel;
        n3.vel = n1.vel;
        synth(n1);
        synth(n2);
        synth(n3);
        delay(Eight);
        n1.vel = -35;
        n2.vel = n1.vel;
        n3.vel = n1.vel;
        synth(n1);
        synth(n2);
        synth(n3);
        delay(Eight);
      }
  });

  for (;;)
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }
}
