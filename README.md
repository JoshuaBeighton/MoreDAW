# moreDAW
A DAW for use on linux that follows unix standards while being compatible with modern VSTs.

# ROADMAP TO COMPLETION

### Phase 1
Read existing audio files (uncompressed wav), access audio drivers and play back audio.
This will be handled by C, and have no GUI at this point.

I will use portAudio to process the audio on the backend.

### Phase 2
Write GUI in gtk with a play button, record button and some sort of timer showing the length of the current audio 
being played.

### Phase 3
Use audio drivers to record audio, then play it back / save to uncompressed wav

### Phase 4
Introduce multiple tracks with concurrent playback.

### Phase 5
Introduce standard mixing features like level control, gain, FX.

### Phase 6
Introduce configuaration from files in ~/.config





# TODO Before Phase 4
- Improve code quality / commenting
- Figure out how to record variable length audio.
- Stop recording button
- Better state management.
- Make timer update in realtime as audio plays
- Generate waveform images and show cursor moving through the image.
- Allow playback from a time within the audio.