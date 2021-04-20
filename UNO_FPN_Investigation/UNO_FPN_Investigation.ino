

#include "UNO_FPN_header.h"

int main (void){ 
long Int_num;
float float_num_1, float_num_2;
char input_mode, op;                                                  //Integer or float input from keyboard or IO
//char str[12];



setup_328_HW;                                                     //see header file"
sei();
input_mode = 0; 

f_num_ptr = &float_num_1;
float_num_1 = *f_num_ptr;
char_ptr_2 = (char*)&float_num_1;

//for (int m = 0; m <= 11; m++)str[m] = 0;
//strcpy(str, "0.5");

while(1){
do{Serial.write("F/I    ");                                       //User prompt 
if(switch_1_down){input_mode = 'f'; num_type = 'F'; break;} 
if(switch_2_down){input_mode = 'i'; num_type = 'I'; break;}}
while((isCharavailable(127) == 0));                           

if((input_mode == 'f') || (input_mode == 'i'))break;
switch (Serial.read()){
case 'F': input_mode = 'F'; num_type = 'F'; break;
case 'I': input_mode = 'I'; num_type = 'I';  break;
default: input_mode = 0; break;}
if (!(input_mode)) continue; else break;}

while(1){

if(input_mode == 'F'){Serial.write("\r\nFPN from keyboard");
float_num_1 = Float_from_KBD();}
//if(input_mode == 'f')float_num_1 = FPN_number_from_IO();
Serial.write("\r\n");
prt_FPN_in_binary;


ftoaL(float_num_1,display_array);
Serial.write("\r\n");
pcb_display_to_PC(display_array);}

/**************************************************************************************************************/
for(int m = 0; m<=100; m++)
  {if (!m)Serial.write("\r\nEnter zero to exit \
or +,-,*, followed by new number. /\r\n");
while (!(Serial.available())); 
op = Serial.read();
if(op == '0')break;
float_num_2 = Float_from_KBD();
//if(input_mode == 'f')float_num_2 = FPN_number_from_IO();

switch (op){                                                          //Do some integer arithmetic
  case '+': float_num_1 = (float_num_1 + float_num_2);break;
  case '-': float_num_1 = (float_num_1 - float_num_2);break;
  case '*': float_num_1 = (float_num_1 * float_num_2);break;
  case '/': float_num_1 = (float_num_1 / float_num_2);break;
  case '^': float_num_1 = pow(float_num_1, float_num_2);break;
  default: break;}

//float_num_to_display(float_num_1);
ftoaL(float_num_1,display_array);
pcb_display_to_PC(display_array);
}

SW_reset;}
/*************************************************************************************************************/

/*
ftoaL(float_num_1*2.0,display_array);
Serial.write('C');
pcb_display_to_PC(display_array);

waitforkeypress();

SW_reset;}*/
