
void Num_to_PC(char, long);
void SBtoAL(char*, long, char);
void NumericString_to_PC(char*);
void Char_to_PC(char);
long longToStr(long, char*, int);
void reverse(char *, int);
char decimal_digit (char);
char wait_for_return_key(void);
void int_string_to_display(void);
void float_string_to_display(void);
long Num_from_KBD(char*);
float Float_from_KBD(char *);

signed char ftoa(float, char*, int);
void Print_unrounded(char*, char);
void Round_for_Display(char*, char, signed char);







void Num_to_PC(char radix, long long_num){
char array[12];							//Long has 10 chars + sign + null terminator	
SBtoAL(array, long_num, radix);			//calls the Binary to askii subroutine
NumericString_to_PC(array);}				//Prints characters in reverse order

/*********************************************************************/
void SBtoAL(char array[], long num, char radix){					//Signed Binary to Askii
int m=0;
long sign;

if (num == 0x80000000){
switch(radix){
case 10: array[0] = '8';array[1] = '4'; array[2] = '6';		//0x80000000 * -1 = 0
array[3] = '3';array[4] = '8';array[5] = '4'; array[6] = '7';
array[7] = '4';array[8] = '1';array[9] = '2';
array[10] = '-'; array[11] = '\0'; break;

case 16: array[0] = '0';array[1] = '0'; array[2] = '0';		//0x80000000 * -1 = 0
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
while (s[n] != '\0')n++;							//scroll to end of string counting the number of characters
for(int m = n; m; m--)Char_to_PC(*(s + m-1));}		//print last character first


void Char_to_PC(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}


/***************************************************************************************************************************************/
signed char ftoa(float Fnum, char FP_string[], int afterpoint){
  long ipart, Fnum_int;
  char sign = '+';
  signed char expt;
  
  if (Fnum < 0){sign = '-'; Fnum *= (-1);}                                //Convert negative numbers to positive ones and set the sign character
  
  for(int m = 0; m <= 15; m++) FP_string[m] = 0;                          //Clear the floating point array
  
  Fnum_int = (long)Fnum;                                                  //Obtain integer part of array
  if(Fnum_int < 0) Fnum_int = Fnum_int * (-1);                            //Only used to set the number of decimal places 

  if (Fnum_int < 10)afterpoint = 5;                                       //Number of decimal places is matched to display length
  if ((Fnum_int >= 10) && (Fnum_int < 100))afterpoint = 4;
  if ((Fnum_int >= 100) && (Fnum_int < 1000))afterpoint = 3;
  if ((Fnum_int >= 1000) && (Fnum_int < 10000))afterpoint = 2;
  if ((Fnum_int >= 10000) && (Fnum_int < 100000))afterpoint = 1;

  expt = 0;                                                                 //Convert very large and small numbers to scientific form
  if (Fnum  >= 10000) { while (Fnum > 10)
  {Fnum /= 10; expt += 1;}afterpoint = 5;}
  if (Fnum < 0.01) {while (Fnum < 1){Fnum *= 10; expt -= 1;}}
  
                                        
  ipart = (long)Fnum;                                                       //Obtain integer part of FP number
  float fpart = Fnum - (float)ipart;                                        //Obtain floating part
  long i = longToStr(ipart, FP_string, 0);                                  //Convert integer part to string

  if (afterpoint != 0){                                                     //Add Decimal part to the string
    FP_string[i] = '.';
    fpart = fpart * pow(10,afterpoint);
  longToStr((long)fpart, FP_string + i + 1, afterpoint);}

  Print_unrounded(FP_string, sign);
  Round_for_Display(FP_string, sign, expt);

return expt;}


/**************************************************************************************************************************************/
void Print_unrounded(char *FP_string, char sign){
for(int m = 0; m <= 15; m++)un_rounded_array[m] = FP_string[m];
if(sign == '-')
    {for(int m = 0; m <= 14; m++)un_rounded_array[15-m] = un_rounded_array[14-m];               //For negative numbers shift the array once place to the right
    un_rounded_array[0] = '-';}}



/******************************************************************************************************/
void Round_for_Display(char* res, char sign, signed char expt){
   
    int LSB_ptr;

for (int m = 15; m; m--)                                                //Remove trailing zeros if the is a decimal point
   {if (res[m] == 0) continue; 
   if (res [m] == '0') res[m] = 0;
   else break;}

    
    for (int m = 15; m; m--)                                            //start rounding with the least significant digit
   {LSB_ptr = m;
    if (res[m] == 0) continue; 
   else  break;}

if (res[LSB_ptr] >= '5'){res[LSB_ptr--] = 0; 
if(res[LSB_ptr] == '.')LSB_ptr -= 1;
res[LSB_ptr] += 1;
while (res[LSB_ptr] == ':'){res[LSB_ptr--] = 0;
if (res[LSB_ptr] == '.')LSB_ptr -= 1;
res[LSB_ptr] += 1;}}

if(sign == '-')
    {for(int m = 0; m <= 15; m++)res[16-m] = res[15-m];               //For negative numbers shift the array once place to the right
    res[0] = '-';}

}












/***************************************************************************************************************************************/
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


/***************************************************************************************************************************************/
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    {   temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; }} 




/***************************************************************************************************************************************/
long Num_from_KBD(char digits[]){                                   //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long I_number;
cr_keypress = 0;                                                    //Set to one when carriage return keypress terminates the string
for(int n = 0; n<=8; n++) digits[n] = 0;                            //Clear the buffer used to the string
do
{keypress =  waitforkeypress();} 
while ((!(decimal_digit(keypress)))
&& (keypress != '-'));
digits[0] = keypress;
int_string_to_display();                                            //Update display with the first key press
while(1){
if ((keypress = wait_for_return_key())  =='\r')break;               //Detect return key press (i.e \r or\r\n)
if ((decimal_digit(keypress)) || (keypress == '\b') || (keypress == '-'))
{

if (keypress == '\b'){
for (int n = 0; n <= 7; n++)
digits[n] = digits[n + 1];}


else


{for(int n = 8; n>=1; n--)                                          //Shift display for each new keypress except '.'
digits[n] = digits[n-1];
digits[0] = keypress;  }                                            //Add new keypress           

int_string_to_display();
}}                                                                  //Update display includes "cr_keypress"                                                 
cr_keypress = 1;                                                     //End of string; return key pressed
int_string_to_display();
cr_keypress = 0;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);                    //Activate TWI and wait for contact from display pcb 
while (!(TWCR & (1 << TWINT)));
I_number =  byte(receive_byte_with_Ack());                            //Build up the number as each byte is received
I_number = (I_number << 8) + byte(receive_byte_with_Ack());
I_number = (I_number << 8) + byte(receive_byte_with_Ack());
I_number = (I_number << 8) + byte(receive_byte_with_Nack());
TWCR = (1 << TWINT);
return I_number;}



