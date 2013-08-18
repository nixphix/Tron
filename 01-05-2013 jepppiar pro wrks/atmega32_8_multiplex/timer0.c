#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "include_me/global.c"
#include "include_me/system.c"
#include "include_me/tronicman.h"
#define  F_CPU 16000000UL

volatile int minutes = 10,seconds=0;

int main(void)

 {	
	SYS_INIT();
	USART_Init(103);
	USART_Intr();
	Timer1Init();
	sei();		   

   while(1)							//Infinie loop
  
   {
   
		if(GCSP==1)
		{Timer1DIS();}
        
		if(GC!=2)
		{
			seconds=0;
			if(GC==9)
			{
				minutes--;
			}
			else if(GC==0)
			{
				minutes=10;
			}
			else if(GC==1)
			{
				minutes++;
				if(minutes>60)
				{minutes=0;}
			}
			GC=2;
		}
         
		display1(BT);
				
   		PORTC = 0b10000000;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
         
		display1(BF); 		 
		scoAdigit(ASH,ARO);
		 
		PORTC = 0b01000000;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
		 
	    	
	    display1(QT);
		scoBdigit(BSH,ARO);
		 
		PORTC = 0b00100000;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);	
		
		display1(AF);
		display2(AS/10);
		
		PORTC = 0b00010000;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
		
		display1(seconds%10);
		display2(AS%10);
		
		PORTC = 0b00001000;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
		
		display1(seconds/10);
		display2(BS/10);
		
		PORTC = 0b00000100;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
		
		display1(minutes%10);
		display2(BS%10);
		
		PORTC = 0b00000010;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
		
		display1(minutes/10);
		display2(AT);
		
		PORTC = 0b00000001;
		 _delay_ms(1);
		PORTC = 0b00000000;
		_delay_us(100);
		
		

		
		if(minutes == -1)
		{
			//buz=0;
			Timer1DIS(); //TIMSK&=~(1<<TOIE1); // Disabled global and timer overflow interrupt;
			minutes = 10;
			GCSP=1;
			seconds=0;
			if(GC_SET!=1)
			 {
			  Buzz(4);
			  //buzzer=1;
			 }
		}		
 
	}
	return 0;
 }

ISR(TIMER0_OVF_vect) // for buzzer
{
  buzCount++;
  if(buzCount>244) //4s 4*61
  {
	 PORTD &= ~(_BV(PD6));
	 Timer0DIS();
	 buzCount=0;
  }
}

ISR(TIMER1_OVF_vect) 
{
  TCNT1=0xBDC; // set initial value to remove time error (16bit counter register)
  seconds--;
	if(seconds == -1)
	{
		seconds = 59;
		minutes--;
	}
}

ISR(USART_RXC_vect) // @ brd side
{

   RXC_ISR_DATA[RXC_ISR_INDEX]=UDR;
   RXC_ISR_INDEX++;
   // PORTD ^= 0xff;
   //_delay_ms(100);
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
				  case 200:// #
					   //display(5);
				       AS=RXC_ISR_DATA[2];
					   // if(AS>99){ASH=1;}
					   // else{ASH=0;}
					   if(AS>99)
						{
						  ASH=1;// 1
						  AS=AS%100;//00
						}
					   else
						{
						 ASH=0;
						}
				  break;
				    
				  case 201: //$
					   AF=RXC_ISR_DATA[2];
				  break;
				   
				  case 202: //%
					   AT=RXC_ISR_DATA[2];
				  break;
				    
				  case 210: //&
					   BS=RXC_ISR_DATA[2];
					  //  if(BS>99){BSH=1;}
					  //  else{BSH=0;}
					   	 if(BS>99)
						 {
						   BSH=1;  // 1
						   BS=BS%100;//00
						 }
						 else
						 {
						   BSH=0;
						 }
				  break;
				  
				  case 211: //'
					   BF=RXC_ISR_DATA[2];
				  break;
				  
				  case 212: //(
					   BT=RXC_ISR_DATA[2];
				  break;
				  
				  case 220: //)
					   QT=RXC_ISR_DATA[2];
					   if(QT>4)
					   {QT=10;}
				  break;
				  
				  case 221: //)
					   ARO=RXC_ISR_DATA[2];
					   
				  break;
				  
				  case 222: //)
				  
					   GCSP=RXC_ISR_DATA[2];
					   GC_SET=0;
					   if(GCSP==1)
						{
						    Timer1DIS();
						}
						else
						{
							Timer1EN(); //TIMSK|=(1<<TOIE1); // Enabled global and timer overflow interrupt;
						}
				  break;
				  
				  case 223: //)
					   GC=RXC_ISR_DATA[2];
					   GC_SET=1;
					   GCSP=1;
				  break;
				  
				  case 100://NBA
				  case 101:
				  case 102:
				  case 103:
				  case 104:
				  case 105:
				  case 106:
				  case 150://NBB
				  case 151:
				  case 152:
				  case 153:
				  case 154:
				  case 155:
				  case 156:
				    NB_Tx(RXC_ISR_DATA[1],RXC_ISR_DATA[2]); // 3 byte packet
				    break;
					
				  case 80:
                    Buzz(RXC_ISR_DATA[2]);
					break;
				  default :
                    NB_Tx(RXC_ISR_DATA[1],RXC_ISR_DATA[2]); // 3 byte packet	
			   }
			}
	  default:	
	       
		   RXC_ISR_INDEX=0;	
        	
   }
}
