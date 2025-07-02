#pragma once
#include "entity.hpp"
#include <filesystem>

class Sample : public Entity
{
public:
  Sample(class Sink &, std::filesystem::path);
  auto operator()(double = -12) -> void;
};
