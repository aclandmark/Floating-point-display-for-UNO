
//C program for implementation of ftoa()
#include <math.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "Resources\askii_subroutines.c"


#define wdr()  __asm__ __volatile__("wdr")
#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}
  
  #define setup_HW \
  CLKPR = (1 << CLKPCE);\
CLKPR = (1 << CLKPS0);\
  setup_watchdog;\
  USART_init(0,25);
  
void sendStringF(const char*);
void sendStringV(char*);
void sendChar(const char);
char waitforkeypress(void);
