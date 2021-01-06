//DEVELOPMENT OF UNO SLAVE TWI INTERFACE


#include "UNO_AVR_programmer.h"
#define Version "UNO_programmer_V2.5\r\n" 

void Num_from_KBD(unsigned char *);
void int_num_to_display(void);
void float_string_to_display(void);
void int_string_to_display(void);
signed char ftoa(float, char*, int);
void sendStringV(char*);



int main (void){ 
char str[10];
int afterpoint = 4;
 signed char expt=0;
//float float_num;

setup_328_HW;                                                     //see "Resources\ATMEGA_Programmer.h"
char counter, keypress;

sei();
TWAR = 0x06;
Reset_H;                                                          //Put target in reset state to dissable UART
Number = 23456789;
int_num_to_display();



while(1){
do{sendString("s  ");} 
while((isCharavailable(255) == 0));                                   //User prompt 
if(receiveChar() == 's')break;}

/****************************Test UNO float to askii subroutines********************************************************/
/*sendString("\r\nFP_num_test\t");                                                     
strcpy(str, "1875.725");                                                //Type and reat number string
float_num = atof(str);                                                     //ASKII to float library function
//float_num = float_num;  */                                                  //A bit of arithmetic
//ftoaSpecial(13579, str, 5);                                                   //Float to askii (non-library function)
//sendString(str); 

/****************************Test UNO integer number to display**********************************************************/
sendString("\r\nUNO sends ints. AK.\r\n");
waitforkeypress();
Number = 98765432;
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

while(1){

sendString("\r\nFP_num?: Then x to double, y to escape, AOK to halve\r\n\r\n");
float_num = Float_from_KBD(data_buff);
while(1){
 expt =  ftoa(float_num, str, afterpoint); 
 
 sendStringV(un_rounded_array); 
 sendString("\t Round Number plus exponent\t"); 
sendStringV(str); if(expt){sendChar ('E');Num_to_PC(10, expt); }
 sendString("\r\n");
 keypress = waitforkeypress();
 
 if( keypress == 'y')break;
   if( keypress == 'x'){ if (expt <= 36)float_num = float_num *2;
   else float_num = float_num /2;}
   else {if (expt >= -44)float_num = float_num/2;  
   else float_num = float_num *2;}
   float_num_to_display();                                 
      }  }


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
      
      
      Char_ptr = (char*)&float_num;
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







      void sendStringV(char s[]){
  int i = 0;
  while(i < 200){
    if(s[i] == '\0') break;
  sendChar(s[i++]);} }





  signed char ftoaSpecial(float Fnum, char FP_string[], int afterpoint){
  long ipart, Fnum_int;
  char sign = '+';
  signed char expt;
  //expt = 0;
  if (Fnum < 0){sign = '-'; Fnum *= (-1);}                                //Convert negative numbers to positive ones and set the sign character
  
  for(int m = 0; m <= 15; m++) FP_string[m] = 0;                          //Clear the floating point array
  
  Fnum_int = (long)Fnum;                                                  //Obtain integer part of array
  if (Fnum_int < 10)afterpoint = 5;                                       //Number of decimal places is matched to display length
  if ((Fnum_int >= 10) && (Fnum_int < 100))afterpoint = 4;
  if ((Fnum_int >= 100) && (Fnum_int < 1000))afterpoint = 3;
  if ((Fnum_int >= 1000) && (Fnum_int < 10000))afterpoint = 2;
  
  expt = 0;                                                                 //Convert very large and small numbers to scientific form
  if (Fnum  >= 10000) { while (Fnum >= 10)
  {Fnum /= 10; expt += 1;}afterpoint = 5;}
  if (Fnum < 0.01) {while (Fnum < 1){Fnum *= 10; expt -= 1;}}
                                          
  ipart = (long)Fnum;                                                       //Obtain integer part of FP number
  float fpart = Fnum - (float)ipart;                                        //Obtain floating part
  long i = longToStr(ipart, FP_string, 0);                                  //Convert integer part to string

  if (afterpoint != 0){                                                     //Add Decimal part to the string
    FP_string[i] = '.';
    fpart = fpart * pow(10,afterpoint);
  longToStr((long)fpart, FP_string + i + 1, afterpoint);}

 Print_unrounded(FP_string, sign);
  expt = Round_for_Display(FP_string, sign, expt);

return expt;}
