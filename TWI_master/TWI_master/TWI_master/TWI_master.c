
#include "Project.h"

void Display_driver(void);
void Char_definition(void);
void USI_TWI_Master_Initialise(void);
long string_to_binary(char *);


volatile int buf_ptr;
volatile char int_counter;

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
int_counter = 0;
//digit_3; 
sei();

USI_TWI_Master_Initialise();
while (!(send_save_address_plus_RW_bit(0x6)));	//three;			//master writes to slave
for(int m = 0; m <= 4; m++){
	if (m==4)write_data_to_slave(4, 1);
	else write_data_to_slave(m, 0);	}
while(!(send_save_address_plus_RW_bit(0x6)));				//
for(int m = 5; m <= 9; m++){
	if (m==9)write_data_to_slave(m, 1);
else write_data_to_slave(m, 0);	}


while(1){
TCCR0B = 1;
while (!(cr_keypress));
cr_keypress = 0;

Number = 0;
/*for(int m = 0; m <= 3; m++){
if(display_buf[m]){	
Number = Number*10 + (display_buf[m] - '0');}}*/
Number = string_to_binary(display_buf);


/*{Number = display_buf[0] - '0';												//Convert the string to a number
	Number = Number*10 + (display_buf[1] - '0');									//But only after a return key press
	Number = Number*10 + ( display_buf[2] - '0');
Number = Number*10 + ( display_buf[3] - '0');}*/

while (!(send_save_address_plus_RW_bit(0x6)));							//Data sent to UNO
	for(int m = 0; m <= 3; m++){												//BUT only after a return key press
		if(m == 3)write_data_to_slave(Number, 1);
	else write_data_to_slave(Number >> (8*(3-m)), 0);}
}





while(1);

while(1){
USI_TWI_Master_Initialise();
for(int m = 0; m < 4; m++)
{while(!(data));TCCR0B = 0;							//Pause clock for I2C transaction
	display_buf[m] = data; data = 0;}
USICR = 0;											//Put I2C on hold
TCCR0B = 1;	}										//Re-start 8MHz clock



wdt_enable(WDTO_60MS); while(1);}



/******************************************************************************************************/
ISR (TIMER0_OVF_vect){TCNT0H = 0x7F;	//TCCR0B = 0;				//Generates interrupt every 4.096mS.
	TCNT0L = 0xFF;
Display_driver();
int_counter ++;
if (int_counter == 10){int_counter = 0; data_from_UNO(); }
}



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
	
	
	

long string_to_binary(char array[]){
	
	char sign = '+';
	long number = 0;

	for(int m = 0; m <= 3; m++){
		if(array[m]){
			if(array[m] == '-'){sign = '-'; continue;}
		number = number*10 + (array[m] - '0');}}
	if (sign == '-')number *= (-1);
	return number;}


