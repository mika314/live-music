#include "entity.hpp"

Entity::Entity(int32_t aId) : id(aId) {}

auto Entity::getId() const -> int32_t
{
  return id;
}
