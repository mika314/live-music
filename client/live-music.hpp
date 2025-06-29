#pragma once
#include "conn.hpp"
#include "speaker.hpp"
#include "synth.hpp"

auto log(std::string v) -> void;
auto now() -> int;
auto setBpm(float) -> void;
auto delay(float) -> void;
