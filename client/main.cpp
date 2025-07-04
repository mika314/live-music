#include "live-music.hpp"

auto main() -> int
{
  setBpm(120);
  auto master = Speaker{};
  auto reverb = Reverb{master};
  reverb.wet(1);
  thread([&]() {
    auto loop =
      Sample{master, "samples/stargate-sample-pack/fugue-state-audio/loops/120-filterhats.wav", -12};

    auto kick =
      Sample{master, "samples/stargate-sample-pack/karoryfer/kicks/kick_Szpaderski_24_dampened.wav", -5};
    kick.send(reverb, -12, 0);

    auto snare =
      Sample{master, "samples/stargate-sample-pack/karoryfer/snares/snare_Pearl_alumunum_14x8.wav", -5};
    snare.send(reverb, -12, .25);

    auto clap = Sample{
      master, "samples/stargate-sample-pack/freesound/drums/clap/388042__sami-kullstrom__clap.wav", -12};
    clap.send(reverb, -15, .25);

    for (;;)
    {
      loop(-35);
      for (auto i = 0; i < 4 * 4; ++i)
      {
        kick();
        if (i % 2 == 1)
        {
          snare(-20);
          clap(-15);
        }
        delay(d8);
        if (i % 2 == 1)
          clap(-25);
        delay(d8);
      }
    }
  });

  thread([&]() {
    auto hat = Sample(master, "samples/stargate-sample-pack/karoryfer/hihats/hihat_BRD_tight.wav", -5);
    for (;;)
    {
      hat(-25 - rand() % 6);
      if (rand() % 8 != 0)
        delay(d8);
      else
      {
        delay(d16);
        hat(-25 - rand() % 6);
        delay(d16);
      }
    }
  });

  thread([&]() {
    auto bass = createBass(master);
    // clang-format off
     for (;;)
       bass.seq(C.setVel(-25).setDur(d8) + O2,
                I,   d4, I,   d4, I,   d4, I,   d4,
                II,  d4, II,  d4, II,  d4, II,  d4,
                IV,  d4, IV,  d4, IV,  d4, IV,  d4,
                III, d4, III, d4, III, d4, III, d4);
    // clang-format on
  });

  thread([&]() {
    auto pad = createPad(master, -15);
    pad.send(reverb, -5, 0);
    for (;;)
    {
      pad.chord(C.setVel(-25).setDur(Bar) + O3, I, III, V, VII);
      delay(Bar);
      pad.chord(D.setVel(-25).setDur(Bar) + O3, I, iii, V, vii);
      delay(Bar);
      pad.chord(F.setVel(-25).setDur(Bar) + O3, I, III, V, VII);
      delay(Bar);
      pad.chord(E.setVel(-25).setDur(Bar) + O3, I, iii, V, vii);
      delay(Bar);
    }
  });
  thread([&]() {
    auto pluck = createPluck(master);
    pluck.send(reverb, -15, 0);
    for (;;)
    {
      // clang-format off
      pluck.seq(C.setVel(0).setDur(d16) + O4,
                I,   d8, II,  d8, III, d8, IV,  d8, V,   d8, VI,     d8, VII,     d8, O,       d8,
                II,  d8, III, d8, IV,  d8, V,   d8, VI,  d8, VII,    d8, O,       d8, O + II,  d8,
                IV,  d8, V,   d8, VI,  d8, VII, d8, O,   d8, O + II, d8, O + III, d8, O + IV,  d8,
                III, d8, IV,  d8, V,   d8, VI,  d8, VII, d8, O,      d8, O + II,  d8, O + III, d8);
      // clang-format on
    }
  });

  for (;;)
    std::this_thread::sleep_for(std::chrono::seconds(10));
}
