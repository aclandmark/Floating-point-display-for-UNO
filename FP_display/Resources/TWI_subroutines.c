
 #define DDR_USI             DDRA
 #define PORT_USI            PORTA
 #define PIN_USI             PINA
 #define PORT_USI_SDA        PORTA0
 #define PORT_USI_SCL        PORTA2
 #define PIN_USI_SDA         PINA0
 #define PIN_USI_SCL         PINA2
 
 #define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TRUE  1
#define FALSE 0
#define USI_TWI_NO_ACK_ON_ADDRESS   0x06 
#define TWI_NACK_BIT  0							// Bit position for (N)ACK bit.
#define USI_TWI_NO_ACK_ON_DATA      0x05		// The slave did not acknowledge  all data
#define USI_TWI_MISSING_STOP_CON    0x08		// Generated Stop Condition not detected on bus


#define SYS_CLK   8000.0  // [kHz]
#define T2_TWI    ((SYS_CLK *4700) /8000000) +1 // >4,7us
#define T4_TWI    ((SYS_CLK *4000) /8000000) +1 // >4,0us



void USI_TWI_Master_Initialise(void);
unsigned char USI_TWI_Master_Transfer( unsigned char);
unsigned char USI_TWI_Start_Transceiver_With_Data( unsigned char *, unsigned char);
unsigned char USI_TWI_Master_Stop(void);





/*************************************************************************************************************************************/
union  USI_TWI_state
{
	unsigned char errorState;         // Can reuse the TWI_state for error states due to that it will not be need if there exists an error.
	struct
	{
		unsigned char addressMode         : 1;
		unsigned char masterWriteDataMode : 1;
		unsigned char unused              : 6;
	};
}   USI_TWI_state;




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
unsigned char USI_TWI_Get_State_Info( void )
{return ( USI_TWI_state.errorState );}                            // Return error state.









/*************************************************************************************************************************************/
unsigned char USI_TWI_Start_Transceiver_With_Data( unsigned char *msg, unsigned char msgSize)
{
	unsigned char tempUSISR_8bit = 
	(1<<USISIF)|(1<<USIOIF)| (1<<USIPF)|(1<<USIDC)|						// Prepare register value to: Clear flags, and
	(0x0<<USICNT0);														// set USI to shift 8 bits i.e. count 16 clock edges.
	unsigned char tempUSISR_1bit = 
	(1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|						// Prepare register value to: Clear flags, and
	(0xE<<USICNT0);														// set USI to shift 1 bit i.e. count 2 clock edges.

	USI_TWI_state.errorState = 0;
	USI_TWI_state.addressMode = TRUE;

	


	if ( !(*msg & (1<<TWI_READ_BIT)) )									// The LSB in the address byte determines if is a masterRead or masterWrite operation.
	{USI_TWI_state.masterWriteDataMode = TRUE;}

																		// Release SCL to ensure that (repeated) Start can be performed
	PORT_USI |= (1<<PIN_USI_SCL);										// Release SCL.
	while( !(PIN_USI & (1<<PIN_USI_SCL)) );								// Verify that SCL becomes high.
	
	_delay_us( T2_TWI );												// Delay for T2TWI if TWI_STANDARD_MODE
	

																		// Generate Start Condition */
	PORT_USI &= ~(1<<PIN_USI_SDA);										// Force SDA LOW.
	_delay_us( T4_TWI );
	PORT_USI &= ~(1<<PIN_USI_SCL);										// Pull SCL LOW.
	PORT_USI |= (1<<PIN_USI_SDA);										// Release SDA.

	
																		//Write address and Read/Write data
	do
	{
		/* If masterWrite cycle (or initial address transmission)*/
		if (USI_TWI_state.addressMode || USI_TWI_state.masterWriteDataMode)
		{
																					// Write a byte
			PORT_USI &= ~(1<<PIN_USI_SCL);											// Pull SCL LOW.
			USIDR     = *(msg++);													// Setup data.
			USI_TWI_Master_Transfer( tempUSISR_8bit );								// Send 8 bits on bus.
			
			
																					//Clock and verify (N)ACK from slave
			DDR_USI  &= ~(1<<PIN_USI_SDA);											// Enable SDA as input.
			if( USI_TWI_Master_Transfer( tempUSISR_1bit ) & (1<<TWI_NACK_BIT) )
			{
				if ( USI_TWI_state.addressMode )
				USI_TWI_state.errorState = USI_TWI_NO_ACK_ON_ADDRESS;
				else
				USI_TWI_state.errorState = USI_TWI_NO_ACK_ON_DATA;
				return (FALSE);
			}
			USI_TWI_state.addressMode = FALSE;            // Only perform address transmission once.
		}
		/* Else masterRead cycle*/
		else
		{
			/* Read a data byte */
			DDR_USI   &= ~(1<<PIN_USI_SDA);               // Enable SDA as input.
			*(msg++)  = USI_TWI_Master_Transfer( tempUSISR_8bit );

			/* Prepare to generate ACK (or NACK in case of End Of Transmission) */
			if( msgSize == 1)                            // If transmission of last byte was performed.
			{
				USIDR = 0xFF;                              // Load NACK to confirm End Of Transmission.
			}
			else
			{
				USIDR = 0x00;                              // Load ACK. Set data register bit 7 (output for SDA) low.
			}
			USI_TWI_Master_Transfer( tempUSISR_1bit );   // Generate ACK/NACK.
		}
	}while( --msgSize) ;                             // Until all data sent/received.
	
	USI_TWI_Master_Stop();                           // Send a STOP condition on the TWI bus.

	/* Transmission successfully completed*/
	return (TRUE);
}



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
unsigned char USI_TWI_Master_Stop( void )
{
	PORT_USI &= ~(1<<PIN_USI_SDA);           // Pull SDA low.
	PORT_USI |= (1<<PIN_USI_SCL);            // Release SCL.
	while( !(PIN_USI & (1<<PIN_USI_SCL)) );  // Wait for SCL to go high.
	_delay_us( T4_TWI );
	PORT_USI |= (1<<PIN_USI_SDA);            // Release SDA.
	_delay_us( T2_TWI );
	
	#ifdef SIGNAL_VERIFY
	if( !(USISR & (1<<USIPF)) )
	{
		USI_TWI_state.errorState = USI_TWI_MISSING_STOP_CON;
		return (FALSE);
	}
	#endif

	return (TRUE);
}