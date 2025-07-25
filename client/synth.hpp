#pragma once
#include "source.hpp"
#include <shared/envelope.hpp>
#include <shared/note.hpp>
#include <shared/osc_type.hpp>

auto delay(double) -> void;

struct SynthParams
{
  double gain = 0.0;
  double pan = 0.0;
  OscType oscType;
  Envelope envelope;
};

enum Articulation { legato, staccato, unchanged };

class Synth : public Source
{
public:
  Synth(class Sink &, SynthParams = SynthParams{});
  template <typename... Event>
  auto operator()(Event &&...e) -> void
  {
    (play(e), ...);
  }
  auto operator()(Envelope) -> void;
  auto operator()(Note) -> void;
  auto operator()(double) -> void;

private:
  auto play(Note) -> void;
  auto play(const dur &) -> void;
  auto play(const root &) -> void;
  auto play(double) -> void;
  auto play(Articulation) -> void;
  auto play(Rest) -> void;
  auto play(chord<double>) -> void;
  auto play(chord<Note>) -> void;
  double curDur = 1;
  Note rootNote = C4;
  Articulation articulation = Articulation::legato;
};
