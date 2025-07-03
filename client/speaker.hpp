#pragma once
#include "sink.hpp"
#include "source.hpp"

class Speaker : public Sink, public Source
{
public:
  Speaker();
};
