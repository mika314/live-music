#pragma once
#include "source.hpp"
#include <filesystem>

class Sample : public Source
{
public:
  Sample(class Sink &, std::filesystem::path);
  auto operator()(double = -12) -> void;
};
