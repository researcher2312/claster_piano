#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "main.h"

typedef struct PIANO_Sequence{
	uint8_t notes[64];
	uint8_t current_note;
	uint8_t previous_note;
	uint32_t last_note_time;
	uint32_t timeout;
	UART_HandleTypeDef *huart;
} PIANO_Sequence;

//void playSequence (uint8_t notes [64], uint32_t delayTime,UART_HandleTypeDef *huart);

//void loopSequence(UART_HandleTypeDef *huart);

void readSequence(PIANO_Sequence *sequence);
HAL_StatusTypeDef playNoteFromSequence(PIANO_Sequence *sequence);
void initSequence(PIANO_Sequence *sequence, uint32_t new_timeout, 	UART_HandleTypeDef *new_huart);
void resetSequence(PIANO_Sequence *sequence);

#endif
