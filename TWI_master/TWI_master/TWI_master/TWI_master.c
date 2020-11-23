
#include "Project.h"

void Display_driver(void);
void Char_definition(void);
void USI_TWI_Master_Initialise(void);
long string_to_binary(char *);




int main (void){

setup_ATtiny_HW;	

set_digit_drivers;
clear_digits;
clear_display;	
TCCR0A |= 1 << TCW0;												//Timer0 in 16 bit mode
OCR0B = 0x1;														//Used to control intensity
OCR0A = 0;
TCNT0H = 0x7F;														//Generates 4mS interrupt stream 
TCNT0L = 0xFF;
TIMSK |= (1 << TOIE0) | (1 << OCIE0A);								//Initialise Timer interrupts

int_counter = 0;													//T0 overflow interrupt counter
 
sei();																//Required by display and TWI

USI_TWI_Master_Initialise();
while (!(send_save_address_plus_RW_bit(0x6)));						//master writes to slave
for(int m = 0; m <= 4; m++){
	if (m==4)write_data_to_slave(4, 1);
	else write_data_to_slave(m, 0);	}
while(!(send_save_address_plus_RW_bit(0x6)));				//
for(int m = 5; m <= 9; m++){
	if (m==9)write_data_to_slave(m, 1);
else write_data_to_slave(m, 0);	}


TCCR0B = 1;																//Start 4mS Timer0 clock:TWI ready to receive binary or string data 
while(1){
while (!(cr_keypress));													//Wait here for TWI interrupts. 
cr_keypress = 0;														//String received from UNO: Clear carriage return 

Number = string_to_binary(display_buf);									//Convert the string to a binary number

while (!(send_save_address_plus_RW_bit(0x6)));							//Return the number to the UNO
	for(int m = 0; m <= 3; m++){
		if(m == 3)write_data_to_slave(Number, 1);
	else write_data_to_slave(Number >> (8*(3-m)), 0);}
}																		//If binary data is received, display it but return nothing to the UNO





while(1);

/*while(1){
USI_TWI_Master_Initialise();
for(int m = 0; m < 4; m++)
{while(!(data));TCCR0B = 0;							//Pause clock for I2C transaction
	display_buf[m] = data; data = 0;}
USICR = 0;											//Put I2C on hold
TCCR0B = 1;	}										//Re-start 8MHz clock
*/


wdt_enable(WDTO_60MS); while(1);}



/******************************************************************************************************/
ISR (TIMER0_OVF_vect){TCNT0H = 0x7F;				//Generates interrupt every 4.096mS.
	TCNT0L = 0xFF;
Display_driver();
int_counter ++;
if (int_counter == 25)								//update display every 100mS
{int_counter = 0;
data_from_UNO(); }									//Poll the UNO 
}



/******************************************************************************************************/
void Display_driver()								//Display multiplexer advances every 4mS							
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
	ISR (TIMER0_COMPA_vect){						//Controls display intensity
	clear_display;}									//by shutting drive current down in less than 4mS.



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
	PORTB &= (~(seg_a | seg_f)); PORTB &= (~(seg_d | seg_e | seg_g ));break;
	
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
	
	
	

long string_to_binary(char array[]){
	
	char sign = '+';
	long num = 0;

	for(int m = 0; m <= 3; m++){
				
		if(array[m]){
		if(array[m] == '-'){sign = '-'; continue;}
			
		if (array[m] & 0x80)	num = num*10 + ((array[m] & 0x7F) - '0');
		else num = num*10 + (array[m] - '0');}}
	if (sign == '-')num *= (-1);
	return num;}


