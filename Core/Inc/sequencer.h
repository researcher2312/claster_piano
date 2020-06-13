#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "main.h"

typedef struct PIANO_Sequence{
	uint8_t notes[64];
	uint8_t current_note;
	uint8_t previous_note;
	uint32_t last_note_time;
	UART_HandleTypeDef *huart;
} PIANO_Sequence;

void readSequence(PIANO_Sequence *sequence);
void addToSequence(PIANO_Sequence *sequence, uint8_t note);
HAL_StatusTypeDef playNoteFromSequence(PIANO_Sequence *sequence, uint32_t timeout);
void initSequence(PIANO_Sequence *sequence,	UART_HandleTypeDef *new_huart);
void resetNotes(PIANO_Sequence *sequence);
void resetCounters(PIANO_Sequence *sequence);

#endif
