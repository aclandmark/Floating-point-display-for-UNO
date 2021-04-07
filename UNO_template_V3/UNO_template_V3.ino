
#include "UNO_template_V3.h"

#define Version "UNO template for floating point display Version 2\r\n" 



int main (void){ 
long Int_num_1, Int_num_2;
float float_num_1, float_num_2;
char keypress, op, mode;                                            //+,-,* or 


setup_328_HW;                                                 //see header file"
sei();
flash_on_FPN_overflow;

while(1){
do{Serial.write("F/I    ");} 
while((isCharavailable(255) == 0));                           //User prompt 

switch (Serial.read()){
case 'F': mode = 'F'; break;
case 'f': mode = 'f'; break;
case 'I': mode = 'I'; break;
case 'i': mode = 'i'; break;
default: mode = 0; break;}

if (!(mode)) continue; else break;}

Serial.write("\r\n\r\n");
Serial.write(Version);

if(mode == 'F'){Serial.write("\r\nFPN fron keyboard");
float_num_1 = Float_from_KBD();

for(int m = 0; m<=100; m++)
  {if (!m)Serial.write("\r\nEnter zero to exit\
or +,-,*, followed by new number. /\r\n");
while (!(Serial.available())); 
op = Serial.read();
if(op == '0')break;
float_num_2 = Float_from_KBD();

switch (op){                                                 //Do some integer arithmetic
  case '+': float_num_1 = (float_num_1 + float_num_2);break;
  case '-': float_num_1 = (float_num_1 - float_num_2);break;
  case '*': float_num_1 = (float_num_1 * float_num_2);break;
  case '/': float_num_1 = (float_num_1 / float_num_2);break;
  case '^': float_num_1 = pow(float_num_1, float_num_2);break;
  default: break;}

float_num_to_display(float_num_1);}}


if(mode == 'I'){Serial.write("\r\nInteger fron keyboard");
Int_num_1 = Num_from_KBD();                          //User entry terminates with a carriage return key press

for(int m = 0; m<=100; m++)
  {if (!m)Serial.write("\r\nEnter zero to exit\
or +,-,*, followed by new number. /\r\n");
while (!(Serial.available())); 
op = Serial.read();
if(op == '0')break;
Int_num_2 = Num_from_KBD();

switch (op){                                                 //Do some integer arithmetic
  case '+': Int_num_1 = (Int_num_1 + Int_num_2);break;
  case '-': Int_num_1 = (Int_num_1 - Int_num_2);break;
  case '*': Int_num_1 = (Int_num_1 * Int_num_2);break;
  case '/': Int_num_1 = (Int_num_1 / Int_num_2);break;
  default: break;}

if (int_num_to_display(Int_num_1))                           //Returns 1 if display overflow occurs
Serial.write("Overflows: Repeat operation or exit");}}


if(mode == 'i'){Serial.write("\r\nInteger from switches\r\n");}

if(mode == 'f'){Serial.write("\r\nFPN from switches\r\n");
Serial.write("Press: sw_1 to populate digit_0, sw3 to shift the display left\r\n\
sw_2 to enter the number and sw1 to clear the display.\r\n\
Note: display flashes to indicate number has been entered.\r\n");

float_num_1 = FPN_number_from_IO();
waitforkeypress();
float_num_to_display(float_num_1*2.0);

while(1);
}
SW_reset;}
