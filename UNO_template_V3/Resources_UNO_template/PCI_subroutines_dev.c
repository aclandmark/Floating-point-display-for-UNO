

float FPN_number_from_IO(void);
void shift_display_left(void);
void scroll_display_zero(void);
void Timer_T0_10mS_delay_x_m(int);

void Timer_T0_sub(char, unsigned char);

#define T0_delay_10ms 5,100

#define shift_digits_left {for (int n = 0; n < 7; n++){data_buff[7-n] = data_buff[6-n];}}

#define switch_1_down	((PIND & 0x04)^0x04)
#define switch_1_up		(PIND & 0x04)
#define switch_2_down	((PIND & 0x80)^0x80)
#define switch_2_up		(PIND & 0x80)
#define switch_3_down	((PINB & 0x04)^0x04)
#define switch_3_up		(PINB & 0x04)


#define PCI_on_SW1_and_SW2 \
PCICR |= (1 << PCIE2);\
PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);

#define PCI_on_SW3 \
PCICR |= (1 << PCIE0);\
PCMSK0 |= (1 << PCINT2);

#define setup_and_enable_PCI \
PCI_on_SW1_and_SW2;\
PCI_on_SW3;

#define disable_pci_on_sw3  PCMSK0 &= (~(1 << PCINT2));
#define disable_pci_on_sw1_and_sw2  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));

#define clear_PCI_on_sw3 		 	PCIFR |= (1<< PCIF0);
#define clear_PCI_on_sw1_and_sw2  	PCIFR |= (1<< PCIF2);

#define enable_pci_on_sw3  PCMSK0 |= (1 << PCINT2);


/*************************************************************************/
float FPN_number_from_IO(void){

char keypress = 0;
float f_number;
float * Flt_ptr_local;
char * Char_ptr_local;

Flt_ptr_local = &f_number;
Char_ptr_local = (char*)&f_number;


setup_and_enable_PCI;
Data_Entry_complete = 0;
digit_entry = 0;
scroll_control = 0b00011110;
dp_control = 0;
/*
bit 0: exponent disabled
bit 1: decimal point enabled
bit 2: negative sign enabled
bit 3: LHS of exponent
bit 4: Waiting fot first character
*/

sei();
cr_keypress = 0;
data_buff[0] = '0';
float_string_to_display();

do{																			//Repeat untill FPN string entry is complete
while((!(Data_Entry_complete)) && (!(digit_entry)));						//Wait for input from IO 
digit_entry = 0;
}while(!(Data_Entry_complete));
Data_Entry_complete = 0;

cr_keypress = 1; float_string_to_display(); cr_keypress = 0;Serial.print('A');

TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);                    		//Activate TWI and wait for contact from display pcb 
while (!(TWCR & (1 << TWINT)));

*Char_ptr_local =  byte(receive_byte_with_Ack());  Char_ptr_local += 1;	//Build up the number as each byte is received
*Char_ptr_local =  byte(receive_byte_with_Ack());  Char_ptr_local += 1;  
*Char_ptr_local =  byte(receive_byte_with_Ack());  Char_ptr_local += 1;      
*Char_ptr_local =  byte(receive_byte_with_Nack());    
f_number = *Flt_ptr_local;
TWCR = (1 << TWINT);
disable_pci_on_sw1_and_sw2;
disable_pci_on_sw3;
return f_number;}


/*************************************************************************/
ISR(PCINT0_vect){															//Press SW3 for entry of complete FPN 
char disp_bkp[8];
if(switch_3_up)return;														//Ignore switch release

for(int m = 0; m<=7; m++)
{disp_bkp[m]=data_buff[m]; data_buff[m]= 0;}
float_string_to_display();
Timer_T0_10mS_delay_x_m(50);												//Flash display
for(int m = 0; m<=7; m++)
{data_buff[m]=disp_bkp[m];}
digit_entry = 1;
Data_Entry_complete=1;
}



/*************************************************************************/
ISR(PCINT2_vect){
if((switch_1_up) && (switch_2_up))return;
while(switch_1_down)
{scroll_display_zero();
Timer_T0_10mS_delay_x_m(20);}
while(switch_3_down);enable_pci_on_sw3;
if(switch_2_down)shift_display_left();
Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw2;}


