

void TWI_interrupt(void);

char int_num_to_display(long);
void float_string_to_display(void);
void int_string_to_display(void);
void float_num_to_display(float);

char receive_byte_with_Ack(void);
char receive_byte_with_Nack(void);
void send_byte_with_Ack(char);
void send_byte_with_Nack(char);

volatile char data_type;



ISR(TWI_vect){TWI_interrupt();}							//UNO slave transmitter test subroutine

void TWI_interrupt(void){
  switch (TWSR){
    case 0xA8:
	
	send_byte_with_Ack(data_type);
	
	switch(data_type){
	case 'A':
	case 'C':
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
	
	case 'B':
	send_byte_with_Ack(num_bytes[3]);
    send_byte_with_Ack(num_bytes[2]);
    send_byte_with_Ack(num_bytes[1]);
    send_byte_with_Nack(num_bytes[0]);break;
	
	
	case 'D':
	send_byte_with_Ack(*Char_ptr); Char_ptr += 1;
    send_byte_with_Ack(*Char_ptr); Char_ptr += 1;;
    send_byte_with_Ack(*Char_ptr); Char_ptr += 1;;
    send_byte_with_Nack(*Char_ptr); break;}
	
	
	TWCR = (1 << TWINT); 
	active_transaction = 0;
	break;}}






/***************************************************************************************************************************************/
void int_string_to_display(void){
    data_type = 'A';                                                          //UNO sends numeric string to display pcb
    active_transaction = 1;
    TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);           //Enable TWI slave with interrupt on address match
    while (active_transaction);}


/***************************************************************************************************************************************/
void float_string_to_display(void){
    data_type = 'C';                                                          //UNO sends numeric string to display pcb
    active_transaction = 1;
    TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);           //Enable TWI slave with interrupt on address match
    while (active_transaction);}
	

/***************************************************************************************************************************************/
void float_num_to_display(float FP_num){
      Char_ptr = (char*)&FP_num;
      data_type = 'D';
      active_transaction = 1;
      TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);            //Enable TWI slave
      while (active_transaction);}


/***************************************************************************************************************************************/
char int_num_to_display(long Int_num){
      
      if((Int_num > 99999999) || (Int_num < -9999999))return 1;
      
      data_type = 'B';
      active_transaction = 1;
      for (int m = 0; m <= 3; m++)num_bytes[m] = Int_num >> 8*m;                //Split the number into its bytes
      TWCR = (1 << TWEN) | (1 <<TWINT) | (1 << TWEA) | (1 << TWIE);            //Enable TWI slave
      while (active_transaction);
      return 0;}



/***************************************************************************************************************************************/
char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);                //Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));                   //Wait for interrupt
byte = TWDR;
return byte;}


   /****************************************************************************************************************/
char receive_byte_with_Ack(void){
char byte;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);    //Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));                     //Wait for interrupt
byte = TWDR;
return byte;}



/****************************************************************************************************************/
void send_byte_with_Ack(char byte){
TWDR = byte;                      //Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);    //clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));}



/****************************************************************************************************************/
void send_byte_with_Nack(char byte){
TWDR = byte;                    //Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEN);    //clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));}
