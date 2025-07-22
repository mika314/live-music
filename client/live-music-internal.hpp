#pragma once
#include "conn.hpp"
#include <chrono>
#include <shared/proto.hpp>
#include <string>

auto getCurTime() -> decltype(std::chrono::steady_clock::now());
auto log(std::string v) -> void;
auto isLate() -> bool;
