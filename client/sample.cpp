#include "sample.hpp"
#include "conn.hpp"
#include "live-music-internal.hpp"
#include "sink.hpp"
#include <shared/note.hpp>

Sample::Sample(Sink &sink, const char *path, double gain, double pan, Note note)
  : Source(ctor(
      msg::Sample_CtorReq{.sinkId = sink.getId(), .path = path, .gain = gain, .pan = pan, .note = note}))
{
}

auto Sample::operator()(double v) -> void
{
  if (isLate())
    return;
  ::send(msg::Sample_Note{.id = getId(), .note = Note{.vel = v}});
}

auto Sample::operator()(Note n) -> void
{
  if (isLate())
    return;
  ::send(msg::Sample_Note{.id = getId(), .note = n});
}

auto Sample::operator()(Envelope e) -> void
{
  if (isLate())
    return;
  ::send(msg::Sample_Envelope{.id = getId(), .envelope = e});
}

auto Sample::play(Note n) -> void
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

auto Sample::play(const root &v) -> void
{
  rootNote = v.v;
}

auto Sample::play(const dur &v) -> void
{
  curDur = v.v;
}

auto Sample::play(double v) -> void
{
  play(rootNote + v);
}

auto Sample::play(Articulation v) -> void
{
  articulation = v;
}

auto Sample::play(Rest) -> void
{
  delay(curDur);
}

auto Sample::play(chord<double> v) -> void
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

auto Sample::play(chord<Note> v) -> void
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