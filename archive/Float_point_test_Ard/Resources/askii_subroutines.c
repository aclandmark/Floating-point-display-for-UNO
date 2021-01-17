
#include <math.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

void Num_to_PC(char, long);
void SBtoAL(char*, long, char);
void NumericString_to_PC(char* );
void Char_to_PC(char);
float Float_from_KBD(char*);
char decimal_digit (char);
char wait_for_return_key(void);
char isCharavailable (int );
char receiveChar(void);
void sendStringF(const char*);
void sendStringV(char*);
void sendChar(char );
char waitforkeypress(void);





void Num_to_PC(char radix, long long_num){
char array[12];              //Long has 10 chars + sign + null terminator  
SBtoAL(array, long_num, radix);     //calls the Binary to askii subroutine
NumericString_to_PC(array);}        //Prints characters in reverse order

/*********************************************************************/
void SBtoAL(char array[], long num, char radix){          //Signed Binary to Askii
int m=0;
long sign;

if (num == 0x80000000){
switch(radix){
case 10: array[0] = '8';array[1] = '4'; array[2] = '6';   //0x80000000 * -1 = 0
array[3] = '3';array[4] = '8';array[5] = '4'; array[6] = '7';
array[7] = '4';array[8] = '1';array[9] = '2';
array[10] = '-'; array[11] = '\0'; break;

case 16: array[0] = '0';array[1] = '0'; array[2] = '0';   //0x80000000 * -1 = 0
array[3] = '0';array[4] = '0';array[5] = '0'; array[6] = '0';
array[7] = '8';array[8] = '-';array[9] = '\0';
array[10] = '\0'; array[11] = '\0'; break; } return;}

for (int n = 0; n <=11; n++)array[n] = 0; 
if ((sign = num) < 0) num = num * (-1);

do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);
if (sign < 0) {array[m] = '-';m++;}}


/*********************************************************************/
void NumericString_to_PC(char* s){          
int n=0;
while (s[n] != '\0')n++;              //scroll to end of string counting the number of characters
for(int m = n; m; m--)Char_to_PC(*(s + m-1));}    //print last character first


void Char_to_PC(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}



/*********************************************************************/
float Float_from_KBD(char digits[]){                                   //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress, flt_array[20];
float f_number;

for(int n = 0; n<=15; n++) digits[n] = 0;                            //Clear the buffer used to the string
do
{keypress =  waitforkeypress();} 
while ((!(decimal_digit(keypress)))
&& (keypress != '-')
&& (keypress != '.'));

digits[0] = keypress;
while(1){
if ((keypress = wait_for_return_key())  =='\r')break;               //Detect return key press (i.e \r or\r\n)
if ((decimal_digit(keypress)) || (keypress == '.')
|| (keypress == '\b')|| (keypress == '-')
|| (keypress == 'E'))

{
for(int n = 15; n>=1; n--)                                          //Shift display for each new keypress except '.'
digits[n] = digits[n-1];
digits[0] = keypress;}  }                                            //Add new keypress           
for(int m = 0; m <= 15; m++)
flt_array[m] = digits[15-m];

while (!(flt_array[0])) for(int m = 0; m <= 15; m++)flt_array[m] = flt_array[m+1];
f_number = atof(flt_array);
return f_number;}



/************************************************************************************/
char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}

char wait_for_return_key(void){  
char keypress,temp;
keypress = waitforkeypress();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = receiveChar();}keypress = '\r';}
return keypress;}

char isCharavailable (int m){int n = 0;    
while (!(UCSR0A & (1 << RXC0))){n++;
if (n>4000) {m--;n = 0;}if (m == 0)return 0;}
return 1;}

char receiveChar(void)
{return UDR0;}



void sendStringF(const char s[]){
  int i = 0;
  while(i < 200){
    if(s[i] == '\0') break;
  sendChar(s[i++]);} }

  void sendStringV(char s[]){
  int i = 0;
  while(i < 200){
    if(s[i] == '\0') break;
  sendChar(s[i++]);} }
  
void sendChar(char data){
  while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}

char waitforkeypress(void){
while (!(UCSR0A & (1 << RXC0)));
return UDR0;}




