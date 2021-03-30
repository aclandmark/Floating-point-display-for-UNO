
#include "UNO_template_V2.h"
#define Version "UNO template for floating point display Version 2\r\n" 

float FPN_Arithmetic (float, char, float);
void Timer_T1_sub_with_interrupt(char, unsigned int);
void Shut_down_Timer_T1_sub(void);
void check_for_OVF(float);

#define wd_timer_off \
wdr();\
MCUSR &= (~(1 << WDRF));\
WDTCSR |= (1<<WDCE) | (1<<WDE);\
WDTCSR = 0x00;

#define T1_delay_100ms 4, 59286
volatile char SREG_BKP;
char FP_string[12];
int expt;
char expt_string[6];
int main (void){ 


char keypress, op;                                            //+,-,* or 

long Int_num_1, Int_num_2;
float float_num;

setup_328_HW;                                                 //see header file"
sei();

Int_num_1 = 23456789;                                         //Initialise display
int_num_to_display(Int_num_1);


while(1){
do{Serial.write("s    ");} 
while((isCharavailable(255) == 0));                           //User prompt 
if(Serial.read() == 's')break;}

Serial.write("\r\n\r\n");
Serial.write(Version);

while (1){
Serial.write("\r\nEnter I or F followed\
by number: Remember cr!\r\n");
while (!(Serial.available())); 
keypress = Serial.read();
switch (keypress){
case 'I':

Int_num_1 = Num_from_KBD(data_buff);                          //User entry terminates with a carriage return key press

for(int m = 0; m<=100; m++)
  {if (!m)Serial.write("\r\nEnter zero to exit\
or +,-,*, followed by new number. /\r\n");
while (!(Serial.available())); 
op = Serial.read();
if(op == '0')break;
Int_num_2 = Num_from_KBD(data_buff);

switch (op){                                                 //Do some integer arithmetic
  case '+': Int_num_1 = (Int_num_1 + Int_num_2);break;
  case '-': Int_num_1 = (Int_num_1 - Int_num_2);break;
  case '*': Int_num_1 = (Int_num_1 * Int_num_2);break;
  case '/': Int_num_1 = (Int_num_1 / Int_num_2);break;
  default: break;}

if (int_num_to_display(Int_num_1))                           //Returns 1 if display overflow occurs
Serial.write("Overflows: Repeat operation or exit");}
break;

case 'F': 
float_num = Float_from_KBD(data_buff);
Serial.write("\r\nPress x or AOK (y to exit)\r\n\r\n");
while(1){
while (!(Serial.available())); 
keypress = Serial.read();
 
if(keypress == 'v')break;
if( keypress == 'w') float_num = float_num * 1e15;
   if( keypress == 'x') float_num = float_num / 1e15; 
   if( keypress == 'y') float_num = float_num * (-2.75);
   if( keypress == 'z') float_num = float_num / 2.75;
   float_num_to_display(float_num);
   check_for_OVF(float_num);
      }  
break;}}

return 1;}





void check_for_OVF(float Fnum){                    
  long  Fnum_int;
  char sign = '+';
    
  if (Fnum < 0){sign = '-'; Fnum *= (-1);}                  //Convert negative numbers to positive ones and set the sign character
  wdt_enable(WDTO_30MS);
  
  Fnum_int = (long)Fnum;                                    //Obtain integer part of the number
  if (Fnum  >= 10000) {while (Fnum >= 10)
  {Fnum /= 10; }}
  if(Fnum < 0.01) {while (Fnum < 1){Fnum *= 10; }}
  wd_timer_off;
  }
