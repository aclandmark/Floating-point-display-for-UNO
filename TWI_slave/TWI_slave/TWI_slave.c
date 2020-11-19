
#include "Project.h"

void Display_driver(void);
void Char_definition(void);
void USI_TWI_Slave_Initialise(unsigned char);

volatile int buf_ptr;


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
data = 0;
sei();

while(1){
USI_TWI_Slave_Initialise(6);
for(int m = 0; m < 4; m++)
{while(!(data));TCCR0B = 0;							//Pause clock for I2C transaction
	display_buf[m] = data; data = 0;}
USICR = 0;											//Put I2C on hold
TCCR0B = 1;	}										//Re-start 8MHz clock

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
		case '-': PORTB &= (~(seg_g)); break;
		case 'E': case 'e':
	PORTB &= (~(seg_a | seg_f)); PORTB &= (~(seg_d | seg_e | seg_g ));break;}}
	
	
	

