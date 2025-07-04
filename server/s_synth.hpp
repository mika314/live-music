#pragma once
#include "s_source.hpp"
#include <functional>
#include <shared/envelope.hpp>
#include <shared/note.hpp>
#include <shared/osc_type.hpp>
#include <unordered_map>

class Synth : public Source
{
public:
  Synth(const double &bpm, class Sink &, double gain, double pan, OscType, Envelope);
  auto isBusy() const -> bool final;
  auto operator()(Envelope) -> void;
  auto operator()(Note) -> void;
  auto internalPull(int samples) -> std::vector<float> final;

private:
  std::reference_wrapper<const double> bpm;
  OscType oscType;
  Envelope envelope;
  int pos = 0;
  struct N
  {
    double note;
    double vel = 0.0; // dB
    int start;
    int end;
  };
  std::vector<N> notes;

  auto osc(float freq) -> float;
};
