#pragma once
#include "s_sink.hpp"
#include <sdlpp/sdlpp.hpp>

class Speaker : public Sink
{
public:
  Speaker();
  auto lock() -> void final;
  auto unlock() -> void final;

private:
  SDL_AudioSpec want;
  SDL_AudioSpec have;
  sdl::Audio audio;
};
