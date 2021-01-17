
//UNO slave address has been set to 3


char data_buff[12];
unsigned char num_bytes[4];
volatile char cr_keypress;
volatile char active_transaction;
char * Char_ptr;


#include "Resources_UNO_template\UNO_TWI_subroutines_V1.c"
#include "Resources_UNO_template\ASKII_subroutines_V1.c"


#define initialise_IO \
MCUCR &= (~(1 << PUD));\
DDRB = 0;\
DDRC = 0;\
DDRD = 0;\
PORTB = 0xFF;\
PORTC = 0xFF;\
PORTD = 0xFF;\
DDRC |= 0x08;

#define Reset_L PORTC &= ~(0x08);
#define Reset_H PORTC |= 0x08;


/************************************************************************************************************************************/
#define wdr()  __asm__ __volatile__("wdr")

#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define SW_reset {wdt_enable(WDTO_30MS);while(1);}


/************************************************************************************************************************************/
#define setup_328_HW \
setup_watchdog;\
ADMUX |= (1 << REFS0);\
initialise_IO;\
Reset_L;\
_delay_ms(10);\
Reset_H;\
USART_init(0,50);
