#pragma once
#include "s_sink.hpp"
#include "s_source.hpp"

class CombFilter
{
public:
  CombFilter(int delaySamples, float feedback)
    : buffer(delaySamples), bufLen(delaySamples), writePos(0), fb(feedback)
  {
  }

  // process one sample
  float process(float in)
  {
    float out = buffer[writePos];
    float fbSample = out * fb + in;
    buffer[writePos] = fbSample;
    if (++writePos >= bufLen)
      writePos = 0;
    return out;
  }

private:
  std::vector<float> buffer;
  int bufLen, writePos;
  float fb;
};
class AllpassFilter
{
public:
  AllpassFilter(int delaySamples, float feedback)
    : buffer(delaySamples), bufLen(delaySamples), writePos(0), fb(feedback)
  {
  }

  float process(float in)
  {
    float bufOut = buffer[writePos];
    float output = -in + bufOut;
    float bufIn = in + bufOut * fb;
    buffer[writePos] = bufIn;
    if (++writePos >= bufLen)
      writePos = 0;
    return output;
  }

private:
  std::vector<float> buffer;
  int bufLen, writePos;
  float fb;
};

class Reverb : public Sink, public Source
{
public:
  Reverb(class Sink &);
  auto lock() const -> void final;
  auto unlock() const -> void final;
  auto pull(int samples) -> std::vector<float> final;
  auto isBusy() const -> bool final;
  auto setWet(double) -> void;

private:
  float process(float in);
  void setMix(float wetLevel);

  std::reference_wrapper<Sink> sink;
  std::vector<CombFilter> combs;
  std::vector<AllpassFilter> allpasses;
  float wet = 0.3f;
  float dry = 0.7f;
};
