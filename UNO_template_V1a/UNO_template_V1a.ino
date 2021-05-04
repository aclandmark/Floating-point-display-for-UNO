
#include "UNO_template_V1a.h"


int main (void){ 
long Int_num;
float float_num_1, float_num_2;
char input_mode, op;                                               //Integer or float input from keyboard or IO

//eeprom_write_byte((uint8_t*)0x3FB, 0xFF);

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
if (eeprom_read_byte((uint8_t*)0x3FB) & 4){
if(input_mode == 'F'){Serial.write("\r\nFPN from keyboard");
float_num_1 = Float_from_KBD();}
if(input_mode == 'f')float_num_1 = FPN_number_from_IO();}
else {
  float_num_1 = float_num_from_eepom();
float_num_to_display(float_num_1);
eeprom_write_byte((uint8_t*)(0x3FB),
 eeprom_read_byte((uint8_t*)(0x3FB)) | 4);

}

for(int m = 0; m<=100; m++)
  {if (!m)Serial.write("\r\nEnter zero to exit \
or +,-,* or ^ followed by new number. /\r\n");
while (!(Serial.available())); 
op = Serial.read();
if(op == '0')break;
if(input_mode == 'F')float_num_2 = Float_from_KBD();
if(input_mode == 'f')float_num_2 = FPN_number_from_IO();

switch (op){                                                          //Do some arithmetic
  case '+': float_num_1 = (float_num_1 + float_num_2);break;
  case '-': float_num_1 = (float_num_1 - float_num_2);break;
  case '*': float_num_1 = (float_num_1 * float_num_2);break;
  case '/': float_num_1 = (float_num_1 / float_num_2);break;
  case '^': float_num_1 = pow(float_num_1, float_num_2);break;
  default: break;}

float_num_to_display(float_num_1);}}


if (num_type == 'I'){
if(input_mode == 'I'){Serial.write("\r\nInt num from keyboard");
Int_num = Int_from_KBD();}
if(input_mode == 'i')Int_num = Int_number_from_IO();

do{Int_num = Int_num/2*3;waitforkeypress();}while (!(int_num_to_display(Int_num)));
}

SW_reset;}
