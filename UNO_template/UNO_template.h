
//UNO slave address has been set to 3


char data_buff[12];
unsigned char num_bytes[4];
volatile char cr_keypress;
volatile char active_transaction;
char * Char_ptr;


#include "Resources_UNO_template\UNO_TWI_subroutines.c"
#include "Resources_UNO_template\ASKII_subroutines.c"


#define initialise_IO \
MCUCR &= (~(1 << PUD));\
DDRB = 0;\
DDRC = 0;\
DDRD = 0;\
PORTB = 0xFF;\
PORTC = 0xFF;\
PORTD = 0xFF;


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
CLKPR = (1 << CLKPCE);\
CLKPR = (1 << CLKPS0);\
setup_watchdog;\
ADMUX |= (1 << REFS0);\
initialise_IO;\
USART_init(0,25);



/************************************************************************************************************************************/
#define User_prompt \
while(1){\
do{sendString("R?    ");}  while((isCharavailable(250) == 0));\
User_response = receiveChar();\
if((User_response == 'R') || (User_response == 'r'))break;} sendString("\r\n");
