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

auto Sample::maj(Note n) -> void
{
  if (isLate())
    return;
  chord(n, I, III, V);
}

auto Sample::min(Note n) -> void
{
  if (isLate())
    return;
  chord(n, I, iii, V);
}
