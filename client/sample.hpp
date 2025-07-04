#pragma once
#include "source.hpp"
#include <filesystem>

class Sample : public Source
{
public:
  Sample(class Sink &, std::filesystem::path, double gain = 0, double pan = 0);
  auto operator()(double = -12) -> void;
};
