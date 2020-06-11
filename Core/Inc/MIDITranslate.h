#ifndef MIDITRANSLATE_H
#define MIDITRANSLATE_H

_uint8_t translatePitchToMIDI(int button, char octave);
void noteOffMIDI(int button, char octave);
void noteOnMIDI(int button, char octave);

#endif
