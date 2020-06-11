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

_uint8_t MIDIcodes [5][13] = {
{0b00011100,0b00011101,0b00011110,0b00011111,0b00100000,0b00100001,0b00100010,0b00100011,0b00100100,0b00100101,0b00100110,0b00100111,0b00101000},
{0b00101000,0b00101001,0b00101010,0b00101011,0b00101100,0b00101101,0b00101110,0b00101111,0b00110000,0b00110001,0b00110010,0b00110011,0b00110100},
{0b00110100,0b00110101,0b00110110,0b00110111,0b00111000,0b00111001,0b00111010,0b00111011,0b00111100,0b00111101,0b00111110,0b00111111,0b01000000},
{0b01000000,0b01000001,0b01000010,0b01000011,0b01000100,0b01000101,0b01000110,0b01000111,0b01001000,0b01001001,0b01001010,0b01001011,0b01001100},
{0b01001100,0b01001101,0b01001110,0b01001111,0b01010000,0b01010001,0b01010010,0b01010011,0b01010100,0b01010101,0b01010110,0b01010111,0b01011000}
};//E,F,F#,G,G#,A,A#,B,C,C#,D,D#,E

_uint8_t translatePitchToMIDI(int button, char octave){ 
	_uint8_t pitch;
	//octave: s - standard; h - high; l - low, b - super low, g - super high	

	if(octave=='b'){
		pitch=MIDIcodes[0][button-1];
	}
	else if(octave=='l'){
		pitch=MIDIcodes[1][button-1];
	}
	else if(octave=='s'){
		pitch=MIDIcodes[2][button-1];
	}
	else if(octave=='h'){
		pitch=MIDIcodes[3][button-1];
	}	
	else{//tutaj jest przypadek octave == 'p'
		pitch=MIDIcodes[4][button-1];
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
void noteOffMIDI(int button, char octave){
	_uint8_t MessageCounter=0;
	_uint8_t MessageLength=0;
	_uint8_t pitch=translatePitchToMIDI(button, octave);
	_uint8_t DataToSend[4] = {				
	0b11000000, 						
        0b00100000|(pitch>>3), 								
        pitch<<5|0b00001011, 
        0b11111000
	};
	MessageLength = sizeof(DataToSend);
	/*TUTAJ WYSŁANIE WIADOMOŚCI PRZEZ USB*/
	for (int i=0;i<4;++i){
		HAL_UART_Transmit(huart2,&DataToSend[i],MessageLength,1000);
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
void noteOnMIDI(int button, char octave){
	_uint8_t MessageCounter=0;
	_uint8_t MessageLength=0;
	_uint8_t pitch=translatePitchToMIDI(button, octave);
	_uint8_t DataToSend[4] = {				
	0b11001000, 						
        0b00100000|(pitch>>3), 								
        pitch<<5|0b00001011, 
        0b11111000
	};
	MessageLength = sizeof(DataToSend);
	/*TUTAJ WYSŁANIE WIADOMOŚCI PRZEZ USB*/
	for (int i=0;i<4;++i){
			HAL_UART_Transmit(huart2,&DataToSend[i],MessageLength,1000);
	}
}


