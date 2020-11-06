	/*
	
	*/
	
	
							

	#include "Project.h"
	void Display_driver(void);
	void Char_definition(void);
	
	
	
	int main (void)
	{
		
								
		setup_ATtiny_HW;													//Configures USI as a transmitter
		User_prompt;														//Respond by pressing R or r.
set_digit_drivers;
clear_digits;
clear_display;
								
		set_device_type_and_memory_size;									//confirm device type and set EEPROM size
		String_to_USI("\r\n\r\nProgram running on ");										//\r\n\r\nProgram running on 
		String_to_USI("\r\nATtiny");
		String_to_USI (Device_type[device_ptr]);
		if (device_ptr == 7)while(1);
		newline();
		Char_from_USI(0);
		//Setup T0 8.192mS overflow interrupt using 8MHz clock counting to 65536
		
		for (int m = 0; m<=3; m++) display_buf[m] = m + '0';
		/*display_buf[0] = '1';
		display_buf[1] = '2';
		display_buf[2] = '3';
		display_buf[3] = '4';*/
		
		
		TCCR0A |= 1 << TCW0;				//16 bit mode
		OCR0B = 0x1;
		OCR0A = 0;
		TCNT0H = 0x7F;						
		TCNT0L = 0xFF;
		TCCR0B = 1;							//Start 8MHz clock
		TIMSK |= (1 << TOIE0) | (1 << OCIE0A);				//Initialise Timer over flow interrupt
		sei();
		
		while(1){
		switch (Char_from_USI(0)){
		case '5': OCR0B = 0x80; OCR0A = 0x80; break;
		case '4': OCR0B = 0x82;OCR0A = 0; break;
		case '3': OCR0B = 0x88;OCR0A = 0; break;
		case '2': OCR0B = 0xA0; OCR0A = 0;break;
		case '1':	OCR0B = 0x1;OCR0A = 0; break;	}
		}
		
	
		/*Char_from_USI(0);
		OCR0B = 0xA0;
		OCR0A = 0;
		
		Char_from_USI(0);
		OCR0B = 0x88;
		OCR0A = 0;
		
		Char_from_USI(0);
		OCR0B = 0x82;
		OCR0A = 0;*/
		
		
		
		while(1);
		wdt_enable(WDTO_60MS); while(1);
	}
	
		
ISR (TIMER0_OVF_vect){TCNT0H = 0x7F;
	TCNT0L = 0xFF;
	Display_driver();//TCCR0B = 0;
	}
	
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


ISR (TIMER0_COMPA_vect){
	clear_display;}




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