#pragma once
#include "source.hpp"
#include <shared/envelope.hpp>
#include <shared/note.hpp>

auto delay(double) -> void;

class Sample : public Source
{
public:
  Sample(class Sink &, const char *, double gain = 0, double pan = 0, Note = C + O4);
  auto operator()(double = -12) -> void;
  auto operator()(Note) -> void;

  template <typename... Args>
  auto operator()(Note n0, Args &&...n) -> void
  {
    (*this)(n0);
    (*this)(n...);
  }
  auto operator()(Envelope) -> void;
  auto maj(Note) -> void;
  auto min(Note) -> void;
  auto seq(Note key, int ofset, double dur) -> void
  {
    if (ofset >= 0)
      (*this)(key + ofset);
    delay(dur);
  }
  template <typename... Args>
  auto seq(Note key, int ofset, double dur, Args &&...n) -> void
  {
    seq(key, ofset, dur);
    seq(key, n...);
  }
  auto chord(Note key, int ofset) -> void { (*this)(key + ofset); }
  template <typename... Args>
  auto chord(Note key, int ofset, Args &&...n) -> void
  {
    chord(key, ofset);
    chord(key, n...);
  }
};
