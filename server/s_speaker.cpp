#include "s_speaker.hpp"
#include "s_source.hpp"
#include <log/log.hpp>

Speaker::Speaker()
  : want{.freq = SampleRate, .format = AUDIO_F32LSB, .channels = ChN, .samples = 1024},
    audio(nullptr, false, &want, &have, 0, [this](Uint8 *stream, int len) {
      memset(stream, 0, len);
      for (auto &s : sources)
      {
        const auto samples = len / sizeof(float) / ChN;
        const auto chunk = s->pull(samples);
        for (auto i = 0U; i < samples * ChN; ++i)
          reinterpret_cast<float *>(stream)[i] += chunk[i];
      }
    })
{
  audio.pause(0);
}

auto Speaker::lock() -> void
{
  audio.lock();
}

auto Speaker::unlock() -> void
{
  audio.unlock();
}
