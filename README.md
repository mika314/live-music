# Live Music System

A live-coding C++ audio framework for real-time performance

A **real-time** live music performance system in C++, featuring a client for music generation and a server for audio processing. Designed for live-coding and continuous playback, enabling rapid prototyping of musical ideas.

---

## Table of Contents

* [Features](#features)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Usage](#usage)
* [Composing Music](#composing-music)
* [Full Composition Example](#full-composition-example)
* [Development Workflow](#development-workflow)
* [API Reference](#api-reference)
* [License](#license)
* [Acknowledgements](#acknowledgements)

---

## Features

* Real-time audio synthesis and sample playback
* DSL-style C++ API for notes, chords, and effects
* Built-in reverb and spatial processing
* Live-coding friendly (auto-restart client)
* Cross-platform with minimal dependencies

---

## Prerequisites

* **C++20** compiler (Clang)
* **coddle** build tool
* **libuv** (asynchronous I/O)
* **SDL2** (audio and input)
* **Make**
* **Git**

---

## Installation

1. **Install coddle**

   ```bash
   git clone https://github.com/coddle-cpp/coddle.git
   cd coddle
   ./build.sh
   sudo ./deploy.sh
   cd ..
   ```
2. **Clone live-music**

   ```bash
   git clone https://github.com/mika314/live-music.git
   cd live-music
   ```
3. **Build project**

   ```bash
   make
   ```

---

## Usage

The system consists of two components:

* **Server**: Performs audio processing, handles sample playback and effects, and outputs to your audio device.
* **Client**: Generates musical events via the C++ DSL and sends them to the server in real time.

To start the system:

* **Run server**:

  ```bash
  cd server && ./server
  ```

* **Run client** (auto-rebuild on changes):

  ```bash
  make client
  ```

  This command recompiles the client and restarts it automatically whenever you modify your music code.

Alternatively, manually launch the client:

```bash
cd client && ./client
```

---

## Composing Music

All composition logic resides in `client/main.cpp`, using a thread-based, fluent API.

### 1. Setting Tempo (BPM)

```cpp
setBpm(120); // Global tempo
```

> **Recommendation:** Set once at startup to maintain sync during live performance.

### 2. Instruments

#### Built-in Synths

```cpp
auto bass  = createBass(master);
bass(root{C2}, d4);

auto pluck = createPluck(master);
pluck(root{E4}, d8, Articulation::staccato);

auto pad   = createPad(master);
pad(root{C3}, d2, Articulation::legato);
```

#### Custom Synth

```cpp
Synth mySynth(master, {
  .gain     = -6.0,
  .pan      = 0.0,
  .oscType  = OscType::saw,
  .envelope = Envelope{ .attack=0.01, .decay=0.1, .sustain=0.8, .release=0.2 }
});
mySynth(root{A4}, d4);
```

#### Samples

```cpp
auto kick = Sample(master, "samples/kick.wav", -8);
kick(d4, I);
```

#### Sequencing Samples

```cpp
thread([&]() {
  auto hat   = Sample(master, "samples/hat.wav");
  auto snare = Sample(master, "samples/snare.wav");
  auto kick  = Sample(master, "samples/kick.wav");
  for (;;) {
    kick(d4, I);
    snare(d4, I);
    hat(d4, I);
    hat(d4, I);
  }
});
```

#### Melodic Sample Example

```cpp
// Grand Piano sustain sample from VCSL
auto piano = Sample(
  master,
  "VCSL/Chordophones/Zithers/Grand Piano, Kawai/Sustains/GPiano_sus_C4_v2_rr1_Player.wav",
  -5
);
thread([&]() {
  for (;;) {
    piano(root{C4}, d4, ChN(C4, E4, G4), I);
    piano(root{A3}, d4, ChN(A3, C4, E4), I);
    piano(root{F3}, d4, ChN(F3, A3, C4), I);
    piano(root{G3}, d4, ChN(G3, B3, D4), I);
  }
});
```

### 3. Expression Tools

* **Chords:** `ChI(I, III, V)` → build by intervals; `ChN(C4, E4, G4)` → explicit notes
* **Rests:** `rest` (insert rest of current default duration)
* **Layering & Polyrhythms:** multiple `thread` calls with `delay`

### 4. Reverb

```cpp
auto reverb = Reverb(master);
reverb.wet(0.5);
// Example: send a synth to reverb
auto bass = createBass(master);
bass.send(reverb, -20);
```

### 5. Sample Library

* `server/samples/` (drum hits, FX)
* `server/VCSL/`  (extended pack
  and list all samples: `cat samples-list.txt`)

---

## Full Composition Example

```cpp
#include "live-music.hpp"

auto main() -> int
{
  setBpm(120);
  auto master = Speaker{};
  auto reverb = Reverb{master};
  reverb.wet(0.9);

  // Drums
  thread([&]() {
    auto kick = Sample(
      master, "samples/stargate-sample-pack/fugue-state-audio/drums/kicks/x0xproc1-kick.wav", -5, -0.2);
    kick.send(reverb, -14);
    for (;;)
      kick(d4, I);
  });

  thread([&]() {
    auto snare =
      Sample(master, "samples/stargate-sample-pack/microlag/One-Shots/Drums/Snare1.wav", -2, 0.2);
    snare.send(reverb, -14);
    for (;;)
      snare(d4, rest, I, rest, I);
  });

  thread([&]() {
    auto hat =
      Sample(master, "samples/stargate-sample-pack/microlag/One-Shots/Drums/Hihat1.wav", -5, 0.6);
    hat.send(reverb, -14);
    for (;;)
    {
      if (rnd() % 3 != 0)
        hat(C4.setVel(-20 - (rnd() % 20)), d8, I);
      else
        hat(C4.setVel(-20 - (rnd() % 20)), d16, I, I);
    }
  });

  // Bass
  thread([&]() {
    auto bass = createBass(master, -10);
    bass.send(reverb, -5);
    for (;;)
    {
      bass(d2, Articulation::staccato, E3, G2, F2, G2);
    }
  });

  // Synth Melody
  thread([&]() {
    auto pluck = createPluck(master, -5, -0.5);
    pluck.send(reverb, -5);
    for (;;)
    {
      pluck(root{E4}, d4, rest, ChI(I, III, V));
      pluck(root{G4}, d4, I, ChI(I, III, V));
      pluck(root{F4}, d4, rest, ChI(I, III, V));
      pluck(root{G4}, d4, I, ChI(I, III, V));
    }
  });

  // Pads
  thread([&]() {
    auto pad = createPad(master, -15, 0.5);
    pad.send(reverb, -2);
    for (;;)
    {
      pad(root{E3}, d8, Articulation::legato, I, III, V, O);
      pad(root{G3}, d8, Articulation::legato, I, III, V, O);
      pad(root{F3}, d8, Articulation::legato, I, III, V, O);
      pad(root{G3}, d8, Articulation::legato, I, III, V, O);
    }
  });

  // Melodic Sample
  thread([&]() {
    auto piano =
      Sample(master,
             "VCSL/Chordophones/Zithers/Grand Piano, Kawai/Sustains/GPiano_sus_C4_v2_rr1_Player.wav",
             -13,
             0.7);
    piano.send(reverb, -13);
    for (;;)
    {
      piano(root{E4}, d8, ChI(I, III, V), rest);
      piano(root{G3}, d8, ChI(I, III, V), I);
      piano(root{F3}, d8, ChI(I, III, V), rest);
      piano(root{G3}, d8, ChI(I, III, V), I);
    }
  });

  runForever();
}
```

## Development Workflow

* **Client-only changes:** `make client`
* **Full rebuild:** `make && cd server && ./server`

---

## API Reference

* **Core Functions:** `setBpm`, `thread`, `delay`, `runForever`
* **Synth:** `Synth`, `SynthParams`, `operator()`
* **Sample:** `Sample`, `operator()`
* **Reverb:** `Reverb`, `wet`
* **Speaker:** `Speaker`
* **Note:** `Note`, `setDur`, `setVel`
* **Envelope:** fields
* **OscType:** waveforms
* **Keep alive:** `runForever()`

---

## Acknowledgements

* coddle - build tool
* Sample packs from **VCSL** and **karoryfer**
