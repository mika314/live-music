#pragma once
#include <shared/consts.hpp>
#include <unordered_set>

class Sink
{
public:
  auto add(class Source *) -> void;
  auto del(Source *) -> void;
  virtual auto lock() -> void = 0;
  virtual auto unlock() -> void = 0;

protected:
  std::unordered_set<Source *> sources;
};
