#include "sample.hpp"
#include "conn.hpp"
#include "live-music.hpp"
#include "sink.hpp"

Sample::Sample(Sink &sink, std::filesystem::path path)
  : Source(ctor(msg::Sample_CtorReq{.sinkId = sink.getId(), .path = path.string()}))
{
}

auto Sample::operator()(double v) -> void
{
  if (isLate())
    return;
  send(msg::Sample_Play{.id = getId(), .vel = v});
}
