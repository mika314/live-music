#pragma once
#include <shared/consts.hpp>
#include <unordered_set>

class Sink
{
public:
  auto add(class Source *) -> void;
  auto del(Source *) -> void;
  virtual auto lock() const -> void = 0;
  virtual auto unlock() const -> void = 0;

protected:
  std::unordered_set<Source *> sources;
};
