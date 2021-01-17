

void USART_init (unsigned char, unsigned char);
void newline(void);
char waitforkeypress(void);
char receiveChar(void);
char isCharavailable (int);
void sendChar(char);
void sendString(const char*);


long Num_from_KBD(char*);
float Float_from_KBD(char *);
char decimal_digit (char);
char wait_for_return_key(void);
void int_string_to_display(void);
void float_string_to_display(void);


/**********************************************************************************************/
void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
UCSR0B = 0;
UBRR0H = UBRROH_N; 
UBRR0L = UBRR0L_N;  
UCSR0A = (1 << U2X0);
UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 




/**********************************************************************************************/
void newline(void){sendString ("\r\n");}



/**********************************************************************************************/
char waitforkeypress(void){
while (!(UCSR0A & (1 << RXC0)));
return UDR0;}



/**********************************************************************************************/
char receiveChar(void)
{return UDR0;}



/**********************************************************************************************/
char isCharavailable (int m){int n = 0;		
while (!(UCSR0A & (1 << RXC0))){n++;
if (n>4000) {m--;n = 0;}if (m == 0)return 0;}
return 1;}




/**********************************************************************************************/
void sendChar(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}





/**********************************************************************************************/
void sendString(const char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }





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
|| (keypress == '\b')|| (keypress == '-')
|| (keypress == 'E') || (keypress == 'E'))

{if(keypress == '\b'){for (int n = 0; n <= 7; n++)
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




