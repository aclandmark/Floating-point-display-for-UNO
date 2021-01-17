/*DEVELOPMENT OF UNO SLAVE TWI INTERFACE
UNO acts as master
slave address is 4; send 8 to address slave and write data to it
send 9 to read from it.*/


#define Initialise_I2C_master_write_04 \
  while(1){\
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);\
  while (!(TWCR & (1 << TWINT)));\
  TWDR = 0x08;\
  TWCR = (1 << TWINT) | (1 << TWEN);\
  while (!(TWCR & (1 << TWINT)));\
  if (TWSR == 0x18)break; } 

#define Initialise_I2C_master_read_04 \
  while(1){\
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);\
  while (!(TWCR & (1 << TWINT)));\
  TWDR = 0x09;\
  TWCR = (1 << TWINT) | (1 << TWEN);\
  while (!(TWCR & (1 << TWINT)));\
  if (TWSR == 0x40)break; }




#include "UNO_AVR_programmer.h"
#define Version "UNO_programmer_V2.5\r\n" 

void I2C_master_transmit(char );
char I2C_master_receive(char );
float F_Number;
char Tx_data[6];

int main (void){ 
char str[10];
//float float_num;
unsigned int  num;

setup_328_HW;                                                     //see "Resources\ATMEGA_Programmer.h"
char counter, keypress;
char R_data;

Reset_L;                                                          //Put target in reset state to dissable UART

while(1){
do{sendString("s  ");} 
while((isCharavailable(255) == 0));                               //User prompt 
if(receiveChar() == 's')break;}

/**********************************************************************

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
*************************************************************************/


Reset_H;

/********************************************************************************************************/
//Development code in here
sendString("Starting TWI AK \r\n");

TWBR = 32;                                                              //Set Baud rate clock (100KHz)
while(1){ 
sendString("fpn? \r\n");
for(int m = 0; m <= 2; m++){                                            //Floating point numer entry
for (int m = 0; m <= 5; m++){Tx_data[m] = 0;}                           //Clear FPN array
  
Float_from_KBD(Tx_data);                                                //FPN string from keyboard
F_Number = atof(Tx_data);
ftoa( F_Number, Tx_data, 1);                                            //Convert to fpn
sendString(Tx_data);                                                    //Ech string to PC


if(Tx_data[0]== '.')Tx_data[0]= '0' | 0x80;                             //Add decimal point
for(int m = 0; m <= 5; m++){ 
if  (m && (Tx_data[m]== '.')){Tx_data[m-1] |= 0x80;
for(int p = m; p <= 5; p++)Tx_data[p] = Tx_data[p+1];}}                 //Shift aray if necessary

for(int m = 0; m <= 3; m++){                                            //FPN library puts MSB in location zero
if (!(Tx_data[3])){for (int p = 0; p <= 2; p++)                         //Display driver puts LSB in locatin zero
Tx_data[3-p] = Tx_data[2-p];Tx_data[m] = 0;}
else break;}

Initialise_I2C_master_write_04;                                         //Transmit number in string form
for (int m = 0; m <= 3; m++){
I2C_master_transmit(Tx_data[m]);}
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
newline();}
                                                              
sendString("\r\n'X' or AOK\r\n");                                       //Receive data from slave 

keypress = waitforkeypress();
Initialise_I2C_master_write_04;

I2C_master_transmit(keypress);                                          //Slave set up to receive 4 bytes at a time
I2C_master_transmit(keypress);
I2C_master_transmit(keypress);
I2C_master_transmit(keypress);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);



for(int m = 0; m <=2; m++){                                           //Receive string 3 times
Initialise_I2C_master_read_04; newline();
for(int m = 0; m <= 9; m++){
if (m == 9)R_data = I2C_master_receive(0);
else R_data = I2C_master_receive(1);
sendChar(R_data);Timer_T0_10mS_delay_x_m(5);}
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);                     //Pause in middle of string

Initialise_I2C_master_read_04;
for(int m = 0; m <= 9; m++){
if (m == 9)R_data = I2C_master_receive(0);
else R_data = I2C_master_receive(1);
sendChar(R_data);Timer_T0_10mS_delay_x_m(5);}                         //TWI allows for pauses in transmission
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}}



/*********************************************************************************************************/



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

/************************************************************************************************************************/

  

  void I2C_master_transmit(char data){
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  }    



  char I2C_master_receive(char mode){
  char data;
  if(!(mode))TWCR = (1 << TWINT) | (1 << TWEN);       
  else TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);         
  while (!(TWCR & (1 << TWINT)));
  data =  TWDR;
  //sendHex(16,TWSR);
  return data;}
