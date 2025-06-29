#include "sink.hpp"

auto Sink::add(class Source *v) -> void
{
  sources.insert(v);
}

auto Sink::del(Source *v) -> void
{
  sources.erase(v);
}
