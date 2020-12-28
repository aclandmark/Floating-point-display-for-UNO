
#include "header_file.h"


void Round_and_Display(char*, char, signed char);
char un_rounded_array[16];
int main()
{char res[16];  //20
  float n ;
  int afterpoint = 4;
  char expt=0;
  
  setup_HW;
  waitforkeypress();waitforkeypress();waitforkeypress();
  sendStringF("Enter FPN numbers\r\n");


   n = (Float_from_KBD(res));
   
   while(1){
 expt =  ftoa(n, res, afterpoint); 
 sendStringV(un_rounded_array); 
 sendStringF("\t Round Number plus exponent\t"); 
  
  sendStringV(res); if(expt){sendChar ('E');Num_to_PC(10, expt); }
   
  sendStringF("\r\n");
   if( waitforkeypress() == 'x') n = n *2;               
   else n = n/2;                                 
      }
  return 0;}






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








/**********************************************************************************************/
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






/**********************************************************************************************************/
void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
  UCSR0B = 0;
  UBRR0H = UBRROH_N;
  UBRR0L = UBRR0L_N;
  UCSR0A = (1 << U2X0);
  UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);}
