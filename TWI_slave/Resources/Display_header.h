


volatile int data_ptr;

/***********************************************************************/

#define setup_ATtiny_HW \
setup_watchdog;\
USIPP |= 1 << USIPOS;\
PORTA |= (1<< PORTA1);\
cal_device;

//setup_IO;



/***********************************************************************/
#define wdr()  __asm__ __volatile__("wdr")

#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCR |= (1 <<WDCE) | (1<< WDE);\
WDTCR = 0;

//Leaves TWI ports in their default states
#define setup_IO \
MCUCR &= (~(1 << PUD));\
DDRA = 0;\
PORTA = 0xFA;\
DDRB = 0;\
PORTB = 0xFF;\
USIPP |= 1 << USIPOS;


#define cal_device \
eeprom_write_byte((uint8_t*)(EE_size - 3), OSCCAL);\
if ((eeprom_read_byte((uint8_t*)(EE_size - 2)) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)(EE_size - 2)) < 0xF0) && (eeprom_read_byte((uint8_t*)(EE_size - 2))\
== eeprom_read_byte((uint8_t*)(EE_size - 1)))) OSCCAL = eeprom_read_byte((uint8_t*)(EE_size - 2));



/*******************************************************************************************************/
/*#define	digit_3		PORTB &= (~(1 << PB6));
#define	digit_2		PORTA &= (~(1 << PA6));
#define	digit_1		PORTA &= (~(1 << PA5));
#define	digit_0		PORTB &= (~(1 << PB0));*/

#define	digit_3		PORTB |= (1 << PB6);
#define	digit_2		PORTA |= (1 << PA6);
#define	digit_1		PORTA |= (1 << PA5);
#define	digit_0		PORTB |= (1 << PB0);

#define	seg_a 	(1 << PA3)
#define	seg_b 	(1 << PA7)
#define	seg_c 	(1 << PB4)
#define	seg_d 	(1 << PB2)
#define	seg_e 	(1 << PB1)
#define	seg_f 	(1 << PA4)
#define	seg_g 	(1 << PB5)
#define	DP		(1 << PB3)


#define set_digit_drivers \
DDRA |= (1 << DDA3) | (1 << DDA4) | (1 << DDA5) | (1 << DDA6) | (1 << DDA7);\
DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5) | (1 << DDB6);

//#define clear_digits	PORTB |= ((1 << PB0) | (1 << PB6)); 
//PORTA |= ((1 << PA5) | (1 << PA6));

#define clear_digits	PORTB &= (~((1 << PB0) | (1 << PB6))); \
PORTA &= (~((1 << PA5) | (1 << PA6)));



#define clear_display \
PORTA |= (seg_a | seg_b | seg_f);\
PORTB |= (seg_c | seg_d | seg_e | seg_g | DP);


#define clear_display_buffer	for(int m = 0; m<=3; m++)display_buf[m] = 0;


#define one_U 	PORTA &= ~(seg_b);
#define ONE_U 	PORTB &= ~(seg_e);
#define one_L 	PORTB &= ~(seg_c);
#define ONE_L 	PORTB &= ~(seg_e);
#define ONE		PORTB &= ~(seg_e); PORTB &= ~(seg_e);
#define one 	PORTA &= ~(seg_b); PORTB &= ~(seg_c);		//(~(seg_b | seg_c));
#define two 	PORTA &= (~(seg_a | seg_b )); PORTB &= (~(seg_d | seg_e | seg_g));
				//PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_d | seg_e | seg_g));
#define three 	PORTA &= (~(seg_a | seg_b)); PORTB &= (~(seg_c | seg_d | seg_g));
				//PORTB &= (~(seg_a )); PORTD &= (~(seg_b | seg_c | seg_d | seg_g));
#define four 	PORTA &= (~(seg_b | seg_f)); PORTB &= (~(seg_c | seg_g));
				//PORTD &= (~(seg_b | seg_c | seg_f | seg_g));
#define five 	PORTA &= (~(seg_a | seg_f )); PORTB &= (~(seg_c | seg_d | seg_g));
				//PORTB &= (~(seg_a)); PORTD &= (~(seg_c | seg_d | seg_f | seg_g));
#define six 	PORTA &= (~(seg_f)); PORTB &= (~(seg_c | seg_d | seg_e | seg_g));
				//PORTD &= (~(seg_c | seg_d | seg_e | seg_f | seg_g));
#define seven 	PORTA &= (~(seg_a | seg_b)); PORTB &= (~(seg_c)); 
				//PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_c ));
#define eight 	PORTA &= (~(seg_a | seg_b | seg_f));  PORTB &= (~(seg_c | seg_d | seg_e | seg_g));
				//PORTB &= (~(seg_a));  PORTD &= (~(seg_b | seg_c | seg_d | seg_e | seg_f | seg_g));
#define nine	PORTA &= (~(seg_a | seg_b | seg_f)); PORTB &= (~( seg_c | seg_g));
				//PORTD &= (~(seg_b | seg_c | seg_f | seg_g));
#define zero	PORTA &= (~(seg_a | seg_b | seg_f )); PORTB &= (~( seg_c | seg_d | seg_e  ));
				//PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_c | seg_d | seg_e | seg_f ));


