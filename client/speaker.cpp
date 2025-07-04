#include "speaker.hpp"
#include "conn.hpp"

Speaker::Speaker(double gain, double pan)
  : Sink(ctor(msg::Speaker_CtorReq{.gain = gain, .pan = pan})), Source(Sink::getId())
{
}
