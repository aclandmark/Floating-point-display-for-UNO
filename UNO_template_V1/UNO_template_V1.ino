
#include "UNO_template_V1.h"
#define Version "UNO template for floating point display Version 1\r\n" 

void Num_from_KBD(unsigned char *);
char int_num_to_display(long);
void float_num_to_display(float);
void sendStringV(char*);

int EE_size = 0x200;

int main (void){ 


char keypress;

long Int_num_1, Int_num_2;
float float_num;

setup_328_HW;                                                     //see "Resources\ATMEGA_Programmer.h"

sei();
TWAR = 0x06;

Int_num_1 = 23456789;
int_num_to_display(Int_num_1);

while(1){
do{Text_to_PC("s  ");} 
while((isCharavailable(255) == 0));                                   //User prompt 
if(receiveChar() == 's')break;}

newline();newline();
Text_to_PC(Version);

while (1){
Text_to_PC("\r\nEnter I or F followed by number: Remember cr!\r\n");

switch (waitforkeypress()){
case 'I':Int_num_1 = Num_from_KBD(data_buff);                                      //User entry terminates with a carriage return key press

for(int m = 0; m<=100; m++)
  {if (!m)Text_to_PC("\r\nEnter zero to exit or new number followed by +,-,*,/\r\n");
  if(!(Int_num_2 = Num_from_KBD(data_buff)))break;

  keypress = waitforkeypress();
switch (keypress){
  case '+': Int_num_1 = (Int_num_1 + Int_num_2);break;
  case '-': Int_num_1 = (Int_num_1 - Int_num_2);break;
  case '*': Int_num_1 = (Int_num_1 * Int_num_2);break;
  case '/': Int_num_1 = (Int_num_1 / Int_num_2);break;
  default: break;}

if (int_num_to_display(Int_num_1))Text_to_PC("Overflows: Repeat operation or exit");}
break;

case 'F': 

float_num = Float_from_KBD(data_buff);
Text_to_PC("\r\nPress x or AOK (y to exit)\r\n\r\n");
while(1){
keypress = waitforkeypress();
 
 if(keypress == 'y')break;
   if( keypress == 'x') float_num = float_num *2.75;
   else float_num = float_num/2.75;  
   float_num_to_display(float_num);}  
break;}}

return 1;}
