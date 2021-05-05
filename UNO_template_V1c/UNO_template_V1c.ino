
#include "UNO_template_V1c.h"
#include "Project_header.h"


void blank_dislay(void);
char data_from_IO(void);
char data_from_KBD(void);


#define IO        '1'
#define FPN       '2'
#define int_num   '3'


int main (void){ 
long Int_num;
float float_num_1, float_num_2;
char input_mode, op;                                               //Integer or float input from keyboard or IO

setup_328_HW;                                                     //see header file"
sei();
input_mode = 0; 



if(ongoing_OVF)                                                   //If a FP overflow has occurred, restore the display
{float_num_1 = float_num_from_eepom(OVF_recovery);
float_num_to_display(float_num_1);}

while(1){
do{Serial.write("F/I    ");                                       //User prompt 
if(switch_1_down)                                                 //Data to be acquired from the IO user switches
{input_mode = IO; num_type = FPN; break;} 
if(switch_2_down)
{input_mode = IO; num_type = int_num; break;}}
while((isCharavailable(127) == 0));    

if(input_mode == IO)break;
switch (Serial.read()){                                           //Data to be acquired from the PC keyboard
case 'F': num_type = FPN; break;
case 'I': num_type = int_num;  break;
default: num_type = 0; break;}
if (!(num_type)); else break;}


if (num_type == int_num)                                          //Integer number operations
{clear_all_recovery_flags;                                        //Data recovery not implemented for integer numbers
blank_dislay();
  if(input_mode == IO)Int_num = Int_number_from_IO();
else {Serial.write("\r\nInt num from keyboard");
Int_num = Int_from_KBD();}
do{Int_num = Int_num/2*3;waitforkeypress();}                      //Arithmetic to test string to num conversion
while (!(int_num_to_display(Int_num)));}

else{                                                             //FPN operations
if(normal_data_entry){                                            //No data recovery operation in progress
if(input_mode == IO)float_num_1 = FPN_number_from_IO();
else
{Serial.write("\r\nFPN from keyboard");
float_num_1 = Float_from_KBD();}}

else{blank_dislay();                                             //Continue after data recovery is complete
float_num_1 = float_num_from_eepom(OVF_recovery);                //Flash the display
float_num_to_display(float_num_1);
clear_OVF_flag;}                                                


for(int m = 0; m<=100; m++)                                       //Do some arithmetic
  {if (!m)Serial.write("\r\nEnter zero to exit \
or +,-,* or ^ followed by new number. /\r\n");

if(input_mode == IO)op = data_from_IO();
else
op = data_from_KBD();

if((op == '0') || (op == 6))break;
if(input_mode == IO)float_num_2 = FPN_number_from_IO();
else float_num_2 = Float_from_KBD();


switch (op){                                                          //Do some arithmetic
  case 1:
  case '+': float_num_1 = (float_num_1 + float_num_2);break;
  case 2:
  case '-': float_num_1 = (float_num_1 - float_num_2);break;
  case 3:
  case '*': float_num_1 = (float_num_1 * float_num_2);break;
  case 4:
  case '/': float_num_1 = (float_num_1 / float_num_2);break;
  case 5:
  case '^': float_num_1 = pow(float_num_1, float_num_2);break;
  default: break;}

float_num_to_display(float_num_1);}}
SW_reset;}

 


void blank_dislay(void){
  for(int m = 0; m <= 7; m++)data_buff[m]=0;
  cr_keypress = 0;
  int_string_to_display();
  _delay_ms(250);}

char data_from_KBD(void){
while (!(Serial.available()));
return Serial.read();}

char data_from_IO(void){
  char op = 0;
  while switch_1_up;
  while(1){while switch_2_up;
  while switch_2_down;
  op += 1;
  if (switch_1_up)break;}
  
  return op;
  }
