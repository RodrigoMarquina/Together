# Together

> Real-time collaborative audio production and signal engineering, built from the ground up on Linux.

Together is an open source DAW built around a single idea: that the computational limits of existing tools are not just a performance problem — they are a design constraint that determines what audio engineering can even mean. By dividing the workload between the CPU and GPU using Vulkan, Together reaches a level of signal control that current tools cannot expose.

You record. You see the signal decomposed into its actual components in real time. You edit those components directly — not the whole signal, but its frequency content, its amplitude envelope, its phase relationships. And you do all of it together with another person, from anywhere, simultaneously.

---

## Why this exists

Two people. One wanted to add a bassline. The other was in a different city. The solution was a Discord call, a shared screen, and one person describing to the other what knobs to turn.

We decided to build what we wished existed.

The full story, the technical thesis, and the complete list of what this makes possible is in [PURPOSE.md](./PURPOSE.md).

---

## What makes it different

**Every major DAW uses only the CPU.** That architecture was designed before GPU compute existed and has never been fundamentally reconsidered. The result is that expensive operations get hidden behind abstractions, and certain operations become impossible entirely.

Together uses Vulkan to run signal processing on the GPU and session management on the CPU in parallel. This is not an optimization of the existing model — it enables a different model entirely.

**Current DAWs give you the signal as a primitive.** You can cut it, volume it, apply effects to it. You cannot reach inside it. Together applies a real-time FFT to decompose the signal into its components — frequency, amplitude, phase — and makes those components individually addressable. You are not turning a knob that affects everything. You are editing specific harmonic content at specific moments.

**Collaboration is first-class, not an afterthought.** The session state is synchronized between machines using a CRDT-based model. Each machine computes its own audio independently using its own hardware. The network carries only the state changes — small, fast, decoupled from the audio pipeline entirely.

---

## Status

Early development. The project is currently being built as a learning exercise in C++, GPU compute, real-time audio programming, and signal processing. Architecture and milestones are defined in [STRATEGY.md](./STRATEGY.md).

This is not ready for production use. It is ready for contributors who want to build something genuinely new.

---

## Technical foundation

| Concern | Approach |
|---|---|
| Language | C++23 |
| GPU compute | Vulkan compute shaders (GLSL) |
| Audio backend | PipeWire / JACK |
| Signal analysis | Real-time FFT, time-frequency decomposition |
| Collaboration | CRDT-based session state sync |
| Platform | Linux-native |
| Build system | CMake |

---

## Documentation

Technical documentation lives in [`/docs`](./docs) and grows alongside the codebase. Architecture decisions, signal processing concepts, and implementation notes are documented as they are built — not after.

---

## Contributing

Together is open source because whatever this level of control makes possible should belong to anyone who wants to build on it. If you are interested in real-time audio, GPU compute, or collaborative systems and want to contribute, read [STRATEGY.md](./STRATEGY.md) for current priorities.

---

## License

GPL-3.0
