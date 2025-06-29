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
  auto synth = Synth{speaker};
  for (auto i = 0; i < 16; ++i)
  {
    synth(Note{.n = 0, .dur = 0.5, .vel = 1});
    delay(1);
    synth(Note{.n = 5, .dur = 0.5, .vel = 1});
    delay(1);
    synth(Note{.n = 7, .dur = 0.5, .vel = 1});
    delay(1);
    synth(Note{.n = 0, .dur = 0.25, .vel = 0.5});
    delay(0.5);
    synth(Note{.n = 0, .dur = 0.25, .vel = .7});
    delay(0.5);
  }

  t.join();

  LOG("now: ", now());
}
