#include "synth.hpp"
#include "conn.hpp"
#include "live-music.hpp"
#include "sink.hpp"

Synth::Synth(Sink &sink, CtorParams params)
  : Entity(ctor(msg::Synth_CtorReq{.sinkId = sink.getId(),
                                   .oscType = params.oscType,
                                   .envelope = std::move(params.envelope)}))
{
}

auto Synth::operator()(Note v) -> void
{
  if (isLate())
    return;
  send(msg::Synth_Note{.id = getId(), .note = std::move(v)});
}

auto Synth::operator()(Envelope v) -> void
{
  send(msg::Synth_Envelope{.id = getId(), .envelope = std::move(v)});
}

auto Synth::maj(Note v) -> void
{
  (*this)(v);
  (*this)(v + III);
  (*this)(v + V);
}

auto Synth::min(Note v) -> void
{
  (*this)(v);
  (*this)(v + iii);
  (*this)(v + V);
}
