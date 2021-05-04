
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

volatile char num_type;    //Floating point or integer


#define OVF_cntl_reg     (uint8_t*)0x3FB          //EEPROM address. Set bits to zero and reset them to one
#define OVF_test          0x0                     //Bit 0: Set for OVF test and reset if OK
#define OVF_active        0x01                    //Bit 1: Set to flash display if OVF test fails
#define Restore_OVF       0x02                    //Bit 2: Set on recovery from OVF
#define Recovery_active   0x03                    //Bit 3: Recovery from glitch




#include "Resources_UNO_template\UNO_TWI_subroutines_V1a.c"
#include "Resources_UNO_template\ASKII_subroutines_V1a.c"
#include "Resources_UNO_template\PCI_subroutines_V1a.c"


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
if(!(eeprom_read_byte(OVF_cntl_reg) & (1 << OVF_test))){\
eeprom_write_byte(OVF_cntl_reg,(eeprom_read_byte(OVF_cntl_reg)) & 0xFD);\
float_num_1 = float_num_from_eepom();\
float_num_to_display(float_num_1);\
wdt_enable(WDTO_500MS);\
while(1){if(switch_3_down)\
{eeprom_write_byte(OVF_cntl_reg,(eeprom_read_byte(OVF_cntl_reg) | ((1<<OVF_test)|(1<<OVF_active))));\
eeprom_write_byte(OVF_cntl_reg,(eeprom_read_byte(OVF_cntl_reg) & (~(1 << Restore_OVF))));\
\
break;\
}}}


//eeprom_write_byte((uint8_t*)(0x3FB),eeprom_read_byte((uint8_t*)(0x3FB)) | 3);eeprom_write_byte((uint8_t*)(0x3FA),0xFF);

/************************************************************************************************************************************/
#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up    (PIND & 0x04)
#define switch_3_down  ((PINB & 0x04)^0x04)


/************************************************************************************************************************************/
