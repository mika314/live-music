#pragma once
#include "conn.hpp"
#include "speaker.hpp"
#include "synth.hpp"
#include <chrono>
#include <shared/consts.hpp>

auto log(std::string v) -> void;
auto now() -> int;
auto setBpm(double) -> void;
auto delay(double) -> void;
auto newThread() -> void;
auto getCurTime() -> decltype(std::chrono::steady_clock::now());
auto thread(std::function<auto()->void>) -> std::thread *;

static int C = 0;
static int Cs = 1;
static int Db = 1;
static int D = 2;
static int Ds = 3;
static int Eb = 3;
static int E = 4;
static int F = 5;
static int Fs = 6;
static int Gb = 6;
static int G = 7;
static int Gs = 8;
static int Ab = 8;
static int A = 9;
static int As = 10;
static int Bb = 10;
static int B = 11;
static int I = 0;
static int II = 2;
static int III = 4;
static int iii = 3;
static int IV = 5;
static int V = 7;
static int VI = 9;
static int VII = 11;
static int O = 12;
