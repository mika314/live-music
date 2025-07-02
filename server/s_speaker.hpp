#pragma once
#include "s_entity.hpp"
#include "s_sink.hpp"
#include "s_source.hpp"

class Speaker : public Sink, public Source
{
public:
  Speaker(class MasterSpeaker &);
  auto lock() const -> void final;
  auto unlock() const -> void final;
  auto pull(int samples) -> std::vector<float> final;
  auto isBusy() const -> bool final;

private:
  std::reference_wrapper<MasterSpeaker> masterSpeaker;
};
