#pragma once
#include "s_entity.hpp"
#include "s_sink.hpp"
#include <sdlpp/sdlpp.hpp>

class MasterSpeaker : public Sink
{
public:
  MasterSpeaker();
  auto lock() -> void final;
  auto unlock() -> void final;
  std::vector<std::unique_ptr<Entity>> orphanage;

private:
  SDL_AudioSpec want;
  SDL_AudioSpec have;
  sdl::Audio audio;
};
