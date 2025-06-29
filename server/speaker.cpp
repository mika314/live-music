#include "speaker.hpp"
#include "source.hpp"
#include <log/log.hpp>

Speaker::Speaker()
  : want{.freq = SampleRate, .format = AUDIO_S16, .channels = ChN, .samples = 1024},
    audio(nullptr, false, &want, &have, 0, [this](Uint8 *stream, int len) {
      memset(stream, 0, len);
      for (auto &s : sources)
      {
        const auto samples = len / sizeof(int16_t) / ChN;
        const auto chunk = s->pull(samples);
        for (auto i = 0U; i < samples * ChN; ++i)
          reinterpret_cast<int16_t *>(stream)[i] += chunk[i];
      }
    })
{
  audio.pause(0);
}
