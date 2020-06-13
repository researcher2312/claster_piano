#include "sequencer.h"
#include "MIDITranslate.h"


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

void readSequence(uint8_t seqNotes[]){
	for(uint8_t i=0;i<64;++i)
		seqNotes[i]=0;
	//jesli wciskany jest przycisk to dopisuje jego numer do tablicy
	//....
	//Na razie do testow:
	seqNotes[0]=3;
	seqNotes[1]=5;
	seqNotes[2]=7;
	seqNotes[3]=6;
	seqNotes[4]=3;
	seqNotes[5]=2;
	seqNotes[6]=7;
	seqNotes[7]=5;
	seqNotes[8]=9;
	seqNotes[9]=12;
	seqNotes[10]=3;
	seqNotes[11]=5;
	seqNotes[12]=3;
	seqNotes[13]=7;
}

void loopSequence(UART_HandleTypeDef *huart){
	int seqNotes[64];
	readSequence(seqNotes);
	//while przycisk play sequence jest włączony (czyli zawsze dogra sekwencję do końca
	while(1)
		//odczytaj czas
		playSequence(seqNotes,500,huart);
	//}
}
