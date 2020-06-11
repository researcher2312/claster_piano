#ifndef MIDITRANSLATE_H
#define MIDITRANSLATE_H

uint8_t translatePitchToMIDI(int button, char octave);
void noteOffMIDI(int button, char octave, UART_HandleTypeDef *huart);
void noteOnMIDI(int button, char octave, UART_HandleTypeDef *huart);

#endif
