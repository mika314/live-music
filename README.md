# Live Music System

This project is a live music performance system, consisting of a client and a server component. The client generates and plays music, while the server handles audio processing and related tasks.

## Building the Project

This project uses `coddle` as its build system. `coddle` is a C++ build tool that simplifies the compilation process. To build the client and server, navigate to the project root directory and run the following command:

```bash
make
```

This will build both the `client` and `server` executables in their respective directories.

## Running the Project

To run the system, navigate to the `server` directory and execute the `server` executable:

```bash
cd server
./server
```

The server automatically starts the client. The `Makefile` is configured to kill any currently running `client-running` processes when `make client` is executed, facilitating continuous operation and live-coding scenarios for the client.

## Project Structure

- `client/`: Contains the source code for the music generation client.
- `server/`: Contains the source code for the audio processing server.
- `shared/`: Contains shared header files and source code used by both client and server.
- `coddle-repo/`: Contains `coddle` related configuration.
- `samples/`: Contains audio samples used by the system.

## Composing Music

To compose music with this system, you will primarily modify the `client/main.cpp` file. This C++ program allows you to define musical parts using threads and various musical objects. After making changes, you will need to recompile and run the system to hear your composition. Refer to the "Recompile and Run" section for detailed instructions.

### 1. Setting the Tempo (BPM)

You can set the global tempo of your composition using `setBpm()` at the beginning of `main()`:

```cpp
setBpm(120); // Sets the tempo to 120 beats per minute
```

### 2. Adding Samples (Drums, Percussion, etc.)

Use the `Sample` object to load and play WAV files. You can specify the path to your sample and its initial volume.

```cpp
// Example: A kick drum
thread([&]() {
  auto kick =
    Sample{master, "samples/stargate-sample-pack/karoryfer/kicks/kick_Polmuz_mod_20_clean.wav", -8};
  for (;;)
  {
    kick(0); // Play the kick drum at 0dB (loudest). Negative values make it quieter.
    delay(d4); // Wait for a quarter note
  }
});
```

- `Sample{master, "path/to/sample.wav", volume}`: Creates a sample instrument. `master` is the main audio output, `"path/to/sample.wav"` is the sound file, and `volume` is in decibels (e.g., -8).
- `kick(0)`: Plays the sample. The argument is a gain value in decibels, where `0` is the loudest and negative values make it quieter.
- `delay(d4)`: Pauses the thread for the duration of a quarter note. `d4`, `d8`, `d16`, `Bar` are predefined durations for quarter, eighth, sixteenth notes, and a full bar, respectively.

### 3. Creating Synthesizer Instruments (Basslines, Melodies, Chords, Pads)

Functions like `createBass()`, `createPluck()`, and `createPad()` create various synthesizer instruments. These instruments can then use the `seq()` method to play sequences of notes or the `chord()` method to play chords.

```cpp
// Example: A simple bassline
thread([&]() {
  auto bass = createBass(master, -20);
  for (;;)
  {
    bass.seq(G + O2, I, d4, I, d4, I, d4, I, d4); // Play G note in Octave 2 for a quarter note
    bass.seq(D + O2, I, d4, I, d4, I, d4, I, d4); // Play D note in Octave 2 for a quarter note
    // ... and so on for other notes
  }
});
```

```cpp
// Example: An optimistic chord progression
thread([&]() {
  auto pad = createPad(master, -5);
  for (;;)
  {
    pad.chord(G.setVel(-20).setDur(d2) + O3, I, III, V); // Play G major chord in Octave 3
    delay(Bar);
    pad.chord(D.setVel(-20).setDur(d2) + O3, I, III, V); // Play D major chord in Octave 3
    // ... and so on for other chords
  }
});
```

- `createBass(master, volume)`: Creates a bass synthesizer.
- `createPluck(master, volume)`: Creates a plucked synth sound.
- `createPad(master, volume)`: Creates a pad synthesizer.
- `note + Octave`: Defines a note (e.g., `G`, `C`, `D`, `E`, `Fs` for F-sharp) and its octave (e.g., `O2`, `O3`, `O5`).
- `I, III, V, iii`: These represent intervals relative to the base note, used in `chord()` or `seq()`. Capital Roman numerals (e.g., `I`, `III`, `V`) denote major intervals, while lowercase Roman numerals (e.g., `iii`) denote minor intervals.
- `synth(Note)`: Plays a single note on the synthesizer.
  ```cpp
  // Example: Play a single note
  thread([&]() {
    auto mySynth = createPluck(master, -10);
    for (;;) {
      mySynth(C + O4); // Play C in Octave 4
      delay(d4);
    }
  });
  ```
