#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#include "global.c"
#include "function.c"
#define F_CPU 16000000UL

volatile int seconds=24;

int main(void)

 {	
	SYS_INIT();
	USART_Init(103);
	USART_Intr();
	TIMSK=(1<<TOIE1); // enabled global and timer overflow interrupt;
	TCNT1=0xBDB; // set initial value to remove time error (16bit counter register)
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
	sei();		
   

   while(1)							//Infinie loop
   {
		/*if(GC!=2)
		{
			seconds=24; // any chnages in the game clock should reset the 24 sec timer, remove the three cases after testing.
			if(GC==9)
				{
				seconds=24;
				}
			else if(GC==0)
				{
				seconds=24;
				}
			else if(GC==1)
				{
				seconds=24;
				}
				GC=2;
				
		}
		
		if(TF!=5)
		{
		
		if(TF==0)
		{
		seconds=24;
		}
		else if(TF==1)
		{
		GCSP=1;
		}
		else if(TF==2)
		{
		seconds=24;
		GCSP=1;
		}
		TF=5;
		}
		
	
		if(TH==3)
		{
		seconds=13;
		TH=5;
		}*/
		
		DataLine(seconds%10);
		
		
		SelectLine(1); // ones digit
    	_delay_ms(1);
		SelectLine(0); // reset digits
		
		_delay_us(100);
		
		DataLine(seconds/10);
		
		
		SelectLine(2); // tens digit
    	_delay_ms(1);
		SelectLine(0); // reset digits
		_delay_us(100);
		
		
		
		if(seconds == -1)
			{
				GCSP=1;
				seconds = 24;
				SelectLine(3);
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
}


ISR(USART_RXC_vect) // @ brd side
{

   RXC_ISR_DATA[RXC_ISR_INDEX]=UDR;
   RXC_ISR_INDEX++;
 
   switch(RXC_ISR_INDEX)
   {
      case 1:
	    
		 if(RXC_ISR_DATA[0]==START_BYTE)
		  {    
		   CHK_SUM=0;   
		  }
		 else
		  {
		   RXC_ISR_INDEX=0;		  
		  }
		break;
	  case 2:
	  
	       CHK_SUM=(0x0F&RXC_ISR_DATA[1]);
		break;	
      case 3:
       		
   	       CHK_SUM^=RXC_ISR_DATA[2];
        break;
	  case 4:
	       if(RXC_ISR_DATA[3]==CHK_SUM)
		    {
			  switch(RXC_ISR_DATA[1])
			   {
				 
				  
				  
				  case 222: 
					   GCSP^=1;
					   
				  break;
				  
				  case 223: 
					   GCSP=1;
				  break;
				  
				  case 224: 
						seconds=24;
						GCSP=0;
				  
					   //TF==RXC_ISR_DATA[2];
					  
				  break;
				  
				  case 225: 
				  seconds=13;
						GCSP=0;
											   
				  break;
				  
				  
			   }
			}
	  default:	
	       
		   RXC_ISR_INDEX=0;	
        	
   }
}