#pragma once
#include "s_source.hpp"
#include <functional>
#include <shared/envelope.hpp>
#include <shared/note.hpp>
#include <unordered_map>

class Synth : public Source
{
public:
  Synth(const float &bpm, class Sink &);
  auto pull(int samples) -> std::vector<float>;
  auto operator()(Note) -> void;
  auto operator()(Envelope) -> void;

private:
  std::reference_wrapper<const float> bpm;
  int pos = 0;
  struct N
  {
    int note;
    float vel = 1.f;
    int start;
    int end;
  };
  std::vector<N> notes;
  Envelope envelope;
};
