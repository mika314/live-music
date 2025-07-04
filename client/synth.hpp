#pragma once
#include "source.hpp"
#include <shared/envelope.hpp>
#include <shared/note.hpp>
#include <shared/osc_type.hpp>
#include <utility>

auto delay(double) -> void;

struct SynthParams
{
  double gain = 0.0;
  double pan = 0.0;
  OscType oscType;
  Envelope envelope;
};

class Synth : public Source
{
public:
  Synth(class Sink &, SynthParams = SynthParams{});
  auto operator()(Note) -> void;

  template <typename... Args>
  auto operator()(Note n0, Args &&...n) -> void
  {
    (*this)(std::move(n0));
    (*this)(std::forward<Args>(n)...);
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
    seq(key, std::forward<Args>(n)...);
  }
  auto chord(Note key, int ofset) -> void { (*this)(key + ofset); }
  template <typename... Args>
  auto chord(Note key, int ofset, Args &&...n) -> void
  {
    chord(key, ofset);
    chord(key, std::forward<Args>(n)...);
  }
};
