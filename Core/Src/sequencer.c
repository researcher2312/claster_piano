#include "sequencer.h"
#include "MIDITranslate.h"

HAL_StatusTypeDef playNoteFromSequence(PIANO_Sequence *sequence, uint32_t timeout){
	HAL_StatusTypeDef state = HAL_OK;
	if (HAL_GetTick() > sequence->last_note_time + timeout){
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

void initSequence(PIANO_Sequence *sequence, UART_HandleTypeDef *new_huart){
	resetCounters(sequence);
	resetNotes(sequence);
	sequence->huart = new_huart;
}

void resetCounters(PIANO_Sequence *sequence){
	noteOffMIDI(sequence->notes[sequence->previous_note],'l',sequence->huart);
	sequence->previous_note = 0;
	sequence->current_note = 0;
	sequence->last_note_time = 0;
}
void resetNotes(PIANO_Sequence *sequence){
	for (uint8_t i = 0; i < 64; ++i){
		sequence->notes[i] = 0;
	}
}

void addToSequence(PIANO_Sequence *sequence, uint8_t note){
	sequence->notes[sequence->current_note] = note;
	sequence->current_note++;
}
