#include "live-music.hpp"
#include <shared/note.hpp>

auto main() -> int
{
  setBpm(120);

  auto master = Speaker{};
  auto reverb = Reverb{master};
  reverb.wet(0.5);

  // --- RHYTHM SECTION ---

  // A simple, steady kick drum.
  thread([&]() {
    auto kick =
      Sample{master, "samples/stargate-sample-pack/karoryfer/kicks/kick_Polmuz_mod_20_clean.wav", -8};
    kick.send(reverb, -20, 0);
    for (;;)
    {
      kick(0);
      delay(d4);
    }
  });

  // A simple hi-hat for rhythm.
  thread([&]() {
    auto hat = Sample(master, "samples/stargate-sample-pack/karoryfer/hihats/hihat_BRD_tight.wav", -10);
    for (;;)
    {
      hat(0);
      delay(d8);
      hat(-15);
      delay(d8);
    }
  });

  // A simple bassline following the root notes of the chords.
  thread([&]() {
    auto bass = createBass(master, -15);
    bass.send(reverb, -20, 0);
    for (;;)
    {
      bass.seq(G3, I, d4, I, d4, I, d4, I, d4);
      bass.seq(D3, I, d4, I, d4, I, d4, I, d4);
      bass.seq(E3, I, d4, I, d4, I, d4, I, d4);
      bass.seq(C3, I, d4, I, d4, I, d4, I, d4);
    }
  });

  // --- HARMONY SECTION ---

  // An optimistic chord progression: G - C - D - Em
  thread([&]() {
    auto pad = createPad(master, -5);
    pad.send(reverb, -8, 0);
    for (;;)
    {
      pad.chord(G4.setVel(-20).setDur(d2), I, III, V);
      delay(Bar);
      pad.chord(D4.setVel(-20).setDur(d2), I, III, V);
      delay(Bar);
      pad.chord(E4.setVel(-20).setDur(d2), I, iii, V);
      delay(Bar);
      pad.chord(C4.setVel(-20).setDur(d2), I, III, V);
      delay(Bar);
    }
  });

  // --- ALGORITHMIC MELODY SECTION ---

  // An algorithmic melody that follows the chord progression.
  thread([&]() {
    auto harp =
      Sample{master, "VCSL/Chordophones/Composite Chordophones/Concert Harp/KSHarp_C3_mf3.wav", 25};
    harp.send(reverb, -5, 0);
    for (;;)
    {
      // G Major chord notes: G, B, D
      Note g_major_notes[] = {G5, B5, D5};
      for (int i = 0; i < 8; ++i)
      { // Play 4 notes per chord
        harp.seq(g_major_notes[rnd() % 3], 0, d16);
        delay(d16);
      }

      // C Major chord notes: C, E, G
      Note c_major_notes[] = {C5, E5, G5};
      for (int i = 0; i < 8; ++i)
      {
        harp.seq(c_major_notes[rnd() % 3], 0, d16);
        delay(d16);
      }

      // D Major chord notes: D, Fs, A
      Note d_major_notes[] = {D5, Fs5, A5};
      for (int i = 0; i < 8; ++i)
      {
        harp.seq(d_major_notes[rnd() % 3], 0, d16);
        delay(d16);
      }

      // E Minor chord notes: E, G, B
      Note e_minor_notes[] = {E5, G5, B5};
      for (int i = 0; i < 8; ++i)
      {
        harp.seq(e_minor_notes[rnd() % 3], 0, d16);
        delay(d16);
      }
    }
  });

  runForever();
}
