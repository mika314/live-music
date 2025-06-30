#include "s_source.hpp"
#include "s_sink.hpp"
#include <log/log.hpp>

Source::Source(Sink &aSink) : sink(aSink)
{
  sink.get().add(this);
}

Source::~Source()
{
  sink.get().del(this);
}
