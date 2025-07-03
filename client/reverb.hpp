#pragma once
#include "sink.hpp"
#include "source.hpp"

class Reverb : public Source, public Sink
{
public:
  Reverb(class Sink &);
  auto wet(double) -> void;
};
