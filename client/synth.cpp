#include "synth.hpp"
#include "conn.hpp"
#include "live-music-internal.hpp"
#include "live-music.hpp"
#include "sink.hpp"

Synth::Synth(Sink &sink, SynthParams params)
  : Source(ctor(msg::Synth_CtorReq{.sinkId = sink.getId(),
                                   .gain = params.gain,
                                   .pan = params.pan,
                                   .oscType = params.oscType,
                                   .envelope = std::move(params.envelope)}))
{
}

auto Synth::operator()(Note v) -> void
{
  if (isLate())
    return;
  ::send(msg::Synth_Note{.id = getId(), .v = std::move(v)});
}

auto Synth::operator()(double v) -> void
{
  operator()(rootNote + v);
}

auto Synth::operator()(Envelope v) -> void
{
  ::send(msg::Synth_Envelope{.id = getId(), .envelope = std::move(v)});
}

auto Synth::play(Note n) -> void
{
  switch (articulation)
  {
  case Articulation::legato: n.setDur(curDur); break;
  case Articulation::staccato: n.setDur(curDur / 4); break;
  case Articulation::unchanged: break;
  }

  operator()(n);
  delay(curDur);
}

auto Synth::play(const root &v) -> void
{
  rootNote = v.v;
}

auto Synth::play(const dur &v) -> void
{
  curDur = v.v;
}

auto Synth::play(const vel &v) -> void
{
  rootNote.vel = v.v;
}

auto Synth::play(double v) -> void
{
  play(rootNote + v);
}

auto Synth::play(Articulation v) -> void
{
  articulation = v;
}

auto Synth::play(Rest) -> void
{
  delay(curDur);
}

auto Synth::play(chord<double> v) -> void
{
  for (auto i : v.v)
  {
    auto n = rootNote + i;
    switch (articulation)
    {
    case Articulation::legato: n.setDur(curDur); break;
    case Articulation::staccato: n.setDur(curDur / 4); break;
    case Articulation::unchanged: break;
    }

    operator()(n);
  }
  delay(curDur);
}

auto Synth::play(chord<Note> v) -> void
{
  for (auto n : v.v)
  {
    switch (articulation)
    {
    case Articulation::legato: n.setDur(curDur); break;
    case Articulation::staccato: n.setDur(curDur / 4); break;
    case Articulation::unchanged: break;
    }

    operator()(n);
  }
  delay(curDur);
}
