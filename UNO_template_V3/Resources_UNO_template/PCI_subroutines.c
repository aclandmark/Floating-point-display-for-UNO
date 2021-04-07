

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
Data_Entry_complete = 0;													//Set to 1 when FP number entry is complete
digit_entry = 0;															//Set to 1 when digit is selected

dp_control = 0;																//Controls display of the dp
neg_sign = 0;																//controls display of the negative sign
exp_control = 0;															//Controls display of the E sign
sei();
cr_keypress = 0;															//Set to 1 when atof() conversion is required  
data_buff[0] = '0';
float_string_to_display();													//Initialise display

do{																			//Repeat untill FPN string entry is complete
while((!(Data_Entry_complete)) && (!(digit_entry)));						//Wait for input from IO 
digit_entry = 0;
}while(!(Data_Entry_complete));
Data_Entry_complete = 0;

cr_keypress = 1; 															//Entry of FP string complete 
float_string_to_display();													//Acquire FP number from display driver
cr_keypress = 0;

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
{scroll_display_zero();														//Hold SW1 down to scroll throuh the availble chars (0-9, E anf -)
Timer_T0_10mS_delay_x_m(20);}
while(switch_3_down);enable_pci_on_sw3;										//Wait for SW3 to be released (may have been pressed to select dp)
if(switch_2_down)shift_display_left();										//Press SW2 to accept the next char and shift the display
Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw2;}


/*************************************************************************/
void shift_display_left(void){

if(data_buff[0] & 0x80)dp_control = 1;										//No more decimal points allowed
if (data_buff[0] == 'e')
{exp_control = 1; neg_sign = 0;} 											//Negative exponent allowed
else neg_sign = 1;															//FP number can have one negative sign 

scroll_control = (exp_control << 2) | (dp_control << 1) | neg_sign;		

shift_digits_left; 
data_buff[0] = '0'; 
float_string_to_display();
digit_entry = 1;}


/*************************************************************************/

void scroll_display_zero(void){
unsigned char data_zero;

disable_pci_on_sw3;

data_zero = data_buff[0];

if (switch_3_up){data_buff[0] &= (~(0x80));							//Decimal point dissabled

switch (data_buff[0]){
case '9':
switch(scroll_control){	
case 0: data_buff[0] = '-'; break;										//Initial state scroll 0-9 -ve 0...... dp is enabled
case 1: data_buff[0] = '0'; break;										//First char entered -ve sign dissabled  dp enabled
case 3: data_buff[0] = 'e'; break;										//Decimal point entered.  Dissable DP. Enable E.
case 6: data_buff[0] = '-'; break;										//E entered. Enable -ve sign
case 7: data_buff[0] = '0'; break;}break;								//Dissable -ve sign

case '-':
switch(scroll_control){
case 0: data_buff[0] = '0'; break;										//Initial state
case 6: data_buff[0] = '0'; break;}break;

case 'e':
switch(scroll_control){
case 0: data_buff[0] = '0'; break;										//Initial state
case 1: data_buff[0] = '0'; break;										//
case 3: data_buff[0] = '0'; break;}break;

default: data_buff[0] += 1; break;}}

if((switch_3_down) && (!(dp_control))){data_buff[0] |= 0x80;			//Decimal point enabled 

switch (data_buff[0] & (~(0x80))){
case '9': 
//case '-':
//case 'e': 
data_buff[0] = ('0' | 0x80);break;
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


