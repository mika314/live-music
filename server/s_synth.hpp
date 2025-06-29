#pragma once
#include "s_source.hpp"
#include <functional>
#include <shared/note.hpp>
#include <unordered_map>

class Synth : public Source
{
public:
  Synth(const float &bpm, class Sink &);
  auto pull(int samples) -> std::vector<int16_t>;
  auto operator()(Note) -> void;

private:
  std::reference_wrapper<const float> bpm;
  int pos = 0;
  struct N
  {
    float vel = 1.f;
    int end;
  };
  std::unordered_map<int, N> notes;
};
