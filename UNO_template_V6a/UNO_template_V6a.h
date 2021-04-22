
//UNO slave address has been set to 3



#include <avr/wdt.h>

float FPN_number_from_IO(void);
float Float_from_KBD(void);
long Int_number_from_IO(void);
long Int_from_KBD(void);
char float_num_to_display(float);
char int_num_to_display(long);

char * Char_ptr;                                                                //Used to convert a FPN to its individual bytes                                        
unsigned char num_bytes[4];                                                     //The four bytes of a FPN
unsigned char data_buff[12];                                                    //Used to hold numerical strings

volatile char cr_keypress;                                                      //Used to control data transfer over the I2C bus
volatile char active_transaction;
                                  
char scroll_control, dp_control, neg_sign, exp_control;                         //Used to control data entry via IO 
volatile char Data_Entry_complete, digit_entry;

volatile char num_type;                                                        	 //Floating point or integer

#include "Resources_UNO_template\UNO_TWI_subroutines_V6a.c"
#include "Resources_UNO_template\ASKII_subroutines_V6a.c"
#include "Resources_UNO_template\PCI_subroutines_V6a.c"


/************************************************************************************************************************************/
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
TWAR = 0x06;\
flash_on_FPN_overflow;\
Serial.begin(38400);\
while (!Serial);



/************************************************************************************************************************************/
#define flash_on_FPN_overflow \
\
if(!(eeprom_read_byte((uint8_t*)0x3FB))){eeprom_write_byte((uint8_t*)(0x3FA),0);\
float_num_1 = float_num_from_eepom();\
float_num_to_display(float_num_1);\
wdt_enable(WDTO_500MS);\
while(1){if(switch_3_down)\
{eeprom_write_byte((uint8_t*)0x3FB, 0xFF);\
eeprom_write_byte((uint8_t*)(0x3FA),0xFF);break;}}}


/************************************************************************************************************************************/
#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up    (PIND & 0x04)
#define switch_3_down  ((PINB & 0x04)^0x04)


/************************************************************************************************************************************/
