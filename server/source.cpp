#include "source.hpp"
#include "sink.hpp"

Source::Source(Sink &aSink) : sink(aSink)
{
  sink.get().add(this);
}

Source::~Source()
{
  sink.get().del(this);
}
