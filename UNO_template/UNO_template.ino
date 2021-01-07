
#include "UNO_template.h"
#define Version "UNO_programmer_V2.5\r\n" 

void Num_from_KBD(unsigned char *);
char int_num_to_display(long);
void float_num_to_display(float);
void sendStringV(char*);

int EE_size = 0x200;

int main (void){ 

setup_328_HW;                                                     //see "Resources\ATMEGA_Programmer.h"
char keypress;

long Int_num_1, Int_num_2;
float float_num;


sei();
TWAR = 0x06;

Int_num_1 = 23456789;
int_num_to_display(Int_num_1);

while(1){
do{sendString("s  ");} 
while((isCharavailable(255) == 0));                                   //User prompt 
if(receiveChar() == 's')break;}


while (1){
sendString("\r\nEnter I or F followed by number: Remember cr!\r\n");

switch (waitforkeypress()){
case 'I':Int_num_1 = Num_from_KBD(data_buff);                                      //User entry terminates with a carriage return key press

for(int m = 0; m<=100; m++)
  {if (!m)sendString("\r\nEnter zero to exit or new number followed by +,-,*,/\r\n");
  if(!(Int_num_2 = Num_from_KBD(data_buff)))break;

  keypress = waitforkeypress();
switch (keypress){
  case '+': Int_num_1 = (Int_num_1 + Int_num_2);break;
  case '-': Int_num_1 = (Int_num_1 - Int_num_2);break;
  case '*': Int_num_1 = (Int_num_1 * Int_num_2);break;
  case '/': Int_num_1 = (Int_num_1 / Int_num_2);break;
  default: break;}

if (int_num_to_display(Int_num_1))sendString("Overflows: Repeat operation or exit");}
break;

case 'F': 

float_num = Float_from_KBD(data_buff);
sendString("\r\nPress x or AOK (y to exit)\r\n\r\n");
while(1){
keypress = waitforkeypress();
 
 if(keypress == 'y')break;
   if( keypress == 'x') float_num = float_num *2.75;
   else float_num = float_num/2.75;  
   float_num_to_display(float_num);}  
break;}}

return 1;}








      void sendStringV(char s[]){
  int i = 0;
  while(i < 200){
    if(s[i] == '\0') break;
  sendChar(s[i++]);} }
