#include "live-music.hpp"

auto main() -> int
{
  setBpm(130);
  auto speaker = Speaker{};
  // thread([&]() {
  //   auto kick = Sample{speaker,
  //                      "/home/mika/.BitwigStudio/installed-packages/1.0/samples/JoMoX/JoMoX "
  //                      "X-Basics/Machine Samples/HW-Mbrane1 Samples/JoMoX Kick 37-01.wav"};
  //   auto clap = Sample{speaker,
  //                      "/mnt/Texas/splice/Samples/packs/Mainstream Pop "
  //                      "House/Dropgun_Samples_-_Mainstream_Pop_House/One_Shots/Drum_One_Shots/Claps/"
  //                      "DS_MPH_drum_clap_one_shot_awake.wav"};
  //   auto loop = Sample{speaker,
  //                      "/mnt/Texas/splice/Samples/packs/Oliver Power Tools Sample Pack "
  //                      "III/OLIVER_VOL3_sample_pack/OLIVER_drums/OLIVER_drum_loops/OLIVER_drum_loops/"
  //                      "OLIVER_noise_glitch/OLIVER_130_drum_loop_noise_glitch_lapsung_metal.wav"};
  //
  //   auto crash = Sample{speaker,
  //                       "/home/mika/.BitwigStudio/installed-packages/1.0/samples/Bajaao/Drums and "
  //                       "Indian Percussion/Acoustic Drums/Nektar A Vintage 16 Crash 01.wav"};
  //   auto breakBeat =
  //     Sample{speaker,
  //            "/mnt/Texas/splice/Samples/packs/Future "
  //            "Catalyst/Future_Catalyst/Loops/Drum_loops/Fills/RKU_FC_130_Fills_Erosion.wav"};
  //   for (;;)
  //   {
  //     crash(-6);
  //     for (auto i = 0; i < 4 * 4; ++i)
  //     {
  //       if (i == 3 * 4)
  //         breakBeat(-15);
  //       if (i % (2 * 4) == 0)
  //         loop(-25);
  //
  //       kick(i % 4 == 0 ? -10 : -12);
  //
  //       if (i % 2 == 1)
  //         clap(-18);
  //
  //       delay(Quarter);
  //     }
  //   }
  // });

  // thread([&]() {
  //   auto synth = createBass(speaker);
  //   for (;;)
  //     for (auto n : {C, C, C, C, D, D, D, D, F, F, F, F, E, E, E, E})
  //     {
  //       n += O2;
  //       n.vel = -20;
  //       n.dur = Quarter;
  //       synth(n);
  //       delay(Quarter);
  //     }
  // });
  // thread([&]() {
  //  auto synth = createPluck(speaker);
  //  for (;;)
  //    for (auto n : {E, G, G, E, D, C, C, D, F, G, F, G, E, F, E, D})
  //    {
  //      n += O5;
  //      n.vel = -10;
  //      n.dur = Eight;
  //      synth(n);
  //      delay(Eight);
  //      synth(n);
  //      delay(Sixteenth);
  //      n.vel = -15;
  //      synth(n);
  //      delay(Sixteenth);
  //    }
  //});
  // thread([&]() {
  //   auto synth = createPad(speaker);
  //   for (;;)
  //   {
  //     {
  //       auto n = C + O3;
  //       n.vel = -25;
  //       n.dur = Whole;
  //       synth(n);
  //       synth(n + III);
  //       synth(n + V);
  //       synth(n + VII);
  //       delay(Whole);
  //     }
  //     {
  //       auto n = D + O3;
  //       n.vel = -25;
  //       n.dur = Whole;
  //       synth(n);
  //       synth(n + iii);
  //       synth(n + V);
  //       synth(n + 10);
  //       delay(Whole);
  //     }
  //     {
  //       auto n = F + O3;
  //       n.vel = -25;
  //       n.dur = Whole;
  //       synth(n);
  //       synth(n + III);
  //       synth(n + V);
  //       synth(n + VII - O);
  //       delay(Whole);
  //     }
  //     {
  //       auto n = E + O3;
  //       n.vel = -25;
  //       n.dur = Whole;
  //       synth(n);
  //       synth(n + iii);
  //       synth(n + V);
  //       synth(n + 10 - O);
  //       delay(Whole);
  //     }
  //   }
  // });
  for (;;)
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }
}
