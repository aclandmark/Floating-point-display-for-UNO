
#include "UNO_template_V2.h"
#define Version "UNO template for floating point display Version 2\r\n" 



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
 
 if(keypress == 'y')break;
   if( keypress == 'x') float_num = float_num * (-2.75);
   else float_num = float_num/2.75;  
   float_num_to_display(float_num);}  
break;}}

return 1;}
