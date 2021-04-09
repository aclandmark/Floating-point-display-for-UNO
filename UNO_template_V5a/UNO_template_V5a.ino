
#include "UNO_template_V5a.h"

#define Version "UNO template for floating point display Version 4\r\n" 



int main (void){ 
long Int_num;
float float_num;
char input_mode;                                                  //Integer or float input from keyboard or IO


setup_328_HW;                                                     //see header file"
sei();
input_mode = 0; 

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

if (num_type == 'F'){
if(input_mode == 'f')float_num = FPN_number_from_IO();
if(input_mode == 'F'){Serial.write("\r\nFPN from keyboard");
float_num = Float_from_KBD();
float_num_to_display(float_num);}

FPN_arithmrtic;}


if (num_type == 'I'){
if(input_mode == 'i')Int_num = Int_number_from_IO();
if(input_mode == 'I'){Serial.write("\r\nInt from keyboard");
Int_num = Int_from_KBD();}

INT_arithmtic;

}
}
