# moreDAW
A DAW for use on linux that follows unix standards while being compatible with modern VSTs.

# ROADMAP TO COMPLETION

### Phase 1 - Done
Read existing audio files (uncompressed wav), access audio drivers and play back audio.
This will be handled by C, and have no GUI at this point.

I will use portAudio to process the audio on the backend.

### Phase 2
Write GUI in gtk with a play button, record button and some sort of timer showing the length of the current audio 
being played.

TODO:
- Finish cursor moving along screen.
- Make waveform fill space of right box.
- Show more info on the track on the left hand side.

### Phase 3 - Done
Use audio drivers to record audio, then play it back / save to uncompressed wav

### Phase 4
Introduce multiple tracks with concurrent playback. This phase will involve a LOT of GUI work.

### Phase 5
Introduce standard mixing features like level control, gain, FX.

### Phase 6
Introduce configuaration from files in ~/.config



### QOL Todo
- Memory Leaks
- Handle sigterm