#include "live-music.hpp"

auto main() -> int
{
  setBpm(130);
  auto speaker = Speaker{};
  thread([&]() {
    auto synth =
      Synth{speaker,
            Synth::CtorParams{.oscType = OscType::sin,
                              .envelope =
                                Envelope{.attack = .01, .decay = .02, .sustain = -40, .release = .01}}};

    for (auto i = 0;; ++i)
      for (auto j : {C, E, G, C + O, C, E, G, C + O})
      {
        synth(Note{.n = 60. + j, .dur = .5, .vel = (-15. + (i % j == 0 ? 7. : 0.))});
        synth(Note{.n = 72. + j, .dur = .5, .vel = (-15. + (i % j == 0 ? 7. : 0.))});
        delay(.5);
      }
  });

  for (;;)
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }
}
