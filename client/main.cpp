#include "live-music.hpp"

auto main() -> int
{
  setBpm(120);
  auto speaker = Speaker{};
  thread([&]() {
    auto loop =
      Sample{speaker, "samples/stargate-sample-pack/fugue-state-audio/loops/120-filterhats.wav"};
    auto kick =
      Sample{speaker, "samples/stargate-sample-pack/karoryfer/kicks/kick_Szpaderski_24_dampened.wav"};
    auto snare =
      Sample{speaker, "samples/stargate-sample-pack/karoryfer/snares/snare_Pearl_alumunum_14x8.wav"};
    auto clap = Sample{
      speaker, "samples/stargate-sample-pack/freesound/drums/clap/388042__sami-kullstrom__clap.wav"};

    for (;;)
    {
      // loop(-35);
      for (auto i = 0; i < 4 * 4; ++i)
      {
        kick();
        if (i % 2 == 1)
        {
          snare(-20);
          clap(-15);
        }
        delay(Eighth);
        if (i % 2 == 1)
          clap(-25);
        delay(Eighth);
      }
    }
  });

  thread([&]() {
    auto hat = Sample(speaker, "samples/stargate-sample-pack/karoryfer/hihats/hihat_BRD_tight.wav");
    for (;;)
    {
      hat(-25 - rand() % 6);
      delay(Eighth);
    }
  });

  thread([&]() {
    auto bass = createBass(speaker);
    for (;;)
    {
      for (auto nn : {C, C, C, C, D, D, D, D, F, F, F, F, E, E, E, E})
      {
        auto n = nn + O2;
        n.vel = -25;
        n.dur = Eighth;
        bass(n);
        delay(Quarter);
      }
    }
  });

  thread([&]() {
    auto pad = createPad(speaker);
    for (;;)
    {
      {
        auto n = C + O3;
        n.vel = -25;
        n.dur = Whole;
        pad.maj(n);
        delay(Whole);
      }
      {
        auto n = D + O3;
        n.vel = -25;
        n.dur = Whole;
        pad.min(n);
        delay(Whole);
      }
      {
        auto n = F + O3;
        n.vel = -25;
        n.dur = Whole;
        pad.maj(n);
        delay(Whole);
      }
      {
        auto n = E + O3;
        n.vel = -25;
        n.dur = Whole;
        pad.min(n);
        delay(Whole);
      }
    }
  });
  thread([&]() {
    auto pluck = createPluck(speaker);
    for (;;)
    {
      for (auto n : {C, D, E, F, G, A, B, C + O})
      {
        n += O4;
        n.vel = -6;
        n.dur = Eighth;
        pluck(n);
        delay(Eighth);
      }
      for (auto n : {D, E, F, G, A, B, C + O, D + O})
      {
        n += O4;
        n.vel = -6;
        n.dur = Eighth;
        pluck(n);
        delay(Eighth);
      }
      for (auto n : {F, G, A, B, C + O, D + O, E + O, F + O})
      {
        n += O4;
        n.vel = -6;
        n.dur = Eighth;
        pluck(n);
        delay(Eighth);
      }
      for (auto n : {E, F, G, A, B, C + O, D + O, E + O})
      {
        n += O4;
        n.vel = -6;
        n.dur = Eighth;
        pluck(n);
        delay(Eighth);
      }
    }
  });

  for (;;)
    std::this_thread::sleep_for(std::chrono::seconds(10));
}
