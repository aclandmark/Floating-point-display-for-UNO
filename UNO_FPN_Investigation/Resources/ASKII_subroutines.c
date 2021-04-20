


char isCharavailable (int);
char waitforkeypress(void);
long Int_from_KBD(void);
char decimal_digit (char);
char wait_for_return_key(void);
float Float_from_KBD(void);

void int_string_to_display(void);
void float_string_to_display(void);

float float_num_from_eepom(void);

float my_atof(unsigned char *);
void FP_string_to_PC(unsigned char*);
void display_to_PC (char* );
void pcb_display_to_PC(char*);

/**********************************************************************************************/
char isCharavailable (int m){int n = 0;										//For m = 1 waits a maximun of 7.8mS
while (!(Serial.available())){n++;											//before returning zero
if (n>4000) {m--;n = 0;}if (m == 0)return 0;}								//Returns 1 immediately that a char is received
return 1;}



char waitforkeypress(void){
while (!(Serial.available()));
return Serial.read();}


/***************************************************************************************************************************************/
char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}


/***************************************************************************************************************************************/
char wait_for_return_key(void){  											//Returns keypress
char keypress,temp;															//but converts \r\n, \r or \n to \r
while (!(Serial.available())); 
keypress = Serial.read();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = Serial.read();}keypress = '\r';}
return keypress;}






/***************************************************************************************************************************************/
float Float_from_KBD(void){                    								//Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
float f_number;
float * Flt_ptr_local;
char * Char_ptr_local;
char SREG_BKP;

SREG_BKP = SREG;
sei();

Flt_ptr_local = &f_number;
Char_ptr_local = (char*)&f_number;

cr_keypress = 0;                                                    		//Set to one when carriage return keypress terminates the string
for(int n = 0; n<=7; n++) data_buff[n] = 0;                           		//Clear the buffer used to the string
do
{while (!(Serial.available())); 
keypress = Serial.read();} 
while ((!(decimal_digit(keypress)))
&& (keypress != '-')
&& (keypress != '.'));


data_buff[0] = keypress;
if (keypress == '.')data_buff[0] = '0' | 0x80;

FP_string_to_PC(data_buff);                                           			//Update display with the first key press
while(1){
if ((keypress = wait_for_return_key())  =='\r')break;               		//Detect return key press (i.e \r or\r\n)

if ((decimal_digit(keypress)) || (keypress == '.')
|| (keypress == '\b')|| (keypress == '-')
|| (keypress == 'E') || (keypress == 'e'))

{if(keypress == '\b'){for (int n = 0; n <= 7; n++)
data_buff[n] = data_buff[n + 1];}

else

{if(keypress != '.')
{for(int n = 7; n>=1; n--)                                          		//Shift display for each new keypress except '.'
data_buff[n] = data_buff[n-1];
data_buff[0] = keypress;}                                              		//Add new keypress           
else data_buff[0] |= 0x80;}												//Intregrate decimal point and its parent digit

FP_string_to_PC(data_buff);
}}                                                                  		//Update display includes "cr_keypress"                                                  = 1;  
FP_string_to_PC(data_buff);
cr_keypress = 0;
SREG = SREG_BKP;

f_number = my_atof(data_buff);

return f_number;}




/**************************************************************************************************************************/
float float_num_from_eepom(void){
float f_number;
float * Flt_ptr_local;
char * Char_ptr_local;

Flt_ptr_local = &f_number;
Char_ptr_local = (char*)&f_number;

*Char_ptr_local =  byte(eeprom_read_byte((uint8_t*)(0x3FF)));  Char_ptr_local += 1;
*Char_ptr_local =  byte(eeprom_read_byte((uint8_t*)(0x3FE)));  Char_ptr_local += 1;
*Char_ptr_local =  byte(eeprom_read_byte((uint8_t*)(0x3FD)));  Char_ptr_local += 1;
*Char_ptr_local =  byte(eeprom_read_byte((uint8_t*)(0x3FC))); 
f_number = *Flt_ptr_local;
return f_number;}




/**************************************************************************************************************************/
float my_atof(unsigned char * UCptr ){										//Reverses string and separates dp from its digit
char f_string[12], p = 0;

for (int m = 0; m <= 11; m++)f_string[m] = 0;

for (int m = 0; m <= 11; m++){if(!(UCptr[11 - m]))continue;
if(UCptr[11- m] & 0x80){f_string[p++] = (UCptr[11- m] & ~(0x80));f_string[p] = '.';}
else {f_string[p] = UCptr[11- m];}
p += 1;}

return atof(f_string);}


/******************************************************************************************************/
void FP_string_to_PC(unsigned char* UCptr){  								//array[zero] always holds least significant 
char array[12], p = 0;


for (int m = 0; m <= 11; m++)array[m] = 0;  Serial.write("\r\n");

for(int m = 0; m <= 11; m++){if (!(UCptr[11- m])) {Serial.write('_');continue;}		//Reverse array as sent to display
if(UCptr[11- m] & 0x80){array[p++] = (UCptr[11- m] & ~(0x80));array[p] = '.';}
else {array[p] = UCptr[11- m];}
p += 1;}

Serial.write(array);															//Prints array starting with [0] (the MSD).
}



/******************************************************************************************************/
void display_to_PC (char* SCptr){
char array[12];
for (int m = 0; m <= 11; m++)array[m] = 0;
Serial.write("\r\n");


for(int m = 0; m <= 11; m++){if (!(SCptr[11-m])) {Serial.write('_');}		//Reverse array as sent to display
if(SCptr[11-m] & 0x80){Serial.write((SCptr[11-m] & ~(0x80)));Serial.write('.');}
else if(SCptr[11-m])Serial.write(SCptr[11-m]);
}}

/******************************************************************************************************/

void pcb_display_to_PC(char* SCptr){

//Serial.write("\r\n");
for(int m = 0; m <= 11; m++){if (!(SCptr[m])) {Serial.write('_');}		//Reverse array as sent to display
if(SCptr[m] & 0x80){Serial.write((SCptr[m] & ~(0x80)));Serial.write('.');}
else if(SCptr[m])Serial.write(SCptr[m]);
}}



