#include "speaker.hpp"
#include "conn.hpp"

Speaker::Speaker() : Sink(ctor(msg::Speaker_CtorReq{})), Source(Sink::getId()) {}
