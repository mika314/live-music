#pragma once
#include "entity.hpp"

class Source : public Entity
{
public:
  using Entity::Entity;
  auto gain(double) -> void;
  auto pan(double) -> void;
};
