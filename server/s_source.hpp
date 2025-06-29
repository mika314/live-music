#pragma once
#include "s_entity.hpp"
#include <vector>

class Source : public Entity
{
public:
  Source(class Sink &);
  virtual ~Source();
  virtual auto pull(int samples) -> std::vector<int16_t> = 0;

protected:
  std::reference_wrapper<Sink> sink;
};
