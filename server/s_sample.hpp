#pragma once
#include "s_source.hpp"
#include <atomic>
#include <filesystem>

class Sample : public Source
{
public:
  Sample(const double &bpm, class Sink &, std::filesystem::path, double gain, double pan);
  auto isBusy() const -> bool final;
  auto operator()(double vel) -> void;

private:
  auto internalPull(int samples) -> std::vector<float> final;

  std::atomic_bool isReady = false;
  std::reference_wrapper<const double> bpm;
  std::filesystem::path path;
  std::reference_wrapper<std::vector<float>> sample;
  int pos = 0;
  struct N
  {
    double vel = 0.0; // dB
    int start;
  };
  std::vector<N> notes;
};
