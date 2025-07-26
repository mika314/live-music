#include "source.hpp"
#include "live-music-internal.hpp"
#include "send.hpp"
#include "sink.hpp"
#include <shared/proto.hpp>

auto Source::gain(double v) -> void
{
  gain_ = v;
  ::send(msg::Source_SetGain{.id = getId(), .v = v});
}

auto Source::pan(double v) -> void
{
  ::send(msg::Source_SetPan{.id = getId(), .v = v});
}

auto Source::send(const class Sink &sink, double gain, double pan) -> class Send
{
  return Send{
    Conn::getInst()
      .reqRsp<msg::Source_SendRsp>(
        msg::Source_SendReq{.sourceId = getId(), .sinkId = sink.getId(), .gain = gain, .pan = pan})
      .id};
}

auto Source::getGain() const -> double
{
  return gain_;
}
