
//UNO slave address has been set to 3

#include <avr/wdt.h>

//void Num_from_KBD(unsigned char *);
char int_num_to_display(long);
void float_num_to_display(float);
void sendStringV(char*);

char * Char_ptr;
void scroll_display_zero(void);
void shift_display_left(void);
float acquire_fpn(void);

unsigned char data_buff[12];
unsigned char num_bytes[4];
volatile char cr_keypress;
volatile char active_transaction;

//unsigned char IO_buff[8];

volatile char Data_Entry_complete, digit_entry;
char scroll_control;
//char digits[8];
volatile char dp_control, neg_sign, exp_control;




#include "Resources_UNO_template\UNO_TWI_subroutines_V2.c"
#include "Resources_UNO_template\ASKII_subroutines_V2.c"
#include "Resources_UNO_template\PCI_subroutines.c"


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
Serial.begin(38400);\
while (!Serial);\
TWAR = 0x06;


#define flash_on_FPN_overflow \
\
if(!(eeprom_read_byte((uint8_t*)0x3FB))){eeprom_write_byte((uint8_t*)(0x3FA),0);\
float_num_1 = float_num_from_eepom();\
float_num_to_display(float_num_1);\
wdt_enable(WDTO_1S);\
while(1){if(Serial.read()== 'a')\
{eeprom_write_byte((uint8_t*)0x3FB, 0xFF);\
eeprom_write_byte((uint8_t*)(0x3FA),0xFF);break;}}}
