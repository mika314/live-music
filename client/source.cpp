#include "source.hpp"
#include "live-music.hpp"
#include <shared/proto.hpp>

auto Source::gain(double v) -> void
{
  send(msg::Source_SetGain{.id = getId(), .v = v});
}

auto Source::pan(double v) -> void
{
  send(msg::Source_SetPan{.id = getId(), .v = v});
}