/***************************************************************************************************************************************/
char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}



/***************************************************************************************************************************************/
char wait_for_return_key(void){  
char keypress,temp;
keypress = waitforkeypress();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = receiveChar();}keypress = '\r';}
return keypress;}


/***************************************************************************************************************************************/
float Float_from_KBD(char digits[]){                                   //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
float f_number;
float * Flt_ptr_local;
char * Char_ptr_local;

Flt_ptr_local = &f_number;
Char_ptr_local = (char*)&f_number;

cr_keypress = 0;                                                    //Set to one when carriage return keypress terminates the string
for(int n = 0; n<=7; n++) digits[n] = 0;                            //Clear the buffer used to the string
do
{keypress =  waitforkeypress();} 
while ((!(decimal_digit(keypress)))
&& (keypress != '-')
&& (keypress != '.'));


digits[0] = keypress;
if (keypress == '.')digits[0] = '0' | 0x80;


float_string_to_display();                                           //Update display with the first key press
while(1){
if ((keypress = wait_for_return_key())  =='\r')break;               //Detect return key press (i.e \r or\r\n)
if ((decimal_digit(keypress)) || (keypress == '.')
|| (keypress == '\b')|| (keypress == '-'))
{

if(keypress == '\b'){for (int n = 0; n <= 7; n++)
digits[n] = digits[n + 1];}

else

{if(keypress != '.')
{for(int n = 7; n>=1; n--)                                          //Shift display for each new keypress except '.'
digits[n] = digits[n-1];
digits[0] = keypress;}                                              //Add new keypress           
else digits[0] |= 0x80;}



float_string_to_display();
}}                                                                  //Update display includes "cr_keypress"                                                 
cr_keypress = 1;                                                     //End of string; return key pressed
float_string_to_display();
cr_keypress = 0;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);                    //Activate TWI and wait for contact from display pcb 
while (!(TWCR & (1 << TWINT)));

*Char_ptr_local =  byte(receive_byte_with_Ack());  Char_ptr_local += 1;                      //Build up the number as each byte is received
*Char_ptr_local =  byte(receive_byte_with_Ack());  Char_ptr_local += 1;  
*Char_ptr_local =  byte(receive_byte_with_Ack());  Char_ptr_local += 1;      
*Char_ptr_local =  byte(receive_byte_with_Nack());    
f_number = *Flt_ptr_local;

TWCR = (1 << TWINT);
return f_number;}




/**********************************************************************************************************/
/* void ftoa(float Fnum, char FP_string[], int afterpoint){
		long ipart;
		char sign = '+';
		
		for(int m = 0; m <= 8; m++) FP_string[m] = 0;
		
		if (Fnum < 0){sign = '-'; Fnum *= (-1);}
		
		ipart = (long)Fnum;
		float fpart = Fnum - (float)ipart;
		long i = longToStr(ipart, FP_string, 0);

		if (afterpoint != 0){
			FP_string[i] = '.';
			fpart = fpart * pow(10,afterpoint);
		longToStr((long)fpart, FP_string + i + 1, afterpoint);}
		
		if(sign == '-'){for(int m = 0; m <= 7; m++)FP_string[8-m] = FP_string[7-m];
		FP_string[0] = '-';	
		}	}*/



