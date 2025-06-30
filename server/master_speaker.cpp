#include "master_speaker.hpp"
#include "s_source.hpp"
#include <log/log.hpp>

MasterSpeaker::MasterSpeaker()
  : want{.freq = SampleRate, .format = AUDIO_F32LSB, .channels = ChN, .samples = 1024},
    audio(nullptr, false, &want, &have, 0, [this](Uint8 *stream, int len) {
      lock();
      memset(stream, 0, len);
      const auto samples = len / sizeof(float) / ChN;
      for (auto &s : sources)
      {
        const auto chunk = s->pull(samples);
        for (auto i = 0U; i < samples * ChN; ++i)
          reinterpret_cast<float *>(stream)[i] += chunk[i];
      }
      samplesProcessed += samples;

      for (auto it = std::begin(orphanage); it != std::end(orphanage);)
        if (!it->get()->isBusy())
          it = orphanage.erase(it);
        else
          ++it;

      unlock();
    })
{
  audio.pause(0);
}

auto MasterSpeaker::lock() const -> void
{
  const_cast<MasterSpeaker *>(this)->audio.lock();
}

auto MasterSpeaker::unlock() const -> void
{
  const_cast<MasterSpeaker *>(this)->audio.unlock();
}

auto MasterSpeaker::getSamplesProcessed() const -> int
{
  auto r = 0;
  lock();
  r = samplesProcessed;
  unlock();
  return r;
}
