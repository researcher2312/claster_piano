#include "sequencer.h"
#include "MIDITranslate.h"

HAL_StatusTypeDef playNoteFromSequence(PIANO_Sequence *sequence){
	HAL_StatusTypeDef state = HAL_OK;
	if (HAL_GetTick() > sequence->last_note_time + sequence->timeout){
		sequence->last_note_time = HAL_GetTick();
		noteOffMIDI(sequence->notes[sequence->previous_note],'l',sequence->huart);
		noteOnMIDI(sequence->notes[sequence->current_note],'l',sequence->huart);
		sequence->previous_note = sequence->current_note;
		sequence->current_note++;
		if (sequence->notes[sequence->current_note] == 0 || sequence->current_note == 64){
			sequence->current_note = 0;
			state = HAL_TIMEOUT;
		}
	}
	return state;
}

void initSequence(PIANO_Sequence *sequence, uint32_t new_timeout, 	UART_HandleTypeDef *new_huart){
	sequence->current_note = 0;
	sequence->previous_note = 0;
	sequence->last_note_time = 0;
	sequence->timeout = new_timeout;
	for (uint8_t i = 0; i < 64; ++i){
		sequence->notes[i] = 0;
	}
	sequence->huart = new_huart;
}

void resetSequence(PIANO_Sequence *sequence){
	noteOffMIDI(sequence->notes[sequence->previous_note],'l',sequence->huart);
	sequence->previous_note = 0;
	sequence->current_note = 0;
	sequence->last_note_time = 0;
}

void playSequence (uint8_t notes [64], uint32_t delayTime, UART_HandleTypeDef *huart){
	for(uint8_t i=0;i<64;++i){
		if(notes[i]==0)
			break;
		noteOnMIDI(notes[i],'l',huart);
		HAL_Delay(delayTime);
		noteOffMIDI(notes[i],'l',huart);
		//noteOn od i
		//delay o czas
		//noteOff od i

	}
}

void readSequence(PIANO_Sequence *sequence){
//	for(uint8_t i=0;i<64;++i)
//		seqNotes[i]=0;
	//jesli wciskany jest przycisk to dopisuje jego numer do tablicy
	//....
	//Na razie do testow:

	sequence->notes[0]=3;
	sequence->notes[1]=5;
	sequence->notes[2]=7;
	sequence->notes[3]=6;
	sequence->notes[4]=3;
	sequence->notes[5]=2;
	sequence->notes[6]=7;
	sequence->notes[7]=5;
	sequence->notes[8]=9;
	sequence->notes[9]=12;
	sequence->notes[10]=3;
	sequence->notes[11]=5;
	sequence->notes[12]=3;
	sequence->notes[13]=7;
}

void loopSequence(UART_HandleTypeDef *huart){
	int seqNotes[64];
	readSequence(seqNotes);
	//while przycisk play sequence jest włączony (czyli zawsze dogra sekwencję do końca
	while(1){
		//odczytaj czas
		playSequence(seqNotes,500,huart);
		if (HAL_GPIO_ReadPin(PLAY_GPIO_Port, PLAY_Pin) == GPIO_PIN_RESET){
			break;
		}
	}


	//}
}
