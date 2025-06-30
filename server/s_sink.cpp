#include "s_sink.hpp"

auto Sink::add(class Source *v) -> void
{
  lock();
  sources.insert(v);
  unlock();
}

auto Sink::del(Source *v) -> void
{
  lock();
  sources.erase(v);
  unlock();
}
