#pragma once
#include <shared/consts.hpp>
#include <unordered_set>
#include <vector>

class Sink
{
public:
  Sink() = default;
  auto add(class Source *) -> void;
  auto del(Source *) -> void;
  virtual auto lock() const -> void = 0;
  virtual auto unlock() const -> void = 0;

protected:
  std::unordered_set<Source *> sources;

  auto mix(int samples) -> std::vector<float>;
};
