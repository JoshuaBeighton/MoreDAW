# openDAW
A DAW for use on linux that follows unix standards while being compatible with modern VSTs.



# ROADMAP TO COMPLETION

### Phase 1
Read existing audio files (uncompressed wav), access audio drivers and play back audio.
This will be handled by C, and have no GUI at this point.

I will use portAudio to process the audio on the backend.

### Phase 2
Write the initial GUI in Java using JavaFX. Interact with the C code from before using JNI.

### Phase 3
Use audio drivers to record audio, then play it back / save to uncompressed wav.

### Phase 4
Introduce multiple tracks with concurrent playback.

### Phase 5
Introduce standard mixing features like level control, gain, FX.

### Phase 6
Introduce configuaration from files in ~/.config