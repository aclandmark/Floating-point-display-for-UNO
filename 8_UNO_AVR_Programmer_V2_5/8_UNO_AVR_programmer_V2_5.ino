


#include "UNO_AVR_programmer.h"
#define Version "UNO_programmer_V2.5\r\n" 

void Num_from_KBD(unsigned char *);



int main (void){ 
long number;
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
case 'e': Prog_EEPROM(); SW_reset; break;
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
sendString("\r\nUNO Rx. AK.\r\n");
waitforkeypress();
TWAR = 0x06;
active_transaction = 0;
UNO_slave_receiver();                                                   //Test subroutine UNO receives data


data_type = 'B';
sendString("\r\nUNO sends ints. AK.\r\n");

waitforkeypress();Number = 9876;Update_display();
for(int m = 0; m <= 9; m++){
  waitforkeypress();Number = Number*3/4;Update_display();}


sendString("\r\nUNO Tx. Enter num.\r\n");
data_type = 'A';
while(1){
cr_keypress = 0;
number = Num_from_KBD(data_buff);
sendHex(10, number);}

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




void Update_display(void){
    switch(data_type) {    
    case 'A':
       TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE); 
    break;

  case 'B':   for (int m = 0; m <= 3; m++)num_bytes[m] = Number >> 8*m;
  TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE); 
   break;

    }
  }                             


/*****************************************************************/
long Num_from_KBD(char digits[]){
char keypress;
long number;
for(int n = 0; n<=7; n++) digits[n] = 0; 

do
{keypress =  waitforkeypress();} 
while ((!(decimal_digit(keypress)))&& (keypress != '-'));            //(non_decimal_char(keypress));  //Not -,0,1,2,3,4,5,6,7,8 or 9
digits[0] = keypress;
//keypress_counter = 1;
Update_display();

while(1){
if ((keypress = wait_for_return_key())  =='\r')break;
if (decimal_digit(keypress)) 
{for(int n = 3; n>=1; n--)
digits[n] = digits[n-1];              
digits[0] = keypress;
//keypress_counter += 1;
Update_display();}}

cr_keypress = 1;
active_transaction = 1;
Update_display();
while (active_transaction);
cr_keypress = 0;


TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
while (!(TWCR & (1 << TWINT)));
sendHex(16,TWSR);
number =  byte(receive_byte_with_Ack());
number = (number << 8) + byte(receive_byte_with_Ack());
number = (number << 8) + byte(receive_byte_with_Ack());
number = (number << 8) + byte(receive_byte_with_Nack());
TWCR = (1 << TWINT);
//sendHex(10, number);
return number;}
/************************************************************************/





 
char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}

char wait_for_return_key(void){  
char keypress,temp;
keypress = waitforkeypress();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = receiveChar();}keypress = '\r';}
return keypress;}

 
