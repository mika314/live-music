#include "live-music.hpp"
#include <log/log.hpp>

auto main() -> int
{
  Conn::init();

  setBpm(130);

  auto t = std::thread{[]() {
    for (auto i = 0; i < 10; ++i)
      log("Hello world! Secondary thread! " + std::to_string(i));
  }};

  for (auto i = 0; i < 10; ++i)
    log("Hello world! Main thread! " + std::to_string(i));

  auto speaker = Speaker{};
  auto synth = Synth{
    speaker,
    Synth::CtorParams{.oscType = OscType::triangle,
                      .envelope = Envelope{.attack = .1, .decay = .5, .sustain = -20, .release = .5}}};
  for (auto i = 0; i < 4; ++i)
  {
    synth(Note{.n = 60 + 0, .dur = 1, .vel = -20});
    synth(Note{.n = 60 + 4, .dur = 1, .vel = -20});
    synth(Note{.n = 60 + 7, .dur = 1, .vel = -20});
    delay(1);
    synth(Note{.n = 60 + 0, .dur = 1, .vel = -20});
    synth(Note{.n = 60 + 4, .dur = 1, .vel = -20});
    synth(Note{.n = 60 + 7, .dur = 1, .vel = -20});
    delay(1);
    synth(Note{.n = 60 + 0, .dur = 1, .vel = -20});
    synth(Note{.n = 60 + 4, .dur = 1, .vel = -20});
    synth(Note{.n = 60 + 7, .dur = 1, .vel = -20});
    delay(1);
    synth(Note{.n = 60, .dur = .5, .vel = -20});
    delay(0.5);
    synth(Note{.n = 60, .dur = .5, .vel = -20});
    delay(0.5);
  }

  t.join();

  LOG("now: ", now());
}
