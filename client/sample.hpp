#pragma once
#include "source.hpp"
#include "synth.hpp"
#include <shared/envelope.hpp>
#include <shared/note.hpp>

auto delay(double) -> void;

class Sample : public Source
{
public:
  Sample(class Sink &, const char *, double gain = 0, double pan = 0, Note = C4);
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
