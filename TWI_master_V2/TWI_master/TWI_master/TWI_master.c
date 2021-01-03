


#include "Project.h"

void Display_driver(void);
void Char_definition(void);
void USI_TWI_Master_Initialise(void);
long string_to_binary(char *);

signed char Round_and_Display(char*, char, signed char);



int main (void){

char letter;													//Used to sends askii chars to UNO for test purposes

setup_ATtiny_HW;	

buf_ptr = 0;														//Used by display driver: Points to next digit of display 
set_digit_drivers;
clear_digits;
clear_display;	
TCCR0A |= 1 << TCW0;												//Timer0 in 16 bit mode
OCR0B =	0xE8;														//Used to control intensity
OCR0A = 0x0;
TCNT0H = 0xE0;												//Generates 1mS interrupt stream 
TCNT0L = 0x0;		
TIMSK |= (1 << TOIE0) | (1 << OCIE0A);								//Initialise Timer interrupts

int_counter = 0;													//T0 overflow interrupt counter

letter = '!';
sei();																//Required by display and TWI

USI_TWI_Master_Initialise();
while (!(send_save_address_plus_RW_bit(0x6)));						//master writes to slave
for(int m = 0; m <= 93; m++){
	if (m==93)write_data_to_slave(letter, 1);
	else write_data_to_slave(letter++, 0);	}


TCCR0B = 1;																//Start 4mS Timer0 clock:TWI ready to receive binary or string data 
while(1){
while (!(cr_keypress));													//Wait here for TWI interrupts. 
cr_keypress = 0;														//String received from UNO: Clear carriage return 



switch (transaction_type){
	case 'A':															//Integer string received
I_number = string_to_binary(display_buf);								//Convert the string to a binary I_number

while (!(send_save_address_plus_RW_bit(0x6)));							//Return the I_number to the UNO
	for(int m = 0; m <= 3; m++){
		if(m == 3)write_data_to_slave(I_number, 1);
	else write_data_to_slave(I_number >> (8*(3-m)), 0);}break;

case 'C':																//Floating point number string received. Convert display format to C format

for(int m = 0; m <= 7; m++){if(display_buf[m] & 0x80)break;				//Add decimal point if necessary
if (m == 7)display_buf[m] |= 0x80;}



for(int m = 0; m <= 15; m++)flt_array[m] = 0;							//Clear the array buffer
for(int m = 0; m <= 7; m++)flt_array[m] = display_buf[m];				//Copy the display into the buffer
while (!(flt_array[0]))													//Shift the buffer so array zero is ocupied
{ for(int m = 0; m < 7 ; m++)
	{flt_array[m] = flt_array[m+1]; flt_array[m+1] = 0;}}


array_cntr = 0;
for(int m = 0; m <= 9; m++){											//Locate the digit that is combined with a decimal point (if any)
	if (!(flt_array[m] & 0x80))continue;								
array_cntr = m+1;break;}

if(array_cntr){for(int m = 9; m > array_cntr ; m--)						//Shift the array one place to the right creating space for the decimal point 
	{flt_array[m] = flt_array[m-1];}
flt_array[array_cntr] = '.';											//Insert the decimal point
flt_array[array_cntr-1]	&= 0x7F;}										//Remove the decimal point from digit with which it was combined

flt_num = atof(flt_array);												//Convert the floating point array to a floating point number
ftoa(flt_num, flt_array, 0);

char_ptr = (char*)&flt_num;												//Split the number into bytes and return them to the UNO
while (!(send_save_address_plus_RW_bit(0x6)));
write_data_to_slave(*char_ptr, 0); char_ptr += 1;
write_data_to_slave(*char_ptr, 0); char_ptr += 1;
write_data_to_slave(*char_ptr, 0); char_ptr += 1;
write_data_to_slave(*char_ptr, 1);
break;
}}																		//If binary data is received, display it but return nothing to the UNO



while(1);


wdt_enable(WDTO_60MS); while(1);}



/******************************************************************************************************/


ISR (TIMER0_OVF_vect){TCNT0H = 0xE0;				//Generates interrupt every 4.096mS.
	TCNT0L = 0x0;
	char request_counter;
	//unsigned int i;
	
		int_counter ++;
		
		if (int_counter == 20){	int_counter = 0;															//update display every 100mS
		if(Display_mode == 1){
			if(PINA &(1 << PA1)){for(int m = 0; m <= 7; m++)									//Copy the array to the display buffer
			display_buf[m] = flt_array[m];}
			
			else {for(int m = 0; m <= 7; m++)													//Copy the array to the display buffer
			display_buf[m] = Non_exp_array[m];}	}
				
		TCCR0B = 0; data_from_UNO(); TCCR0B = 1;}


		if(int_counter == 10){//int_counter = 0;												//9 causes flicker
			request_counter = 32;
			while (((!(send_save_address_plus_RW_bit(0x8)))) && request_counter)			//Address is 3 and W/R bit is 1 for UNO transmit.
			{ request_counter -= 1;}
			if (request_counter){
				write_data_to_slave(display_buf[3], 0);
				write_data_to_slave(display_buf[2], 0);
				write_data_to_slave(display_buf[1], 0);
			write_data_to_slave(display_buf[0], 1);}}
		
		clear_digits;
		clear_display;

		
		}




/******************************************************************************************************/
void Display_driver()								//Display multiplexer advances every 4mS							
{ buf_ptr = buf_ptr%8;
	if (!(buf_ptr))buf_ptr = 4;
	//clear_digits;
	//clear_display;
	switch(buf_ptr){
		case 4: {digit_7;} break;
		case 5: {digit_6;} break;
		case 6: {digit_5;} break;
		case 7: {digit_4;} break;}
	Char_definition();}




/******************************************************************************************************/
	ISR (TIMER0_COMPA_vect){						//Controls display intensity
	//if (!(int_counter%4))				//1 or 4
	{Display_driver();}}


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
		case '-': minus; break;
		case 'E': case 'e':
	PORTB &= (~(seg_a | seg_f)); PORTA &= (~(seg_d | seg_e | seg_g ));break;
	
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
	case ('-' | 0x80): minus_point; break;
	
	}
	buf_ptr++;}
	
	
	

long string_to_binary(char array[]){
	
	char sign = '+';
	long num = 0;

	for(int m = 0; m <= 7; m++){
				
		if(array[m]){
		if(array[m] == '-'){sign = '-'; continue;}
			
		if (array[m] & 0x80)	num = num*10 + ((array[m] & 0x7F) - '0');				//Ignore decimal point
		else num = num*10 + (array[m] - '0');}}
	if (sign == '-')num *= (-1);
	return num;}
	
	
	/*******************************************************************************************************************/
	