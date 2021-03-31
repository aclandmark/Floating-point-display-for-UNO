
#include "UNO_template_V2.h"
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
float_num_1 = Float_from_KBD(data_buff);

for(int m = 0; m<=100; m++)
  {if (!m)Serial.write("\r\nEnter zero to exit\
or +,-,*, followed by new number. /\r\n");
while (!(Serial.available())); 
op = Serial.read();
if(op == '0')break;
float_num_2 = Float_from_KBD(data_buff);

switch (op){                                                 //Do some integer arithmetic
  case '+': float_num_1 = (float_num_1 + float_num_2);break;
  case '-': float_num_1 = (float_num_1 - float_num_2);break;
  case '*': float_num_1 = (float_num_1 * float_num_2);break;
  case '/': float_num_1 = (float_num_1 / float_num_2);break;
  default: break;}

float_num_to_display(float_num_1);}}


if(mode == 'I'){Serial.write("\r\nInteger fron keyboard");
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
Serial.write("Overflows: Repeat operation or exit");}}


if(mode == 'i'){Serial.write("\r\nInteger from switches\r\n");}

if(mode == 'f'){Serial.write("\r\nFPN from switches\r\n");}


SW_reset;}
