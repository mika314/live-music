#pragma once
#include "entity.hpp"
#include <shared/envelope.hpp>
#include <shared/note.hpp>
#include <shared/osc_type.hpp>

class Synth : public Entity
{
public:
  struct CtorParams
  {
    OscType oscType;
    Envelope envelope;
  };
  Synth(class Sink &, CtorParams = CtorParams{});
  auto operator()(Note) -> void;
  auto operator()(Envelope) -> void;
};
