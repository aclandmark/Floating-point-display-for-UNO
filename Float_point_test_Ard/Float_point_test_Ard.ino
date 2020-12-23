//C program for implementation of ftoa()
#include <math.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


#define wdr()  __asm__ __volatile__("wdr")
#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}
  
  #define setup_HW \
  CLKPR = (1 << CLKPCE);\
CLKPR = (1 << CLKPS0);\
  setup_watchdog;\
  USART_init(0,25);
  
void sendStringF(const char*);
void sendStringV(char*);
void sendChar(const char);
char waitforkeypress(void);



int main()
{char res[20];
  float n ;
  long n_int;
  int afterpoint = 4;
  int LSB_ptr;
char expt=0;
  
  setup_HW;
  waitforkeypress();waitforkeypress();waitforkeypress();
  sendStringF("Enter FPN numbers\r\n");


   n = (Float_from_KBD(res));
   
   while(1){
    for (int m = 0; m <= 19; m++)res[m] = 0;

n_int = (long)n;
if(n_int < 0) n_int = n_int * (-1);

if (n_int < 10)afterpoint = 5;
if ((n_int >= 10) && (n_int < 100))afterpoint = 4;
if ((n_int >= 100) && (n_int < 1000))afterpoint = 3;
if ((n_int >= 1000) && (n_int < 10000))afterpoint = 2;
if ((n_int >= 10000) && (n_int < 100000))afterpoint = 1;

expt = 0;
if (n  >= 10000) { while (n > 10){n /= 10; expt += 1;}afterpoint = 5;}
if (n < 0.001) {while (n < 1){n *= 10; expt -= 1;}}

  ftoa(n, res, afterpoint); 
   
   for (int m = 19; m; m--)                                           //Remove trailing zeros if the is a decimal point
   {if (res[m] == 0) continue; 
   if (res [m] == '0') res[m] = 0;
   else break;}
  
 sendStringV(res); if(expt){sendChar ('E');Num_to_PC(10, expt); }
 sendStringF("\t Round Number:AOK\t");
  
  /****************Rounding Fn*******************************/
  for (int m = 19; m; m--)                                                //start rounding with the least significant digit
   {LSB_ptr = m;
    if (res[m] == 0) continue; 
   else  break;}

if (res[LSB_ptr] >= '5'){res[LSB_ptr--] = 0; if(res[LSB_ptr] == '.')LSB_ptr -= 1;
res[LSB_ptr] += 1;
while (res[LSB_ptr] == ':'){res[LSB_ptr--] = 0;
if (res[LSB_ptr] == '.')LSB_ptr -= 1;
res[LSB_ptr] += 1;}}
/**********************************************************/
  
  
  sendStringV(res);
  sendStringF("\r\n");
   if( waitforkeypress() == 'x')n = (n * ( pow (10,expt)))/2;
   else  n = (n * ( pow (10,expt)))*2;
   //waitforkeypress();
   }
  return 0;}



/************************************************************************************/
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
/************************************************************************************/




void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
  UCSR0B = 0;
  UBRR0H = UBRROH_N;
  UBRR0L = UBRR0L_N;
  UCSR0A = (1 << U2X0);
  UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);}

/**********************************************************************************************/
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


void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    {   temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; }} 


long longToStr(long x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    {   str[i++] = (x%10) + '0'; 
        x = x/10; } 
  
      while (i < d) 
        str[i++] = '0'; 
     reverse(str, i); 
    str[i] = '\0'; 
    return i; } 


void ftoa(float Fnum, char FP_string[], int afterpoint){
    long ipart;
    char sign = '+';
    
    for(int m = 0; m <= 16; m++) FP_string[m] = 0;
    
    if (Fnum < 0){sign = '-'; Fnum *= (-1);}
    
    ipart = (long)Fnum;
    float fpart = Fnum - (float)ipart;
    long i = longToStr(ipart, FP_string, 0);

    if (afterpoint != 0){
      FP_string[i] = '.';
      fpart = fpart * pow(10,afterpoint);
    longToStr((long)fpart, FP_string + i + 1, afterpoint);}
    
    if(sign == '-'){for(int m = 0; m <= 15; m++)FP_string[16-m] = FP_string[15-m];
    FP_string[0] = '-'; 
    }}



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
