

void TWI_interrupt(void);

char int_num_to_display(long);
void float_string_to_display(void);
void int_string_to_display(void);
void float_num_to_display(float);

char receive_byte_with_Ack(void);
char receive_byte_with_Nack(void);
void send_byte_with_Ack(char);
void send_byte_with_Nack(char);

void check_for_OVF(float);

volatile char data_type;



#define wdr()  __asm__ __volatile__("wdr")

#define wd_timer_off \
wdr();\
MCUSR &= (~(1 << WDRF));\
WDTCSR |= (1<<WDCE) | (1<<WDE);\
WDTCSR = 0x00;




ISR(TWI_vect){TWI_interrupt();}														//UNO slave transmitter test subroutine

void TWI_interrupt(void){
  switch (TWSR){
    case 0xA8:
	
	send_byte_with_Ack(data_type);
	
	switch(data_type){
	case 'A':																			//Send Integer sttring to display
	case 'C':																			//Send FP string to display
	send_byte_with_Ack(data_buff[0]);
    send_byte_with_Ack(data_buff[1]);
    send_byte_with_Ack(data_buff[2]);
    send_byte_with_Ack(data_buff[3]);
	send_byte_with_Ack(data_buff[4]);
    send_byte_with_Ack(data_buff[5]);
    send_byte_with_Ack(data_buff[6]);
    send_byte_with_Ack(data_buff[7]);
	send_byte_with_Nack(cr_keypress);
	break;  		
	
	case 'B':																			//Send Integer number to display
	send_byte_with_Ack(num_bytes[3]);
    send_byte_with_Ack(num_bytes[2]);
    send_byte_with_Ack(num_bytes[1]);
    send_byte_with_Nack(num_bytes[0]);break;
	
	
	case 'D':																			//Send FP number to display
	send_byte_with_Ack(*Char_ptr); Char_ptr += 1;
    send_byte_with_Ack(*Char_ptr); Char_ptr += 1;
    send_byte_with_Ack(*Char_ptr); Char_ptr += 1;
    send_byte_with_Nack(*Char_ptr); break;}
	
	
	TWCR = (1 << TWINT); 																//Close UNO I2C connection
	active_transaction = 0;
	break;}}






/***************************************************************************************************************************************/
void int_string_to_display(void){
    char SREG_BKP;
	SREG_BKP = SREG;
	sei();	
	data_type = 'A';                                                          			//UNO sends integer string to display pcb
    active_transaction = 1;
    TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);           			//Enable TWI slave with interrupt on address match
    while (active_transaction);
	SREG = SREG_BKP;}

/***************************************************************************************************************************************/
void float_string_to_display(void){
    char SREG_BKP;
	SREG_BKP = SREG;
	sei();
	data_type = 'C';                                                          			//UNO sends FP string to display pcb
    active_transaction = 1;
    TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);           			//Enable TWI slave with interrupt on address match
    while (active_transaction);
	SREG = SREG_BKP;}
	

/***************************************************************************************************************************************/
void float_num_to_display(float FP_num){
	check_for_OVF(FP_num);
      Char_ptr = (char*)&FP_num;
      data_type = 'D';
      active_transaction = 1;
      TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);            		//Enable TWI slave
      while (active_transaction);}


/***************************************************************************************************************************************/
char int_num_to_display(long Int_num){
      
      if((Int_num > 99999999) || (Int_num < -9999999))return 1;
      
      data_type = 'B';
      active_transaction = 1;
      for (int m = 0; m <= 3; m++)num_bytes[m] = Int_num >> 8*m;                		//Split the number into its bytes
      TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);            		//Enable TWI slave
      while (active_transaction);
      return 0;}



/***************************************************************************************************************************************/
char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);                										//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));                   										//Wait for interrupt
byte = TWDR;
return byte;}


   /****************************************************************************************************************/
char receive_byte_with_Ack(void){
char byte;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);    									//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));                     									//Wait for interrupt
byte = TWDR;
return byte;}



/****************************************************************************************************************/
void send_byte_with_Ack(char byte){
TWDR = byte;                      														//Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);    									//clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));}



/****************************************************************************************************************/
void send_byte_with_Nack(char byte){
TWDR = byte;                    														//Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEN);    													//clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));}


/****************************************************************************************************************/
void check_for_OVF(float Fnum){     													//Subroutine crashes if result exceeeds FP bounds                
  long  Fnum_int;
  char sign = '+';
  float Fnum_bkp;
  
  Fnum_bkp = Fnum; 
  
  float * Flt_ptr_local;
  char * Char_ptr_local;

  Flt_ptr_local = &Fnum_bkp;
  Char_ptr_local = (char*)&Fnum_bkp;
  
  eeprom_write_byte((uint8_t*)(0x3FB), 0);												//Remains zero if the FPN exceeds its bounds
  
  if (Fnum < 0){sign = '-'; Fnum *= (-1);}                  							//Convert negative numbers to positive ones and set the sign character
  wdt_enable(WDTO_30MS);
  
  Fnum_int = (long)Fnum;                                    							//Obtain integer part of the number
  if (Fnum  >= 10000) {while (Fnum >= 10)
  {Fnum /= 10; }}
  if(Fnum < 0.01) {while (Fnum < 1){Fnum *= 10; }}
  
  
  wd_timer_off;
  
  eeprom_write_byte((uint8_t*)(0x3FF), (*Char_ptr_local)); Char_ptr_local += 1;		//Save FPN if within bounds
  eeprom_write_byte((uint8_t*)(0x3FE), (*Char_ptr_local)); Char_ptr_local += 1;
  eeprom_write_byte((uint8_t*)(0x3FD), (*Char_ptr_local)); Char_ptr_local += 1;
  eeprom_write_byte((uint8_t*)(0x3FC), (*Char_ptr_local));
  if(eeprom_read_byte((uint8_t*)0x3FA))eeprom_write_byte((uint8_t*)(0x3FB), 0xFF); }