/*************************************************************************/
void shift_display_left(void){
scroll_control &= (~(0x14));							//negative sign & first char disabled

switch(data_buff[0]){
case '-':	break;
case '.':	scroll_control &= (~(0x3));
			dp_control = 0; break;						//exponent & dp disabled	
case 'e':	scroll_control &= (~(0xB));				//Set RHS and disable d.p. 
			scroll_control |= 0x04; break;				//Enable neg sign
default: 	if(scroll_control & 8)						//If LHS but not RHS
			scroll_control |= 1;break;} 				//enable exponent

shift_digits_left; data_buff[0] = '0'; 
float_string_to_display();
digit_entry = 1;}

/*void shift_display_left(void){
shift_digits_left; data_buff[0] = '0'; 
float_string_to_display();
digit_entry = 1;
}*/


/*************************************************************************/

/*void scroll_display_zero(void){
unsigned char data_zero;

disable_pci_on_sw3;

data_zero = data_buff[0];

if (switch_3_up){data_buff[0] &= (~(0x80));

switch (data_buff[0]){
case '9':  data_buff[0] = '-';break;
case '-': data_buff[0] = 'e' ;break;
case 'e': data_buff[0] = '0' ;break;
default:  data_buff[0] += 1; break;}}


if(switch_3_down){data_buff[0] |= 0x80; 

switch (data_buff[0] & (~(0x80))){
case '9': 
case '-':
case 'e': data_buff[0] = ('0' | 0x80);break;
default:  data_buff[0] += 1;break;}}

float_string_to_display();

}*/






void scroll_display_zero(void){							//display scrolls 0 to 9 then minus symbol d.p. E and back to 0

disable_pci_on_sw3;
if (switch_3_up){data_buff[0] &= (~(0x80));

switch (data_buff[0]){
case '9':
switch(scroll_control){
case 0b11110: digits[0] = '-'; break;				//Waiting for first character: digits[0] = '\0'
case 0b01010: digits[0] = '.'; break;				//Waiting for second character: negative number digits[0] = '-'
case 0b01000: digits[0] = '0'; break;				//LHS waiting for first digit (0 to 9)
case 0b01011: digits[0] = '.'; break;				//digits[0] = 0 to 9: can receive d.p. e or additional digit
case 0b01001: digits[0] = 'e'; break;				//Real number: can only receive e or additional digits
case 0b00000: digits[0] = '0'; break;				//RHS: Can only receive digits
case 0b00100: digits[0] = '-'; break;} break;		//RHS: can receive a - or a digit
		

case '-':
switch(scroll_control){
case 0b11110: digits[0] = '.'; break;				//Waiting for first character: digits[0] = '\0'
case 0b00100: digits[0] = '0'; break;} break;		//RHS: can receive a - or a digit

case '.':
switch(scroll_control){
case 0b11110: digits[0] = '0'; break;				//Waiting for first character: digits[0] = '\0'
case 0b01010: digits[0] = '0'; break;				//Waiting for second character: negative number digits[0] = '-'
case 0b01011: digits[0] = 'e'; break;} break;		//digits[0] = 0 to 9: can receive d.p. e or additional digit
	

case 'e':
switch(scroll_control){
case 0b01011: digits[0] = '0'; break;				//digits[0] = 0 to 9: can receive d.p. e or additional digit
case 0b01001: digits[0] = '0'; break;} break;		//Real number: can only receive e or additional digits


default: data_buff[0] += 1; break;}}


if((switch_3_down)){data_buff[0] |= 0x80; 

switch (data_buff[0] & (~(0x80))){
case '9': 
case '-':
case 'e': data_buff[0] = ('0' | 0x80);break;
default:  data_buff[0] += 1;break;}}



float_string_to_display();}



/*********************************************************************/
void Timer_T0_10mS_delay_x_m(int m)
{for (int n = 0; n < m; n++){Timer_T0_sub(T0_delay_10ms);}}

/*********************************************************************/
void Timer_T0_sub(char Counter_speed, unsigned char Start_point){ 
TCNT0 = Start_point;
TCCR0B = Counter_speed;
while(!(TIFR0 && (1<<TOV0)));
TIFR0 |= (1<<TOV0); TCCR0B = 0;}


