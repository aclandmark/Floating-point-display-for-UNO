
/*
Project used to develop TWI slave routines that will 
receive the most significant digits to be displayed.
*/


#include "Project.h"

void Display_driver(void);
void Char_definition(void);
void USI_TWI_Slave_Initialise(unsigned char);

volatile int buf_ptr;
volatile int int_counter;

int main (void){

setup_ATtiny_HW;	

set_digit_drivers;
clear_digits;
clear_display;	
TCCR0A |= 1 << TCW0;								//16 bit mode
OCR0B = 0x1;
OCR0A = 0;
TCNT0H = 0x7F;
TCNT0L = 0xFF;
TIMSK |= (1 << TOIE0) | (1 << OCIE0A);				//Initialise Timer interrupts
TCCR0B = 1;
sei();

for(int m = 0; m <= 3; m++)
{display_buf[m] = m + '0';}							//Initialise display


USI_TWI_Slave_Initialise(4);						//Address of this slave is 4

//USICR = 0;

while(1){											//Receive floating point number strings to drive display
for(int m = 0; m <= 2; m++){
while(!(USI_busy));									//wait for master request
while((USI_busy));									//Pause here while data is received
for(int m = 0; m <= 3; m++)
{display_buf[m] = Rx_data[m]; Rx_data[m] = 0;}
}


while(!(USI_busy));									//data ready: wait for master request
while((USI_busy));									//wait for data transfer to complete
if (Rx_data[0] == 'X')

for(int m = 0; m <=2; m++){
for(int m = 'A'; m <= 'J'; m++)						//Generate string "AB.......J"
Tx_data[m - 'A'] = m;
Tx_data[10] = 0;									//Terminate Tx string in a null character
while(!(USI_busy));									//data ready: wait for master request
while((USI_busy));									//wait for data transfer to complete

									
for(int m = 'K'; m <= 'T'; m++)						//Repeat with string "KLM......T"
Tx_data[m - 'K'] = m;
Tx_data[10] = 0;
while(!(USI_busy));while((USI_busy));}				//Pause here while TWI interrupt take over

else 

for(int m = 0; m <=2; m++){							//Send string "012........987.......0"
	for(int m = '0'; m <= '9'; m++)
	Tx_data[m - '0'] = m;
	Tx_data[10] = 0;
	while(!(USI_busy));									
	while((USI_busy));		
for(int m = 0; m <= 9; m++)Tx_data[m] = 9-m  + '0';
Tx_data[10] = 0;
while(!(USI_busy));while((USI_busy));}}


while(1);



/*while(1){											//Intensity control
	switch (Char_from_USI(0)){
		case '5': OCR0B = 0x80; OCR0A = 0x80; break;
		case '4': OCR0B = 0x82;OCR0A = 0; break;
		case '3': OCR0B = 0x88;OCR0A = 0; break;
		case '2': OCR0B = 0xA0; OCR0A = 0;break;
	case '1':	OCR0B = 0x1;OCR0A = 0; break;	}
}*/

wdt_enable(WDTO_60MS); while(1);}



/******************************************************************************************************/
/*ISR (TIMER0_OVF_vect){TCNT0H = 0x7F;				//Generates interrupt every 4.096mS.
	TCNT0L = 0xFF;
	Display_driver();
	sei();TCCR0B = 0;
	if(data){
	for(int m = 0; m < 4; m++)
	display_buf[m] = data; data = 0;while(!(data));}TCCR0B = 1;}*/

ISR (TIMER0_OVF_vect){TCNT0H = 0x7F;					//Generates interrupt every 4.096mS.
	TCNT0L = 0xFF;
Display_driver();}



/******************************************************************************************************/
void Display_driver()
{buf_ptr++; buf_ptr = buf_ptr%4;
	clear_digits;
	clear_display;
	switch(buf_ptr){
		case 0: {digit_0;} break;
		case 1: {digit_1;} break;
		case 2: {digit_2;} break;
		case 3: {digit_3;} break;}
	Char_definition();}




/******************************************************************************************************/
	ISR (TIMER0_COMPA_vect){
	clear_display;}



/******************************************************************************************************/
	void Char_definition()
	{switch (display_buf[buf_ptr]){
		case '0': zero; break;
		case '1': one; break;
		case '2': two; break;
		case '3': three; break;
		case '4': four; break;
		case '5': five; break;
		case '6': six; break;
		case '7': seven; break;
		case '8': eight; break;
		case '9': nine; break;
		case '-': PORTA &= (~(seg_g)); break;
		case 'E': case 'e':
	PORTA &= (~(seg_a | seg_f)); PORTA &= (~(seg_d | seg_e | seg_g ));break;
	
	
	case ('0' | 0x80): zero_point; break;
	case ('1' | 0x80): one_point; break;
	case ('2' | 0x80): two_point; break;
	case ('3' | 0x80): three_point; break;
	case ('4' | 0x80): four_point; break;
	case ('5' | 0x80): five_point; break;
	case ('6' | 0x80): six_point; break;
	case ('7' | 0x80): seven_point; break;
	case ('8' | 0x80): eight_point; break;
	case ('9' | 0x80): nine_point; break;
		
	}}
	
	
	

