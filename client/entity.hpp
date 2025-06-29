#pragma once
#include <cstdint>

class Entity
{
public:
  Entity(int32_t);
  auto getId() const -> int32_t;

private:
  int32_t id;
};
