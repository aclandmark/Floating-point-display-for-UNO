
#include "UNO_template_V5.h"

#define Version "UNO template for floating point display Version 4\r\n" 



int main (void){ 
long Int_num_1, Int_num_2;
float float_num_1, float_num_2;
char keypress, op, mode;                                      //+,-,* or /


setup_328_HW;                                                 //see header file"
sei();
flash_on_FPN_overflow;
mode = 0; 

while(1){
do{Serial.write("F/I    ");
if(switch_1_down){mode = 'f'; num_type = 'F'; break;} 
if(switch_2_down){mode = 'i'; num_type = 'I'; break;}}


while((isCharavailable(127) == 0));                           //User prompt 
if((mode == 'f') || (mode == 'i'))break;
switch (Serial.read()){
case 'F': mode = 'F'; num_type = 'F'; break;
case 'I': mode = 'I'; num_type = 'I';  break;
default: mode = 0; break;}
if (!(mode)) continue; else break;}

if (num_type == 'F'){
if(mode == 'f')float_num_1 = FPN_number_from_IO();
if(mode == 'F'){Serial.write("\r\nFPN from keyboard");
float_num_1 = Float_from_KBD();
float_num_to_display(float_num_1);}

FPN_arithmrtic;}


if (num_type == 'I'){
if(mode == 'i')Int_num_1 = Int_number_from_IO();
if(mode == 'I'){Serial.write("\r\nInt from keyboard");
Int_num_1 = Num_from_KBD();}
int_num_to_display(Int_num_1 * 2);
}
}
