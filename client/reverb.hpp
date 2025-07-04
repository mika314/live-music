#pragma once
#include "sink.hpp"
#include "source.hpp"

class Reverb : public Source, public Sink
{
public:
  Reverb(class Sink &, double gain = 0, double pan = 0);
  auto wet(double) -> void;
};
