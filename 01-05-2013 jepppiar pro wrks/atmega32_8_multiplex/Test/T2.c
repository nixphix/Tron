#define dis2_PORT PORTB

#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#include "include_me/global.c"
#include "include_me/system.c"
#include "include_me/tronicman.h"
#define F_CPU 16000000UL

volatile int minutes = 10,seconds=0;

void display(uint8_t e)		//Define a function to display the number passed on the seven segment display
{
	switch(e)			//Open a switch case
	{
		case 1: 
		dis2_PORT = 0b00110000;
		break;			
	 
		case 2: 
		dis2_PORT = 0b01101101;
		break;
		
		case 3: 
		dis2_PORT = 0b01111001;
		break;
		
		case 4: 
		dis2_PORT = 0b00110011;
		break;
		
		case 5: 
		dis2_PORT = 0b01011011;
		break;
		
		case 6: 
		dis2_PORT = 0b01011111;
		break;
		
		case 7: 
		dis2_PORT = 0b01110000;
		break;
		
		case 8: 
		dis2_PORT = 0b01111111;
		break;
		
		case 9: 
		dis2_PORT = 0b01110011;
		break;
		
		case 0: 	
		dis2_PORT = 0b01111110;
		
	}

}


int main(void)
 {	
	SYS_INIT();
	USART_Init(103);
	USART_Intr();
	TIMSK=(1<<TOIE1); // enabled global and timer overflow interrupt;
	TCNT1=0xBDB; // set initial value to remove time error (16bit counter register)
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
	sei();		
   
   PORTA = 0x55;
   while(1)							//Infinie loop
   { 
     display(RXC_ISR_DATA[0]);
	 for(int  i =0;i<8;i++)
	 {
	   PORTC = (1<<i);
	   _delay_us(2);
	   PORTC = 0x00;
	   _delay_us(6);
	 }
	 
   }
   return 0;
 }

ISR(TIMER1_OVF_vect) 
{
	
  TCNT1=0xBDC; // set initial value to remove time error (16bit counter register)
  if(GCSP==0)
  {
    seconds--;
  }
  else 
  {
    buz++;
  }
}

ISR(USART_RXC_vect) // @ brd side
{

   RXC_ISR_DATA[0]=UDR;
   PORTA^=0xFF;
}
