
char receive_byte_with_Ack(void);
char receive_byte_with_Nack(void);
void send_byte_with_Ack(char);
void send_byte_with_Nack(char);
void TWI_interrupt(void);

void TWI_master(void);
void UNO_slave_receiver(void);



void TWI_master(void){                                  	//Configure UNO as master
 char data[4];
 unsigned char R_W_bit = 0;                              	//Master write operation
 Reset_H;
 TWBR = 255;                                              	//29.6KHz   (32  gives  100khz)
    
  while(1){
    for(int m = 0; m < 4; m++){  
  data[m] = waitforkeypress();}
    
  while(1){                                               //Repetitively poll the PIC until it responds
TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);         //send a start condition  (this identical to a "repeated start")
while (!(TWCR & (1 << TWINT)));                           //Wait for TWINT flag 
TWDR = (slave_address  << 1) | R_W_bit;                  //Address of slave (master write operation)  SLA + W
TWCR = (1 << TWINT) | (1 << TWEN);                        //Clear TWINT bit to start transmission of address
while (!(TWCR & (1 << TWINT)));                           //Wait for TWINT flag 
if (TWSR == 0x18)break; }                                 //SLA + W successfully transmitted ACK recveived 



for(int m = 0; m < 4; m++){
  TWDR = data[m];
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));}
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}}




ISR(TWI_vect){TWI_interrupt();}							//UNO slave transmitter test subroutine

void TWI_interrupt(void){
  switch (TWSR){
    case 0xA8:
	
	send_byte_with_Ack(data_type);
	
	switch(data_type){
	case 'A':
	case 'C':
	send_byte_with_Ack(data_buff[3]);
    send_byte_with_Ack(data_buff[2]);
    send_byte_with_Ack(data_buff[1]);
    send_byte_with_Ack(data_buff[0]);
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
    send_byte_with_Nack(*Char_ptr); break;
		
	}
	
	
	TWCR = (1 << TWINT); 
	active_transaction = 0;
	break;}
	}



void UNO_slave_receiver(void){								//UNO slave receiver test subroutine
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
while (!(TWCR & (1 << TWINT)));

for (int m = 0; m <= 4; m++){
  if(m == 4)data[4] = receive_byte_with_Nack();
  else data[m] = receive_byte_with_Ack();}
TWCR = (1 << TWINT);for(int m = 0; m<=4; m++)sendChar(data[m] + '0');
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
while (!(TWCR & (1 << TWINT)));

for (int m = 5; m <= 9; m++){
  if(m == 9)data[9] = receive_byte_with_Nack();
  else data[m] = receive_byte_with_Ack();}
TWCR = (1 << TWINT);
for(int m = 5; m<=9; m++)sendChar(data[m] + '0');}









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
while (!(TWCR & (1 << TWINT)));
}



/****************************************************************************************************************/
void send_byte_with_Nack(char byte){
TWDR = byte;                    //Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEN);    //clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));
}
