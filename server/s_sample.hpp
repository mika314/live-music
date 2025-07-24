#pragma once
#include "s_source.hpp"
#include <atomic>
#include <filesystem>
#include <shared/envelope.hpp>
#include <shared/note.hpp>

class Sample : public Source
{
public:
  Sample(const double &bpm, class Sink &, std::filesystem::path, double gain, double pan, Note);
  auto isBusy() const -> bool final;
  auto play(Note) -> void;
  auto set(Envelope) -> void;

private:
  auto internalPull(int samples) -> std::vector<float> final;

  std::atomic_bool isReady = false;
  std::reference_wrapper<const double> bpm;
  std::filesystem::path path;
  std::reference_wrapper<std::vector<float>> sample;
  Note note;
  Envelope envelope;
  int pos = 0;
  struct N
  {
    Note note;
    int start;
  };
  std::vector<N> notes;
};
