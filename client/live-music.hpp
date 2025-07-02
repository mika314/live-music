#pragma once
#include "conn.hpp"
#include "sample.hpp"
#include "speaker.hpp"
#include "synth.hpp"
#include <chrono>
#include <shared/consts.hpp>

auto createBass(Sink &) -> Synth;
auto createPad(Sink &v) -> Synth;
auto createPluck(Sink &) -> Synth;
auto delay(double) -> void;
auto getCurTime() -> decltype(std::chrono::steady_clock::now());
auto log(std::string v) -> void;
auto newThread() -> void;
auto now() -> int;
auto setBpm(double) -> void;
auto thread(std::function<auto()->void>) -> std::thread *;
auto isLate() -> bool;

static auto C = Note{.n = 0};
static auto Cs = Note{.n = 1};
static auto Db = Note{.n = 1};
static auto D = Note{.n = 2};
static auto Ds = Note{.n = 3};
static auto Eb = Note{.n = 3};
static auto E = Note{.n = 4};
static auto F = Note{.n = 5};
static auto Fs = Note{.n = 6};
static auto Gb = Note{.n = 6};
static auto G = Note{.n = 7};
static auto Gs = Note{.n = 8};
static auto Ab = Note{.n = 8};
static auto A = Note{.n = 9};
static auto As = Note{.n = 10};
static auto Bb = Note{.n = 10};
static auto B = Note{.n = 11};
static auto I = 0.0;
static auto II = 2.;
static auto III = 4.;
static auto iii = 3.;
static auto IV = 5.;
static auto V = 7.;
static auto VI = 9.;
static auto VII = 11.;
static auto O = 12.;
static auto O0 = 12.;
static auto O1 = 24.;
static auto O2 = 36.;
static auto O3 = 48.;
static auto O4 = 60.;
static auto O5 = 72.;
static auto O6 = 90.;
static auto Sixteenth = .25;
static auto Eight = .5;
static auto Quarter = 1.;
static auto Halth = 2.;
static auto Whole = 4.;
static auto Bar = 4.;
