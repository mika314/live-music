#pragma once
#include "s_source.hpp"
#include <deque>

class Send : public Source
{
  friend class Source;

public:
  Send(class Sink &, Source &owner, double gain, double pan);

private:
  auto internalPull(int samples) -> std::vector<float> final;
  auto isBusy() const -> bool final;

  std::reference_wrapper<Source> owner;
  std::deque<std::vector<float>> q;
};
