#include "main.h"
#include "MIDITranslate.h"
/*
Zrodla w ktorych opisane sa kody, z ktorych korzysta standard MIDI:
MIDI codes:
https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message
*/

/*
Funkcja sluzaca do tlumaczenia identyfikatora przycisku na kod wysokości dźwięku
w standardzie MIDI
Function to translate key identifier to MIDI pitch info
*/

uint8_t MIDIcodes [5][13] = {
{0b0011000,0b0011001,0b0011010,0b0011011,0b0011100,0b0011101,0b0011110,0b0011111,0b0100000,0b0100001,0b0100010,0b0100011,0b0100100},
{0b0100100,0b0100101,0b0100110,0b0100111,0b0101000,0b0101001,0b0101010,0b0101011,0b0101100,0b0101101,0b0101110,0b0101111,0b0110000},
{0b0110000,0b0110001,0b0110010,0b0110011,0b0110100,0b0110101,0b0110110,0b0110111,0b0111000,0b0111001,0b0111010,0b0111011,0b0111100},
{0b0111101,0b0111110,0b0111111,0b1000000,0b1000001,0b1000010,0b1000011,0b1000100,0b0000101,0b1000110,0b1000111,0b1001000,0b1001001},
{0b1001001,0b1001010,0b1001011,0b1001100,0b1001101,0b1001110,0b1001111,0b1010000,0b0010001,0b1010010,0b1010011,0b1010100,0b1010101}
};//E,F,F#,G,G#,A,A#,B,C,C#,D,D#,E  	-4
  //C,C#,D,D#,E,F,F#,G,G#,A,A#,B,C
uint8_t translatePitchToMIDI(int button, char octave){
	uint8_t pitch;
	//octave: s - standard; h - high; l - low, b - super low, g - super high	

	if(octave=='b'){
		pitch=MIDIcodes[0][button];
	}
	else if(octave=='l'){
		pitch=MIDIcodes[1][button];
		if(button==14){
			pitch=0b0100000;
		}
	}
	else if(octave=='s'){
		pitch=MIDIcodes[2][button];
	}
	else if(octave=='h'){
		pitch=MIDIcodes[3][button];
	}	
	else{//tutaj jest przypadek octave == 'p'
		pitch=MIDIcodes[4][button];
	}		

	return pitch;
}

/*
	Funkcja sluzaca do tlumaczenia i nadania przez interfejs USB
	informacji o zwolnieniu klawisza
	Function to translate and send via USB info when button is released
	
	Format wiadomosci:
        start_bit zwolnienie_klawisza kanal_1 stop_bit start_bit wysokosc_dzwieku stop_bit start_bit dynamika_dzwieku stop_bit 

	Message format:
	start_bit command_note_off channel_1 stop_bit start_bit note_pitch stop_bit start_bit note_velocity stop_bit 

	//11000000 001nnnnn nnn01011 111110 00
*/
void noteOffMIDI(int button, char octave, UART_HandleTypeDef *huart){
	uint8_t MessageLength=0;
	uint8_t pitch=translatePitchToMIDI(button, octave);
	uint8_t DataToSend[3] = {
	0b10000000,
	pitch,
	0b00000000
	};
	MessageLength = sizeof(DataToSend);
	/*TUTAJ WYSŁANIE WIADOMOŚCI PRZEZ USB*/
	for (int i=0;i<3;++i){
		HAL_UART_Transmit(huart, &DataToSend[i],MessageLength,1000);
	}
}
/*
        Funkcja sluzaca do tlumaczenia i nadania przez interfejs USB 
        informacji o wcisnieciu klawisza
	Function to translate and send via USB info when button is touched

	Format wiadomosci:
        start_bit wcisniecie_klawisza kanal_1 stop_bit start_bit wysokosc_dzwieku stop_bit start_bit dynamika_dzwieku stop_bit 

        Message format:
        start_bit command_note_on channel_1 stop_bit start_bit note_pitch stop_bit start_bit note_velocity stop_bit 

        //11001000 001nnnnn nnn01011 111110 00
*/

//Argumentem jest przycisk od 1 do 25
void noteOnMIDI(int button, char octave, UART_HandleTypeDef *huart){
	uint8_t MessageLength=0;
	uint8_t pitch=translatePitchToMIDI(button, octave);
	uint8_t DataToSend[3] = {
	0b10010000,
        //0b00100000|(pitch>>3),
        //pitch<<5|0b00001011,
        //0b11111000
	pitch,
	//0b00111100,
	0b1000000
	};
	MessageLength = sizeof(DataToSend);
	/*TUTAJ WYSŁANIE WIADOMOŚCI PRZEZ USB*/
	for (int i=0;i<3;++i){
			HAL_UART_Transmit(huart, &DataToSend[i],MessageLength,1000);
	}
	if(octave=='l')
		noteOffMIDI(14, octave, huart);
}

//Argumentem jest numer kontrolera i wartość
void controllerValueChangeMIDI(int controller, int value, UART_HandleTypeDef *huart){
	uint8_t MessageLength=0;
	uint8_t DataToSend[3] = {
	0b10110000,
        //0b00100000|(pitch>>3),
        //pitch<<5|0b00001011,
        //0b11111000
	controller,
	//0b00111100,
	value
	};
	MessageLength = sizeof(DataToSend);
	/*TUTAJ WYSŁANIE WIADOMOŚCI PRZEZ USB*/
	for (int i=0;i<3;++i){
			HAL_UART_Transmit(huart, &DataToSend[i],MessageLength,1000);
	}
}
