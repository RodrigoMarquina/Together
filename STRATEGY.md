# STRATEGY

This document defines the development strategy for Together. It is intentionally abstract at the higher levels — the goal is to define *what* we are building and *in what order*, not *how* we build it. Implementation details will be documented in `/docs` as they are decided. This document should remain stable even as those details change.

A milestone is complete when we have a working prototype that demonstrates all features defined for that milestone. Not perfect. Not production-ready. Working and testable.

---

## The four milestones

### Milestone 1 — The Signal Editor
*The foundation. Everything else builds on this.*

A working audio editor where the signal is decomposed in real time via FFT running on the GPU, and the harmonic components are observable and editable directly. The target is a tool that does for signal engineering what Audacity does for basic audio editing — but where the primary view is not the waveform, it is the signal decomposed into its components across frequency, amplitude, and phase.

This milestone also includes real-time collaboration. Two people, two machines, editing the same signal simultaneously.

This is the main target. Milestones 2, 3, and 4 do not begin until this is complete and working.

**Submilestones are defined below.**

---

### Milestone 2 — The Composition Layer
*Building on top of the signal editor, not replacing it.*

Once the signal editor exists, the layer above it: arranging signals, sequencing them, building a song. This is where Together starts feeling like a DAW rather than a signal analysis tool. The signal editor is unchanged — this layer sits on top of it and uses it as its primitive.

Details are deliberately undefined until Milestone 1 is complete. What this layer needs to expose will become clear from using the signal editor.

---

### Milestone 3 — The UX and Accessibility Layer
*Making the tool usable by more than its builders.*

The DaVinci Resolve model applied to audio. Responsibility-divided workspaces. Layered complexity where a non-technical user has a clear path and a signal engineer has a clear path, both using the same engine underneath. Higher layers do not affect the flow of lower layers.

Two dimensions to explore:
- **Depth of technicality** — from composition at the top to harmonic editing at the bottom. The user chooses how deep they go.
- **Division by responsibility** — separate workspaces for separate concerns. Audio editing, composition, export. Workflows that do not collide.

This milestone comes third because the UX cannot be designed until the operations it exposes are known. Building the interface before the engine is building a facade.

Details are deliberately undefined until Milestone 2 is complete.

---

### Milestone 4 — Open Territory
*What becomes possible once the tool exists.*

This milestone is intentionally vague. It represents features that cannot be fully defined yet because they depend on what we learn building milestones 1 through 3. Candidates include AI integration, community-contributed DSP nodes, and capabilities that emerge from actually using the tool.

One concrete candidate: an AI layer with awareness of the session state that can navigate and operate the software through natural language. This is not planned in detail — it is noted here as a direction worth exploring once the foundation is solid.

This milestone may never formally close. It is the space where Together grows beyond what we originally imagined.

---

## Milestone 1 submilestones

These are ordered by dependency. Each one must be working before the next begins.

---

### 1.1 — Audio I/O
*Before anything else, audio in and out.*

Integrate with PipeWire and JACK. Establish a real-time audio callback. Audio comes in from a source, passes through the program, and comes out the other side without dropouts or latency artifacts.

No GPU yet. No signal processing yet. The sole purpose of this submilestone is to internalize the constraints of the real-time audio thread — no memory allocation, no locking, no blocking — and prove that the program can operate within them reliably.

**Done when:** audio passes through the program in real time without dropouts at standard buffer sizes.

---

### 1.2 — Vulkan compute foundation
*The GPU pipeline exists and can do work.*

Initialize the Vulkan compute pipeline. Write a compute shader that performs a simple mathematical operation on dummy data. Send data to the GPU, dispatch the shader, read results back. Nothing audio-related yet — the goal is to prove the architecture works and that the GPU/CPU communication is correct and stable.

This is the hardest infrastructure to get right and the most important to get right early. Everything above it depends on it.

**Done when:** a compute shader runs on real GPU hardware, processes dummy input, and returns correct output.

---

### 1.3 — Real-time FFT on GPU
*The first moment the signal is decomposed.*

Connect the audio pipeline from 1.1 to the Vulkan compute pipeline from 1.2. Audio buffers flow into the GPU, the FFT is computed, and frequency domain data comes back. The signal is now decomposed into its components in real time.

This is the architectural core of Together. The FFT does not need to be at full production quality yet — it needs to be correct, stable, and fast enough to run within the audio buffer window without dropouts.

**Done when:** live audio input is transformed to the frequency domain in real time on the GPU and the output is verifiably correct.

---

### 1.4 — Visualization
*The first moment you can see the signal decomposed.*

Render the frequency domain data in real time. The data computed by the FFT in 1.3 is already in GPU memory — the visualization reads from that data directly and renders it without copying it back to the CPU. A live spectrogram, harmonic content visible as the audio plays or is recorded.

The visualization is not decoration. It is the primary interface for everything that follows. Getting this right — resolution, frame rate, responsiveness — is as important as the DSP correctness.

**Done when:** a live spectrogram renders in real time from the GPU-computed FFT data, updating smoothly as audio input changes.

---

### 1.5 — Harmonic editing
*The first moment you can reach inside the signal.*

The user can select components in the visualization and modify them. Attenuate a harmonic. Remove a frequency range at a specific moment. Shift a component in phase. The inverse FFT converts the modified frequency domain data back to audio. The result is heard immediately.

This is the feature that defines Together. Everything before this is infrastructure. This is where the tool becomes real.

**Done when:** a modification made in the frequency domain is audible in the output audio and mathematically correct — meaning the inverse FFT of the unmodified components produces output identical to the input.

---

### 1.6 — Collaboration
*Two people, one signal.*

Implement CRDT-based session state synchronization. Two machines connect, share a session, and see the same signal decomposed in real time. An edit made by one person — a harmonic attenuated, a frequency removed — appears on the other person's visualization immediately and is reflected in their audio output.

Each machine computes its own audio independently using its own hardware. The network carries only the session state changes. The collaboration layer is decoupled from the audio pipeline entirely.

**Done when:** two people on two machines can edit the same signal simultaneously, with changes propagating in real time and audio output remaining consistent on both machines.

---

## Principles

**Build in order.** Each submilestone is a dependency of the next. Skipping ahead creates foundations built on untested assumptions.

**Working beats complete.** A submilestone is done when the defined behaviour works, not when every edge case is handled. Edge cases are addressed in iteration, not in the initial build.

**Document as you build.** Every architectural decision, every non-obvious implementation choice, every dead end goes into `/docs` at the time it is made. Documentation written after the fact is documentation that never gets written.

**The structure is stable, the implementation is not.** This document defines what we are building. How we build it will change. When an implementation decision turns out to be wrong, change the implementation — not the milestone structure.

---

## Future

AI integration, extended DSP node library, community contributions, and capabilities that emerge from using the tool are tracked as ideas in [`/docs/future.md`](./docs/future.md) and considered for Milestone 4 as the project matures.


1. jack_client_open() — register as a JACK client
2. jack_set_process_callback() — register your process callback
3. jack_port_register() — create an output port
4. jack_activate() — tell JACK you're ready
And inside the callback:
5. jack_port_get_buffer() — get the buffer to fill
6. Fill it with your sine wave formula
7. Return