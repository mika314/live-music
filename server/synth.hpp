#pragma once
#include "source.hpp"

class Synth : public Source
{
public:
  Synth(class Sink &);
  auto pull(int samples) -> std::vector<int16_t>;

private:
  int pos = 0;
};
