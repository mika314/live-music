#include "reverb.hpp"
#include "live-music.hpp"

Reverb::Reverb(Sink &sink)
  : Source(ctor(msg::Reverb_CtorReq{.sinkId = sink.getId()})), Sink(Source::getId())
{
}

auto Reverb::wet(double v) -> void
{
  send(msg::Reverb_SetWet{.id = Sink::getId(), .v = v});
}
