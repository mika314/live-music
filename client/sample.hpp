#pragma once
#include "source.hpp"

class Sample : public Source
{
public:
  Sample(class Sink &, const char *, double gain = 0, double pan = 0);
  auto operator()(double = -12) -> void;
};
