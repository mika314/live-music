#pragma once
#include "entity.hpp"
#include <shared/note.hpp>

class Synth : public Entity
{
public:
  Synth(class Sink &);
  auto operator()(Note) -> void;
};
