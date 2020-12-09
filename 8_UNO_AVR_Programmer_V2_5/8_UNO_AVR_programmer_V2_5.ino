


#include "UNO_AVR_programmer.h"
#define Version "UNO_programmer_V2.5\r\n" 

void Num_from_KBD(unsigned char *);
void int_num_to_display(void);
void float_string_to_display(void);
void int_string_to_display(void);
void ftoa(float, char*, int);




int main (void){ 
char str[10];
//float float_num;

setup_328_HW;                                                     //see "Resources\ATMEGA_Programmer.h"
char counter, keypress;


Reset_L;                                                          //Put target in reset state to dissable UART

while(1){
do{sendString("s  ");} 
while((isCharavailable(255) == 0));                               //User prompt 
if(receiveChar() == 's')break;}

Atmel_powerup_and_target_detect;                                  //Leave target in programming mode                              


sendString(" detected.\r\nPress -p- to program flash, \
-e- for EEPROM, -r- to run target or -x- to escape.");

while(1){
op_code = waitforkeypress();
switch (op_code){

case 'r': Exit_programming_mode; break;                      //Wait for UNO reset
case 'R': Verify_Flash_Text();  SW_reset; break;
//case 'e': Prog_EEPROM(); SW_reset; break;
case 't': set_cal_clock();break;

case 'd':                                                       //Delete contents of the EEPROM
sendString("\r\nReset EEPROM! D or AOK to escape");             //but leave cal data.
newline();
if(waitforkeypress() == 'D'){
sendString("10 sec wait");
for (int m = 0; m <= EE_top; m++)  
{Read_write_mem('I', m, 0xFF);}                                 //Write 0xFF to all EEPROM loactions bar the top 3
sendString(" Done\r\n");}
SW_reset;break;

case 'x': SW_reset; break;
default: break;} 

if ((op_code == 'p')||(op_code == 'P')) break;} 
sendString("\r\nSend hex file (or x to escape).\r\n");

Program_Flash_Hex();
Verify_Flash_Hex();

sendString("\r\nText_file? y or n\r\n");
if (waitforkeypress() == 'y')
{op_code = 't';                                                 //Required by UART ISR
Program_Flash_Text();}


sendString (Version);
newline();

Read_write_mem('I', EE_size - 4, \
(Atmel_config(signature_bit_2_h, signature_bit_2_l)));          //Define target type on target device
Read_write_mem('I', EE_size - 5, \
(Atmel_config(signature_bit_3_h, signature_bit_3_l)));       

initialise_IO;
Set_LED_ports;
LEDs_off;

Reset_H;
sei();

/****************************Tests data flow: display pcb to UNO********************************************************/
sendString("\r\nUNO Rx. AK.\t");
waitforkeypress();
TWAR = 0x06;                                                            //UNO address is 3; R/W bit 0 is zero: UNO accepts data
UNO_slave_receiver();                                                   //Waits for TWI address match interrupt

/****************************Test UNO float to askii subroutines********************************************************/
sendString("\r\nFP_num_test\t");                                                     
strcpy(str, "1875.725");                                                //Type and reat number string
float_num = atof(str);                                                     //ASKII to float library function
float_num = float_num;                                                    //A bit of arithmetic
ftoa(float_num, str, 5);                                                   //Float to askii (non-library function)
sendString(str); 

/****************************Test UNO integer number to display**********************************************************/
sendString("\r\nUNO sends ints. AK.\r\n");
waitforkeypress();
Number = 9876;
  int_num_to_display();
 waitforkeypress();

do{
  Number = Number*15/16;                                                  //Some simple arithmetic
  int_num_to_display();
  if(isCharavailable (1)){receiveChar();waitforkeypress();}             //Enables user to pause data flow
  }while(Number >= 50);

/****************************Test UNO integer number string to display***************************************************/
sendString("\r\nUNO Tx. Enter num.\r\n");
Number = Num_from_KBD(data_buff);                                      //User entry terminates with a carriage return key press
for(int m = 0; m <= 4; m++) {                                          //Display pcb automatically reurns the string in binary form
Number = Number - Num_from_KBD(data_buff);                             //More simpe arithmetic
int_num_to_display();}

/****************************Test UNO float number string to display***FAILS!*********************************************/
sendString("\r\nFP_num?\r\n");


while(1){
float_num = Float_from_KBD(data_buff); 
ftoa(float_num, str, 2);                                                   //Float to askii (non-library function)
sendString(str);
while(1){
if (waitforkeypress()== 'x')break;
newline();
float_num /= 2.0;
float_num_to_display();
ftoa(float_num, str, 2);                                                   //Float to askii (non-library function)
sendString(str);
}newline();
}



while(1);


Exit_programming_mode;                                                  //Wait for UNO reset
return 1;}



/***************************************************************************************************************************************************/
ISR(USART_RX_vect){
switch (op_code){
case 't': upload_text();break;
case 'p':
case 'P': upload_hex(); break;}}



/***************************************************************************************************************************************************/
ISR(TIMER2_OVF_vect) {                                          //Timer2 times out and halts at the end of the text file
if(text_started == 3)                                           //Ignore timeouts occurring before start of file download
  {endoftext -= 1; TCCR2B = 0; TIMSK2 &= (~(1 << TOIE2));       //Shut timer down
  inc_w_pointer; store[w_pointer] = 0;                          //Append two '\0' chars to the end of the text
  inc_w_pointer; store[w_pointer] = 0; }}



/***************************************************************************************************************************************************/
void set_cal_clock(void){

sendString("\r\n\r\nSquare wave with 65.536mS period on PB5\r\n");
UCSR0B &= (~((1 << RXEN0) | (1<< TXEN0)));
initialise_IO;
Set_LED_ports;
LEDs_off;
DDRB |= 1 << DDB5;
PORTB &= (~(1 << PORTB5));                                       //Output low
TCNT0 = 0;
TCCR0B = (1 << CS02) | (1 << CS00);                             //7.8125 KHz clock counts to 256 in 32.768mS                    

Reset_H;

while(1){
while(!(TIFR0 & (1<<TOV0)));
TIFR0 |= (1<<TOV0);
PORTB ^= (1 << PORTB5);}}




/***************************************************************************************************************************************/
void int_string_to_display(void){
    
    data_type = 'A';                                                          //UNO sends numeric string to display pcb
    active_transaction = 1;
    TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);//}          //Enable TWI slave with interrupt on address match
    while (active_transaction);}


/***************************************************************************************************************************************/
void float_string_to_display(void){
    
    data_type = 'C';                                                          //UNO sends numeric string to display pcb
    active_transaction = 1;
    TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);//}          //Enable TWI slave with interrupt on address match
    while (active_transaction);}

/***************************************************************************************************************************************/
void float_num_to_display(void){
      
      //float * Flt_ptr_2;
      //char * Char_ptr_2;
      
      Char_ptr_2 = (char*)&float_num;
      data_type = 'D';
      active_transaction = 1;
      TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);            //Enable TWI slave
      while (active_transaction);}



/***************************************************************************************************************************************/
void int_num_to_display(void){
      data_type = 'B';
      active_transaction = 1;
      for (int m = 0; m <= 3; m++)num_bytes[m] = Number >> 8*m;                //Split the number into its bytes
      TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);            //Enable TWI slave
      while (active_transaction);}
