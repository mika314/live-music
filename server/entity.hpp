#pragma once
#include <cstdint>

class Entity
{
public:
  static auto nextId() -> int32_t;
  Entity();
  virtual ~Entity() = default;
  int32_t id;

private:
  Entity(const Entity &) = delete;
  auto operator=(const Entity &) = delete;
};
