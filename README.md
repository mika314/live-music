# Live Music System

This project is a live music performance system consisting of a client and a server component. The client generates and plays music, while the server handles audio processing and related tasks.

## Building the Project

This project uses `coddle` as its build system. `coddle` is a C++ build tool that simplifies the compilation process. To build the client and server, navigate to the project root directory and run:

```bash
make
```

This will build both the `client` and `server` executables in their respective directories.

## Running the Project

To run the system, navigate to the `server` directory and execute the `server` binary:

```bash
cd server
./server
```

The server automatically starts the client. The `Makefile` is configured to kill any currently running `client-running` processes when `make client` is executed, facilitating continuous operation and live-coding scenarios.

## Project Structure

* `client/`: Source code for the music generation client.
* `server/`: Source code for the audio processing server.
* `shared/`: Shared header files and source code used by both client and server.
* `coddle-repo/`: Contains `coddle` configuration.
* `server/samples/` and `server/VCSL/`: Collections of audio samples used by the system.

## Composing Music

You will primarily modify `client/main.cpp` to compose music. This C++ file allows you to define musical parts using threads and musical objects. After making changes, recompile and run the system to hear your composition. Refer to the **Recompile and Run** section for instructions.

### 1. Setting the Tempo (BPM)

Set the global tempo using `setBpm()` at the beginning of `main()`:

```cpp
setBpm(120); // Sets the tempo to 120 beats per minute
```

### 2. Instruments

Both `Synth` and `Sample` classes share the same interface for playing music.

#### Creating Instruments

*   **Synths:** Use `createBass()`, `createPluck()`, and `createPad()` to create synthesizers with predefined parameters. You can also create a custom `Synth` object.
*   **Samples:** Create a `Sample` object by providing the path to a WAV file.

```cpp
// Example: Creating a pluck synth
auto synth = createPluck(master);

// Example: Creating a sample instrument
auto kick = Sample{master, "samples/stargate-sample-pack/karoryfer/kicks/kick_Polmuz_mod_20_clean.wav", -8};
```

#### Playing Notes and Chords

The composition syntax is designed to be expressive and flexible. You can play sequences of notes and chords by passing a series of events to the instrument object.

```cpp
// Example: A simple melody
thread([&]() {
  auto synth = createPluck(master);
  synth.send(reverb, -5);
  for (;;)
  {
    // clang-format off
     synth(root{C4},        // Set the root note to C4
           Articulation::legato, // Set the articulation to legato
           d4,I,  I,  V,  V,  VI, VI, d2,  V, // Play a sequence of notes
           d4,IV, IV, III,III,II, II, d2,  I,
           d4,V,  V,  IV, IV, III,III,d2, II,
           d4,V,  V,  IV, IV, III,III,d2, II);
    // clang-format on
  }
});
```

Key Concepts:

*   `root{note}`: Sets the root note for the following sequence.
*   `Articulation::{legato, staccato, unchanged}`: Sets the articulation for the following notes.
*   `d4`, `d8`, `d16`, `Bar`, etc.: Predefined durations.
*   `I`, `ii`, `III`, `iv`, `V`, etc.: Intervals relative to the root note.
*   `ChI(...)`: Creates a chord with intervals relative to the root note.
*   `ChN(...)`: Creates a chord with specific notes.
*   `rest`: Represents a rest.

### 3. Using Reverb

Add reverb to instruments for spatial effects:

```cpp
auto reverb = Reverb{master};
reverb.wet(0.5);

instrument.send(reverb, -20, 0);
```

### 4. Samples

A full list of available samples can be found in the `samples-list.txt` file.

### 5. Recompile and Run

After changes:

#### For client-only changes:

```bash
make client
```

This kills the running client and starts a new one.

#### For server or shared code changes:

```bash
make
cd server
./server
```

### 6. Keeping the Music Playing

The `runForever()` function at the end of `main.cpp` ensures continuous playback by keeping the application running and processing threads.
