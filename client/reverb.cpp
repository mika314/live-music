#include "reverb.hpp"
#include "live-music-internal.hpp"

Reverb::Reverb(Sink &sink, double gain, double pan)
  : Source(ctor(msg::Reverb_CtorReq{.sinkId = sink.getId(), .gain = gain, .pan = pan})),
    Sink(Source::getId())
{
}

auto Reverb::wet(double v) -> void
{
  ::send(msg::Reverb_SetWet{.id = Sink::getId(), .v = v});
}
