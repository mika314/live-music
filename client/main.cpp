#include "live-music.hpp"
#include <log/log.hpp>

auto main() -> int
{
  Conn::init();

  setBpm(130);

  auto speaker = Speaker{};

  auto t = std::thread{[&]() {
    log("Bass");
    auto synth = Synth{speaker,
                       Synth::CtorParams{.oscType = OscType::square,
                                         .envelope = Envelope{
                                           .attack = .02, .decay = .25, .sustain = -20, .release = 3}}};

    const auto v = -96;
    for (auto h : {0.0f, 7.f, 0.0f, 7.f})
    {
      synth(Note{.n = 36.f + h, .dur = 1, .vel = v + 6});
      delay(0.5);
      synth(Note{.n = 36.f + h, .dur = 1, .vel = v - 12});
      delay(0.5);
      synth(Note{.n = 36.f + h + 12, .dur = 1, .vel = v});
      delay(0.5);
      synth(Note{.n = 36.f + h + 12, .dur = 1, .vel = v - 12});
      delay(0.5);
      synth(Note{.n = 36.f + h, .dur = 1, .vel = v + 3});
      delay(0.5);
      synth(Note{.n = 36.f + h, .dur = 1, .vel = v - 12});
      delay(0.5);
      synth(Note{.n = 36.f + h + 12, .dur = 1, .vel = v});
      delay(0.5);
      synth(Note{.n = 36.f + h + 12, .dur = 1, .vel = v - 12});
      delay(0.5);
    }
  }};

  log("Chords");

  auto t2 = std::thread{[&]() {
    auto synth = Synth{speaker,
                       Synth::CtorParams{.oscType = OscType::triangle,
                                         .envelope = Envelope{
                                           .attack = .1, .decay = .25, .sustain = -40, .release = .5}}};
    for (auto i = 0; i < 4; ++i)
    {
      const auto v = -94;
      synth(Note{.n = 60 + 0, .dur = 0.5, .vel = v});
      synth(Note{.n = 60 + 4, .dur = 0.5, .vel = v});
      synth(Note{.n = 60 + 7, .dur = 0.5, .vel = v});
      delay(0.5);
      synth(Note{.n = 72 + 0, .dur = 0.5, .vel = v - 6});
      synth(Note{.n = 72 + 4, .dur = 0.5, .vel = v - 6});
      synth(Note{.n = 72 + 7, .dur = 0.5, .vel = v - 6});
      delay(0.5);
      synth(Note{.n = 60 + 0, .dur = 0.5, .vel = v});
      synth(Note{.n = 60 + 4, .dur = 0.5, .vel = v});
      synth(Note{.n = 60 + 7, .dur = 0.5, .vel = v});
      delay(0.5);
      synth(Note{.n = 72 + 0, .dur = 0.5, .vel = v - 6});
      synth(Note{.n = 72 + 4, .dur = 0.5, .vel = v - 6});
      synth(Note{.n = 72 + 7, .dur = 0.5, .vel = v - 6});
      delay(0.5);
      synth(Note{.n = 60 + 0, .dur = 0.5, .vel = v});
      synth(Note{.n = 60 + 4, .dur = 0.5, .vel = v});
      synth(Note{.n = 60 + 7, .dur = 0.5, .vel = v});
      delay(0.5);
      synth(Note{.n = 72 + 0, .dur = 0.5, .vel = v - 6});
      synth(Note{.n = 72 + 4, .dur = 0.5, .vel = v - 6});
      synth(Note{.n = 72 + 7, .dur = 0.5, .vel = v - 6});
      delay(0.5);
      synth(Note{.n = 60, .dur = .5, .vel = v});
      delay(0.5);
      synth(Note{.n = 72, .dur = .5, .vel = v - 12});
      delay(0.5);
    }
  }};
  auto t3 = std::thread{[&]() {
    auto synth = Synth{
      speaker,
      Synth::CtorParams{.oscType = OscType::triangle,
                        .envelope = Envelope{.attack = .1, .decay = .3, .sustain = -20, .release = 1}}};
    const auto v = -12.f;
    for (auto k : {C, D, F, C})
      for (auto n : {I,
                     k == D ? iii : III,
                     7,
                     k == D || k == G ? I : VII,
                     0,
                     7,
                     k == D || k == G ? I : VII,
                     k == D ? iii : III})
      {
        synth(Note{.n = 60.0f + (k + n) % 24, .dur = 0.5, .vel = v + rand() % 10 - 5});
        delay(0.5);
      }
  }};
  auto t4 = std::thread{[&]() {
    auto synth = Synth{speaker,
                       Synth::CtorParams{.oscType = OscType::triangle,
                                         .envelope = Envelope{
                                           .attack = .01, .decay = .1, .sustain = -20, .release = .1}}};
    const auto v = -10.f;
    for (auto k : {C, D, F, C})
      for (auto n : {I, I + O, I, I + O})
        for (auto i : {I, V, O, I})
        {
          synth(Note{.n = 48.0f + (k + n + i) % 24, .dur = 0.25, .vel = v + rand() % 10 - 5});
          delay(0.25);
        }
  }};

  t.join();
  t2.join();
  t3.join();
  t4.join();

  LOG("now: ", now());
}
