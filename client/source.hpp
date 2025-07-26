#pragma once
#include "entity.hpp"

class Source : public Entity
{
public:
  using Entity::Entity;
  auto gain(double) -> void;
  auto getGain() const -> double;
  auto pan(double) -> void;
  auto send(const class Sink &, double gain = 0, double pan = 0) -> class Send;

private:
  double gain_;
};
