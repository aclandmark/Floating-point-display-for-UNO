
#include "UNO_template_V2.h"
#define Version "UNO template for floating point display Version 2\r\n" 

float FPN_Arithmetic (float, char, float);
void Timer_T1_sub_with_interrupt(char, unsigned int);
void Shut_down_Timer_T1_sub(void);
signed char ftoaL(float, char*);

#define T1_delay_100ms 4, 59286
volatile char SREG_BKP;
char FP_string[12];
int expt;
char expt_string[6];
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
   if( keypress == 'x') float_num = FPN_Arithmetic (float_num, '*', (-2.75));
   else float_num = FPN_Arithmetic (float_num, '/', 2.75);  
   float_num_to_display(float_num);
   expt = ftoaL(float_num, FP_string);
   //itoa(expt,expt_string, 10 );
   //Serial.write(FP_string); Serial.write('\t');Serial.write(expt_string);Serial.write("\r\n");
   }  
break;}}

return 1;}


float FPN_Arithmetic (float num_1, char op, float num_2){
  float res;
  
switch (op){
case '+': res = num_1 + num_2; break;
case '-': res = num_1 - num_2; break;
case '*': res = num_1 * num_2; break;
case '/': res = num_1 / num_2; break;
}
return res;}

void Timer_T1_sub_with_interrupt(char Counter_speed, unsigned int Start_point){ 
  
SREG_BKP = SREG;
sei();
TIMSK1 |= (1 << TOIE1);
TCNT1 = Start_point;
TCCR1B = Counter_speed;}

void Shut_down_Timer_T1_sub(void){
TCCR1B = 0;
TIMSK1 &= (~(1 << TOIE1));
TCNT1 = 0;}

ISR (TIMER1_OVF_vect){Shut_down_Timer_T1_sub();
SW_reset;}



signed char ftoaL(float Fnum, char FP_string[]){                    //See "https://www.geeksforgeeks.org/convert-floating-point-number-string/"
  int afterpoint = 0;
  long ipart, Fnum_int;
  char sign = '+';
  signed char expt;
  
  if (Fnum < 0){sign = '-'; Fnum *= (-1);}                  //Convert negative numbers to positive ones and set the sign character
  
  for(int m = 0; m <= 11; m++) FP_string[m] = 0;                //Clear the floating point array


  Timer_T1_sub_with_interrupt(T1_delay_100ms);
  Fnum_int = (long)Fnum;                                //Obtain integer part of the number
  if (Fnum_int < 10)afterpoint = 5;                     //Number of decimal places is matched to display length
  if ((Fnum_int >= 10) && (Fnum_int < 100))afterpoint = 4;
  if ((Fnum_int >= 100) && (Fnum_int < 1000))afterpoint = 3;
  if ((Fnum_int >= 1000) && (Fnum_int < 10000))afterpoint = 2;
  
  expt = 0;                                 //Convert large and small numbers to scientific form
  if (Fnum  >= 10000) {while (Fnum >= 10)
  {Fnum /= 10; expt += 1;}afterpoint = 5;}
  
  if(Fnum < 0.01) {while (Fnum < 1){Fnum *= 10; expt -= 1;}}
  
  Shut_down_Timer_T1_sub();
  
  /* Serial.write('\t');                                     
  ipart = (long)Fnum;                             //Obtain integer part of FP number
  float fpart = Fnum - (float)ipart;                      //Obtain floating part
  long i = longToStr(ipart, FP_string, 0);                  //Convert integer part to string

  if (afterpoint != 0){                           //Add Decimal part to the string
    FP_string[i] = '.';
    fpart = fpart * pow(10,afterpoint);
  longToStr((long)fpart, FP_string + i + 1, afterpoint);}
  
  if(sign == '-')
  {for(int m = 0; m <= 10; m++)FP_string[11-m] = FP_string[10-m]; 
  FP_string[0] = '-';}

  //for(int m = 0; m <= 15; m++){if (expt & (1 << (15-m))) Serial.write('1'); else Serial.write('0');}
  return expt;*/
  return 1;
  }



/***************************************************************************************************************************************/
/*long longToStr(long x, char str[], int d)
{
  int i = 0;
  while (x)
  {   str[i++] = (x%10) + '0';
  x = x/10; }
  
  while (i < d)
  str[i++] = '0';
  reverse(str, i);
  str[i] = '\0';
return i; }*/


/***************************************************************************************************************************************/
/*void reverse(char *str, int len)
{
  int i=0, j=len-1, temp;
  while (i<j)
  {   temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  i++; j--; }}*/

  
