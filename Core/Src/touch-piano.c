#include "touch-piano.h"
#include "MIDITranslate.h"

uint8_t runTouchStateMachine(PIANO_HandleTypeDef *hpiano){
	uint8_t pressed_key = 0;
	tsl_user_status_t status = TSL_USER_STATUS_BUSY;
	status = tsl_user_Exec();
	if(TSL_USER_STATUS_BUSY == status){
	// Nothing to do
		HAL_Delay(1);
	}
	else{
		for (uint8_t i = 0; i < 13; ++i){
			if (MyTKeysB[i].p_Data->Change == TSL_STATE_CHANGED){
				if (MyTKeysB[i].p_Data->StateId == TSL_STATEID_DETECT){
					hpiano->keys[i] = 1;
					noteOnMIDI(i+1, 'l', hpiano->huart);
					pressed_key = i;
				}
				else if(hpiano->keys[i] == 1){
					hpiano->keys[i] = 0;
					noteOffMIDI(i+1, 'l', hpiano->huart);
				}
			}
		}
	}
	return pressed_key;
}

void pianoInit(PIANO_HandleTypeDef *hpiano, UART_HandleTypeDef *new_huart){
	for (uint8_t i = 0; i < 13; ++i){
		hpiano->keys[i] = 0;
	}
	hpiano->huart = new_huart;
}