- `synth(Envelope)`: Applies an ADSR (Attack, Decay, Sustain, Release) envelope to the synthesizer's output. The `Envelope` struct defines these stages in seconds for attack, decay, and release, and in dB for sustain.
  ```cpp
  // Example: Apply an envelope to a synth
  thread([&]() {
    auto mySynth = createPad(master, -10);
    // Define an envelope: attack (0.1s), decay (0.5s), sustain (-20dB), release (0.1s)
    Envelope customEnvelope = Envelope{0.1, 0.5, -20, 0.1};
    for (;;) {
      mySynth(customEnvelope);
      mySynth(C + O4);
      delay(Bar);
    }
  });
  ```
- `Envelope{attack, decay, sustain, release}`: Creates an envelope with specified attack (seconds), decay (seconds), sustain (dB), and release (seconds) values.
- `synth.maj(Note)`: Plays a major chord based on the given root note.
  ```cpp
  // Example: Play a major chord
  thread([&]() {
    auto mySynth = createPad(master, -10);
    for (;;) {
      mySynth.maj(C + O3); // Play C major chord in Octave 3
      delay(Bar);
    }
  });
  ```
- `synth.min(Note)`: Plays a minor chord based on the given root note.
  ```cpp
  // Example: Play a minor chord
  thread([&]() {
    auto mySynth = createPad(master, -10);
    for (;;) {
      mySynth.min(A + O3); // Play A minor chord in Octave 3
      delay(Bar);
    }
  });
  ```
- `synth.seq(Note key, int ofset, double dur, ...)`: Plays a sequence of notes. `key` is the base note, `ofset` is the interval from the key, and `dur` is the duration.
- `synth.chord(Note key, int ofset, ...)`: Plays a chord based on the root note and specified intervals.
- `note.setVel(velocity).setDur(duration) + Octave`: Sets the velocity (volume) and duration for a note.

### 5. Using Reverb

You can add reverb to your instruments to create space and depth.

```cpp
auto reverb = Reverb{master};
reverb.wet(0.5); // Adjust the wetness (amount of reverb)

// Send an instrument's output to the reverb
kick.send(reverb, -20, 0); // kick instrument sends to reverb with specific gain and pan
bass.send(reverb, -20, 0);
pad.send(reverb, -8, 0);
pluck.send(reverb, -5, 0);
```

- `Reverb{master}`: Initializes a reverb effect connected to the master output.
- `reverb.wet(amount)`: Sets the wetness level of the reverb (0.0 to 1.0).
- `instrument.send(reverb, gain, pan)`: Sends the output of an instrument to the `reverb` effect. `gain` is the volume sent to the reverb, and `pan` is the stereo position.

### 6. Algorithmic Composition

The system supports algorithmic generation of melodies or other musical elements. The `main.cpp` example includes a section for an algorithmic melody:

```cpp
// Example: Algorithmic melody
thread([&]() {
  auto pluck = createPluck(master, -8);
  for (;;)
  {
    // G Major chord notes: G, B, D
    Note g_major_notes[] = {G, B, D};
    for (int i = 0; i < 4; ++i)
    {
      pluck.seq(g_major_notes[rnd() % 3] + O5, 0, d16); // Randomly pick a note from the chord
      delay(d16);
    }
    // ... similar logic for other chords
  }
});
```

- `rnd() % N`: Generates a random number between 0 and N-1, useful for creating variations.

### 7. Recompile and Run

After making changes to any C++ source files:

1.  **Save the file.**
2.  **Recompile and Restart Client (for client-side changes):** If you've only modified client-side code (e.g., `client/main.cpp`), go to the project root directory and run `make client`. This will recompile the client, kill any running client processes, and then restart the new client.
    ```bash
    make client
    ```
3.  **Recompile All and Restart Server (for server or shared changes):** If you've modified server-side code or shared components, go to the project root directory and run `make` to recompile both the client and server. Then, restart the server to apply the changes.
    ```bash
    make
    cd server
    ./server
    ```

Experiment with different notes, durations, instruments, and effects to create your own unique compositions!

### Keeping the Music Playing

The `runForever()` function, called at the end of `client/main.cpp`, ensures that the music continues to play indefinitely. It keeps the client application running in a loop, processing and outputting audio based on the defined musical threads.