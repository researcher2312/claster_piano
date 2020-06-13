#ifndef INC_TOUCH_PIANO_H_
#define INC_TOUCH_PIANO_H_

#include "touchsensing.h"

typedef struct PIANO_HandleTypeDef{
	uint8_t keys[13];
	UART_HandleTypeDef *huart;
} PIANO_HandleTypeDef;

void pianoInit(PIANO_HandleTypeDef *hpiano, UART_HandleTypeDef *new_huart);
uint8_t runTouchStateMachine(PIANO_HandleTypeDef *hpiano);





#endif /* INC_TOUCH_PIANO_H_ */
