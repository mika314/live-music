#include "live-music.hpp"

auto main() -> int
{
  setBpm(120);
  auto master = Speaker{};
  auto reverb = Reverb{master};
  reverb.wet(1.0);

  thread([&]() {
    auto kick =
      Sample{master, "samples/stargate-sample-pack/karoryfer/kicks/kick_Polmuz_mod_20_clean.wav", -8};
    for (;;)
    {
      kick(d4, I, I, I, d8, I, I);
    }
  });

  thread([&]() {
    auto synth = createPluck(master, -15);
    synth.send(reverb, -5);
    for (;;)
    {
      // clang-format off
       synth(root{C4},
             Articulation::legato,
             d4,I,  I,  V,  V,  VI, VI, d2,  V,
             d4,IV, IV, III,III,II, II, d2,  I,
             d4,V,  V,  IV, IV, III,III,d2, II,
             d4,V,  V,  IV, IV, III,III,d2, II);
      // clang-format on
    }
  });
  thread([&]() {
    auto synth = createPad(master, -15);
    synth.send(reverb, -10);
    for (;;)
    {
      // clang-format off
      synth(root{C3},
            Articulation::legato,
            Bar,ChI(I, III, V),
            d2,ChN(C3, F3, A3),
            d2,ChI(I, III, V),
            d2,ChN(C3, F3, A3),
            d2,ChI(I, III, V),
            d2,ChN(D3, G3, B3),
            d2,ChI(I, III, V),

            d2,ChI(I, III, V),
            d2,ChN(C3, F3, A3),
            d2,ChI(I, III, V),
            d2,ChN(D3, G3, B3),
            d2,ChI(I, III, V),
            d2,ChN(C3, F3, A3),
            d2,ChN(D3, G3, B3),
            d2,ChI(I, III, V)
      );
      // clang-format on
    }
  });
  runForever();
}
