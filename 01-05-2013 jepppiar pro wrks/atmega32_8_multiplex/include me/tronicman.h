/* Copyright (c) 2012 Tronicman LLP

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

/* $Id:tronicman.h,v 0.1.03 2012/7/31 12:45:07 dmix Exp $ */

/*
   tronicman.h - tronicman proprietary functions

   Author : surya
            surya@tronicman.com
 */
 
#ifndef TRONICMAN_H_INCLUDED
#define TRONICMAN_H_INCLUDED
/**
 
            This header file declares tronicman proprietary constants and
            functions.
 
 **/
 /******** DEFINABLES ********/
 
#ifndef KEYPAD_PORT 
 //#warning "KEYPAD PORT NOT DEFINED: ASSIGNING DEFAULT PORT A" 
 #define KEYPAD_PORT PORTA  // ASSIGN PORTA AS DEFAULT PORT
#endif

#ifndef KEYPAD_DDR 
 #define KEYPAD_DDR DDRA  
#endif

#ifndef KEYPAD_PIN 
 #define KEYPAD_PIN PINA
#endif
 /******** DEFINABLES ********/

#if !defined(__DOXYGEN__)
// src alpha
static inline void t1_init(void) __attribute__((always_inline));
static inline void t1_rst(void) __attribute__((always_inline));
// src matrix for 4x4 mat kb
static inline void USART_Init(unsigned int __baud) __attribute__((always_inline));
static inline void USART_Transmit(uint8_t __data) __attribute__((always_inline));
static inline uint8_t USART_Receive(void) __attribute__((always_inline));
static inline uint8_t GetKeyPressed(void) __attribute__((always_inline));
static inline uint8_t match(uint8_t __keypress) __attribute__((always_inline));
// packet data
static inline void USART_Intr(void) __attribute__((always_inline));
static inline void USART_Tx(uint8_t __Addr,uint8_t __data) __attribute__((always_inline));
//static inline uint8_t USART_Rx(void) __attribute__((always_inline));
#endif
void
t1_init(void)
{
   
   TCCR1B=(1 << WGM12);                // timer 1 in CTC mode
   TCCR1B|=(1<<CS10)|(1<<CS12);     // Prescaler = FCPU/1024
   OCR1A= 15625;                // compare match val for A 
   TIMSK|= (1<<OCIE1A);              //  compare interrupt : ISR(TIMER1_COMPA)
  					     // Initialize Counter

}

void
t1_rst(void)
{
   TCNT1=0;
}

void 
USART_Init( unsigned int __baud )
{
		/* Set baud rate */
		UBRRH = (uint8_t)(__baud>>8);
        UBRRL = (uint8_t)__baud;
		/* Enable receiver and transmitter */
		UCSRB = (1<<RXEN)|(1<<TXEN);
		/* Set frame format: 8data, 2stop bit */
		UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void 
USART_Intr(void)
{
        UCSRB|=(1<<RXCIE); 
}

void 
USART_Transmit( uint8_t __data )
{
		/* Wait for empty transmit buffer */
		while ( !( UCSRA & (1<<UDRE)) );
		
		/* Put data into buffer, sends the data */
		UDR = __data;
}

uint8_t 
USART_Receive(void)
{
		/* Wait for data to be received */
		while (!(UCSRA & (1<<RXC)));
		
		/* Get and return received data from buffer */
		return UDR;
}

 void 
 USART_Tx(uint8_t __Addr,uint8_t __data)
 {
   uint8_t __i,__Pkt[4]={0x53,__Addr,__data,0x45} ;
   for(__i=0;__i<4;__i++)
    {   
	  while ( !( UCSRA & (1<<UDRE)) );
		/* Put data into buffer, sends the data */
		UDR = __Pkt[__i];
	}
 }
 
uint8_t 
GetKeyPressed(void)
 {
	int __r,__c;

	KEYPAD_PORT|= 0X0F;

	 for(__c=0;__c<4;__c++)
      {
         KEYPAD_DDR&=~(0XFF); // DDRA input
         KEYPAD_DDR|=(0X80>>__c);// making one particular column as output
         for(__r=0;__r<4;__r++)
         {
            if(!(KEYPAD_PIN & (0X08>>__r)))
            {
			 
              return (__r*4+__c);
			   
            }
         }
      }

	return 0xFF;//Indicate No keypressed
}

uint8_t 
match( uint8_t __keypress)
 {
  uint8_t __keyPressed;
  switch (__keypress)			   
		   {
		   	case (0xee): __keyPressed = 16; 
				 		 break;
			case (0x0C): __keyPressed = 1;
				 		 break;
			case (0x0D): __keyPressed = 2; 
			   			 break;
			case (0x0E): __keyPressed = 3; 
				 		 break;
			case (0x0F): __keyPressed = 10; 
				 		 break;
			case (0x08): __keyPressed = 4; 
				 		 break;
			case (0x09): __keyPressed = 5; 
				 		 break;
			case (0x0A): __keyPressed = 6; 
				 		 break;
			case (0x0B): __keyPressed = 11; 
				 		 break;
			case (0x04): __keyPressed = 7; 
				 		 break;
			case (0xbb): __keyPressed = 8; 
				 		 break;
			case (0xb7): __keyPressed = 9; 
				 		 break;
			case (0x7e): __keyPressed = 12; 
				 		 break;
			case (0x00): __keyPressed = 14; 
				 		 break;
			case (0x7b): __keyPressed = 0; 
				 		 break;
			case (0x77): __keyPressed = 15; 
				 		 break;
			case (0xFF): __keyPressed = 16;
						 break;
			//default : __keyPressed = 'X';
			}
			return __keyPressed;
 }
 


#endif
