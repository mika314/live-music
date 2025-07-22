#include "sample.hpp"
#include "conn.hpp"
#include "live-music-internal.hpp"
#include "sink.hpp"

Sample::Sample(Sink &sink, const char *path, double gain, double pan)
  : Source(ctor(msg::Sample_CtorReq{.sinkId = sink.getId(), .path = path, .gain = gain, .pan = pan}))
{
}

auto Sample::operator()(double v) -> void
{
  if (isLate())
    return;
  ::send(msg::Sample_Play{.id = getId(), .vel = v});
}
