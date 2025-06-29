#include "synth.hpp"
#include "conn.hpp"
#include "sink.hpp"

Synth::Synth(Sink &sink) : Entity(ctor(msg::Synth_CtorReq{.sinkId = sink.getId()})) {}

auto Synth::operator()(Note v) -> void
{
  send(msg::Synth_Note{.id = getId(), .note = std::move(v)});
}
