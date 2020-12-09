


void USI_TWI_Master_Initialise(void);
char send_save_address_plus_RW_bit(unsigned char address);
void write_data_to_slave(unsigned char data_byte, char last_char);
unsigned char read_data_from_slave(char last_char);
unsigned char USI_TWI_Start_Transceiver_With_Data( unsigned char *, unsigned char);
unsigned char USI_TWI_Master_Transfer(unsigned char);
void USI_TWI_Master_Stop(void);
long data_from_UNO(void);
void reverse(char *, int);
long longToStr(long , char *, int );
void ftoa(float, char *, int);


#define DDR_USI             DDRA
#define PORT_USI            PORTA
#define PIN_USI             PINA
#define PORT_USI_SDA        PORTA0
#define PORT_USI_SCL        PORTA2
#define PIN_USI_SDA         PINA0
#define PIN_USI_SCL         PINA2


unsigned char tempUSISR_8bit =
(1<<USISIF)|(1<<USIOIF)| (1<<USIPF)|(1<<USIDC)|						// Prepare register value to: Clear flags, and
(0x0<<USICNT0);														// set USI to shift 8 bits i.e. count 16 clock edges.
unsigned char tempUSISR_1bit =
(1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|						// Prepare register value to: Clear flags, and
(0xE<<USICNT0);


unsigned char TWI_slaveAddress;
int EE_size = 0x200;


#define T2_TWI 5													//5uS delay generates 100KHz clock
#define T4_TWI 4


/*************************************************************************************************************************************/
void USI_TWI_Master_Initialise( void )
{
	PORT_USI |= (1<<PIN_USI_SDA);										// Enable pull-up on SDA, to set high as released state.
	PORT_USI |= (1<<PIN_USI_SCL);										// Enable pull-up on SCL, to set high as released state.
	
	DDR_USI  |= (1<<PIN_USI_SCL);										// Enable SCL as output.
	DDR_USI  |= (1<<PIN_USI_SDA);										// Enable SDA as output.
	
	USIDR    =  0xFF;													// Pre-load data register with "released level" data.
	USICR    =  (0<<USISIE)|(0<<USIOIE)|								// Disable Interrupts.
	(1<<USIWM1)|(0<<USIWM0)|											// Set USI in Two-wire mode.
	(1<<USICS1)|(0<<USICS0)|(1<<USICLK)|								// Software strobe as counter clock source  (EXTERNAL DATA CLOCK)
	(0<<USITC);
	USISR   =   (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|			// Clear flags,
	(0x0<<USICNT0);														// and reset counter.
}


/*************************************************************************************************************************************/
char send_save_address_plus_RW_bit(unsigned char address_plus_RW_bit){
PORT_USI |= (1<<PIN_USI_SCL);										// Release SCL. (output hight)
while( !(PIN_USI & (1<<PIN_USI_SCL)) );								// Verify that SCL becomes high.
_delay_us( T2_TWI );												// Delay for T2_TWI

PORT_USI &= ~(1<<PIN_USI_SDA);										// Force SDA LOW to Generate Start Condition.
_delay_us( T4_TWI );
PORT_USI &= ~(1<<PIN_USI_SCL);										// Pull SCL LOW.
PORT_USI |= (1<<PIN_USI_SDA);										// Release SDA.	
	
PORT_USI &= ~(1<<PIN_USI_SCL);										// Pull SCL LOW.
USIDR     = address_plus_RW_bit;									// copy slave address to data register.
USI_TWI_Master_Transfer( tempUSISR_8bit );							// Send 8 bits on bus.	
DDR_USI  &= ~(1<<PIN_USI_SDA);										// Enable SDA as input.
if(!(USI_TWI_Master_Transfer( tempUSISR_1bit ) & 1)	)				// Wait for (N)Ack byte
return 1;															//Ack received
else return 0;}														//Nack received



/*************************************************************************************************************************************/
void write_data_to_slave(unsigned char data_byte, char last_char){
PORT_USI &= ~(1<<PIN_USI_SCL);										// Pull SCL LOW.
USIDR     = data_byte;												// Setup data.
USI_TWI_Master_Transfer( tempUSISR_8bit );							// Send 8 bits on bus.

DDR_USI  &= ~(1<<PIN_USI_SDA);										// Enable SDA as input.
USI_TWI_Master_Transfer( tempUSISR_1bit );							// Wait for (N)Ack byte
if(last_char)USI_TWI_Master_Stop();}




/*************************************************************************************************************************************/
unsigned char read_data_from_slave(char last_char){
unsigned char data_byte;
	
DDR_USI   &= ~(1<<PIN_USI_SDA);										// Enable SDA as input.
data_byte  = USI_TWI_Master_Transfer( tempUSISR_8bit );

if( last_char) {USIDR = 0xFF;}										// Load NACK to confirm End Of Transmission.
else {USIDR = 0x00; }												 // Load ACK. Set data register bit 7 (output for SDA) low.
USI_TWI_Master_Transfer( tempUSISR_1bit );							// Send ACK/NACK.
if(last_char)USI_TWI_Master_Stop();
return data_byte;}






/*************************************************************************************************************************************/
unsigned char USI_TWI_Master_Transfer( unsigned char temp )
{
	USISR = temp;														// Set USISR according to temp.
																		// Prepare clocking.
	temp  =  (0<<USISIE)|(0<<USIOIE)|									// Interrupts disabled
	(1<<USIWM1)|(0<<USIWM0)|											// Set USI in Two-wire mode.
	(1<<USICS1)|(0<<USICS0)|(1<<USICLK)|								// Software clock strobe as source.
	(1<<USITC);															// Toggle Clock Port.
	do
	{
		_delay_us( T2_TWI );
		USICR = temp;													// Generate positive SCL edge.
		while( !(PIN_USI & (1<<PIN_USI_SCL)) );							// Wait for SCL to go high.
		_delay_us( T4_TWI );
		USICR = temp;													// Generate negative SCL edge.
	}while( !(USISR & (1<<USIOIF)) );									// Check for transfer complete.
	
	_delay_us( T2_TWI );
	temp  = USIDR;														// Read out data.
	USIDR = 0xFF;														// Release SDA.
	DDR_USI |= (1<<PIN_USI_SDA);										// Enable SDA as output.

	return temp;														// Return the data from the USIDR
}



/*************************************************************************************************************************************/
void USI_TWI_Master_Stop( void )
{
	PORT_USI &= ~(1<<PIN_USI_SDA);										// Pull SDA low.
	PORT_USI |= (1<<PIN_USI_SCL);										// Release SCL.
	while( !(PIN_USI & (1<<PIN_USI_SCL)) );								// Wait for SCL to go high.
	_delay_us( T4_TWI );
	PORT_USI |= (1<<PIN_USI_SDA);										// Release SDA.
	_delay_us( T2_TWI );}
	
	
	
	


	/*************************************************************************************************************************************/
	long data_from_UNO(void){char counter = 32; 
	
	int display_counter;											//Used to convert integer number to string
	char sign = '+';												//Sign of integer number
		
	while (((!(send_save_address_plus_RW_bit(0x7)))) && counter)	//Address is 3 and W/R bit is 1 for UNO transmit. 
	{ counter -= 1;}												//Master polls UNO 32 times and gives up if no response
	
	if (counter){													//UNO responds
	transaction_type = read_data_from_slave(0);						//First data byte 
		
	switch (transaction_type){
	case 'A':														//UNO sends a integer string terminated in carriage return
	for(int m = 0; m <= 3; m++)	{
	display_buf[m] = read_data_from_slave(0);}						//Receive string members one at a time	
	cr_keypress = read_data_from_slave(1);							//One for a carriage return, otherwise zero 
	break;
	
	case 'B':														//UO sends a binary integer as four bytes
	I_number = read_data_from_slave(0);								//Assemble the I_number
	I_number = (I_number << 8) + read_data_from_slave(0);
	I_number = (I_number << 8) + read_data_from_slave(0);
	I_number = (I_number << 8) + read_data_from_slave(1);
	
	display_counter = 0;											//Convert the I_number to a string
	if(I_number < 0 ){sign = '-'; I_number *= (-1);}				//and illuminate the display
	for(int m = 0; m<=3; m++)display_buf[m] = 0;
	do {display_buf[3 - display_counter] = (I_number % 10) + '0' ;
	display_counter++;} while ((I_number = I_number/10) > 0);
	
	if (sign == '-'){display_buf[3 - display_counter] = '-';}
	break;
	
	case 'C':														//UNO sends a float string terminated in carriage return
	for(int m = 0; m <= 3; m++)	{
	display_buf[m] = read_data_from_slave(0);}						//Receive string members one at a time
	cr_keypress = read_data_from_slave(1);							//One for a carriage return, otherwise zero
	break;
	
	case 'D':														//UNO sends a binary number as four bytes
	char_ptr = (char*)&flt_num;
	
	*char_ptr = read_data_from_slave(0);char_ptr += 1;
	*char_ptr = read_data_from_slave(0);char_ptr += 1;
	*char_ptr = read_data_from_slave(0);char_ptr += 1;
	*char_ptr = read_data_from_slave(1);
	f_num_ptr = &flt_num;
	flt_num = *f_num_ptr;
	ftoa(flt_num, flt_array, 2);
	
	/*****************************************************/
	
	array_cntr = 0;
	
	if (flt_array[0] == '.'){ for(int m = 0; m <=8 ; m++){flt_array[9-m] = flt_array[8-m]; }flt_array[0] = '0';}
	
	while (flt_array[array_cntr] != '.')array_cntr += 1;
	{flt_array[array_cntr-1] |= 0x80; for (int p = array_cntr; p <= 8; p++)flt_array[p] = flt_array[p+1];
	
		
		
		
		
	}
	
	/****************************************************/
	display_buf[0] = flt_array[0];
	display_buf[1] = flt_array[1];
	display_buf[2] = flt_array[2];
	display_buf[3] = flt_array[3];
	break;
	
	
	
	}}
	
	else {	USI_TWI_Master_Stop();}									//Send stop is the absence of any response from the UNO.
	
	return I_number;	}
		
		
	/***************************************************************************************************************************************/
	void ftoa(float Fnum, char FP_string[], int afterpoint){
		long ipart = (long)Fnum;
		float fpart = Fnum - (float)ipart;
		long i = longToStr(ipart, FP_string, 0);

		if (afterpoint != 0){
			FP_string[i] = '.';
			fpart = fpart * pow(10,afterpoint);
		longToStr((long)fpart, FP_string + i + 1, afterpoint);}}



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

	