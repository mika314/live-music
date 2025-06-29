#pragma once
#include "entity.hpp"
#include <shared/consts.hpp>
#include <unordered_set>

class Sink : public Entity
{
public:
  auto add(class Source *) -> void;
  auto del(Source *) -> void;

protected:
  std::unordered_set<Source *> sources;
};
