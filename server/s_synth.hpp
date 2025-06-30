#pragma once
#include "s_entity.hpp"
#include "s_source.hpp"
#include <functional>
#include <shared/envelope.hpp>
#include <shared/note.hpp>
#include <shared/osc_type.hpp>
#include <unordered_map>

class Synth : public Source, public Entity
{
public:
  Synth(const float &bpm, class Sink &, OscType, Envelope);
  auto pull(int samples) -> std::vector<float>;
  auto operator()(Note) -> void;
  auto operator()(Envelope) -> void;
  auto isBusy() const -> bool final;

private:
  std::reference_wrapper<const float> bpm;
  OscType oscType;
  Envelope envelope;
  int pos = 0;
  struct N
  {
    float note;
    float vel = 1.f;
    int start;
    int end;
  };
  std::vector<N> notes;

  auto osc(float freq) -> float;
};
