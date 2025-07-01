#include "load-audio.hpp"
#define DR_FLAC_IMPLEMENTATION
#include "../extras/dr_flac.h"
#define DR_MP3_IMPLEMENTATION
#include "../extras/dr_mp3.h"
#define DR_WAV_IMPLEMENTATION
#include "../extras/dr_wav.h"

#define MINIAUDIO_IMPLEMENTATION
#include "../miniaudio.h"

auto loadAudio(std::filesystem::path) -> std::vector<float> {}
