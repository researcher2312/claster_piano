#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "main.h"

void playSequence (uint8_t notes [64], uint32_t delayTime,UART_HandleTypeDef *huart);
void readSequence(uint8_t seqNotes[]);
void loopSequence(UART_HandleTypeDef *huart);

#endif
