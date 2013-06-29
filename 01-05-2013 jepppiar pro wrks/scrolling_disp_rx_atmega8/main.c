#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "font5x7.h"
#include "g.c"
#include "global.c"

volatile PGM_P ptr=smallFont;

volatile uint16_t ii,iii;

//Message to display
volatile uint8_t string[16]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'};
volatile uint8_t len;


#define DISP_ROW_CNT 7
#define DISP_COL_CNT 76// it takes 7 columns for a character but frst 6 is only data and next 1 is null

#define HC595_PORT 	PORTB	
#define HC595_DDR 	DDRB

#define HC595_DS_POS PB0 // 1
#define HC595_SH_CP_POS PB1 //2
#define HC595_ST_CP_POS PB2  //3

#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))
#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))

void HC595Init(void);
void HC595Pulse(void);
void HC595Latch(void);
void SelectRow(uint8_t);

void HC595Init()
{
	HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));
}



void HC595Pulse()
{
	
	HC595_PORT|=(1<<HC595_SH_CP_POS);
	HC595_PORT&=(~(1<<HC595_SH_CP_POS));
}

void HC595Latch()
{
	HC595_PORT|=(1<<HC595_ST_CP_POS);
	_delay_loop_1(1);
	HC595_PORT&=(~(1<<HC595_ST_CP_POS));
	_delay_loop_1(1);
}



void SelectRow(uint8_t r)
{
	
	PORTD&=(~(1<<PD7));
	PORTD&=(~(1<<PD6));
	PORTC&=0x00;

	switch(r)
	{
		case 0:
			PORTC|=(1<<PC0);
			break;
		case 1:
			PORTC|=(1<<PC1);
			break;
		case 2:
			PORTC|=(1<<PC2);
			break;
		case 3:
			PORTC|=(1<<PC3);
			break;
		case 4:
			PORTC|=(1<<PC4);
			break;
		case 5:
			PORTD|=(1<<PD6);
			break;
		case 6:
			PORTD|=(1<<PD7);
			break;
	}
}
		
int main(void)
{
	USART_Init(103);
	TIMSK=(1<<TOIE1); 
	TCNT1=0xFFC0; 
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); 
	DDRC=0xFF;
	DDRD=0XF0;
	sei();
	HC595Init();
	len=strlen(string);
	ii=iii=0;
	int i=0,PA=0,PAF=0,PAB=0,PAL=0,PB=0,PBF=0,PBB=0,PBL=0;
	
  while(1)
  {
	
   if(NAME_CMD==0x01) 
    {
       cli();
	   PA=0;
	   for(i=CHARS;i>0;i--)
         {
		    if(NBA[i]==0x0)
			 {
			   PA++;
			 }
			else
			 {
			   break;
             } 
	     }	 
       PAF = PA/2;
       PAL = CHARS-PA;
       PAB = PA-PAF; 
	   
	   for(i=0;i<PAF;i++)
         {
		     string[i]=" "; 
	     }	 		
       
       for(i=0;i<PAL;i++)
         {		     
			 string[PAF+i]=NBA[i]; 
	     }
 
 	   for(i=0;i<PAB;i++)
         {
		     string[PAF+PAL+i]=" "; 
	     }	
 
	   PB=0;
	   for(i=CHARS;i>0;i--)
         {
		    if(NBB[i]==0x0)
			 {
			   PB++;
			 }
			else
			 {
			   break;
             } 
         }  
		 
       PBF = PB/2;
       PBL = CHARS-PB;
       PBB = PB-PBF; 
	   
	   for(i=0;i<PBF;i++)
         {
		     string[8+i]=" "; 
	     }	 		
       
       for(i=0;i<PBL;i++)
         {		     
			 string[8+PBF+i]=NBB[i]; 
	     }
 
 	   for(i=0;i<PBB;i++)
         {
		     string[8+PBF+PBL+i]=" "; 
	     }
		 
	   NAME_CMD=0x00;
	   sei();
    } 
	
  }
	
	return 0;
	
}



ISR(USART_RXC_vect)
{
	//__i=0;
   RXC_ISR_DATA[RXC_ISR_INDEX]=UDR;
   
     if(RXC_ISR_INDEX==1)
       { 
		  switch(RXC_ISR_DATA[0])
		   {
			  case 100:
				   NBA[0]=RXC_ISR_DATA[1];
			  break;
			  
			  case 101: 
				   NBA[1]=RXC_ISR_DATA[1];
			  break;
			  
			  case 102:
				   NBA[2]=RXC_ISR_DATA[1];
			  break;
			  
			  case 103:
				   NBA[3]=RXC_ISR_DATA[1];
			  break;
			  
			  case 104:
				   NBA[4]=RXC_ISR_DATA[1];
			  break;
			  
			  case 105:
				   NBA[5]=RXC_ISR_DATA[1];
			  break;
			  
			  case 106:
				   NBA[6]=RXC_ISR_DATA[1];
			  break;
			  
			  case 107:
				   NBA[7]=RXC_ISR_DATA[1];
			  break;
				   	
			  case 150:
				   NBB[0]=RXC_ISR_DATA[1];
			  break;
			  
			  case 151: 
				   NBB[1]=RXC_ISR_DATA[1];
			  break;
			  
			  case 152: 
				   NBB[2]=RXC_ISR_DATA[1];
			  break;
			  
			  case 153: 
				   NBB[3]=RXC_ISR_DATA[1];
			  break;
			  
			  case 154: 
				   NBB[4]=RXC_ISR_DATA[1];
			  break;
			  
			  case 155: 
				   NBB[5]=RXC_ISR_DATA[1];
			  break;
			  
			  case 156: 
				   NBB[6]=RXC_ISR_DATA[1];
			  break;
			  
			  case 157: 
				   NBB[7]=RXC_ISR_DATA[1];
			  break;
			  
			  case 180:
			       NAME_CMD=0x01;
			  break;	   
		   }
         RXC_ISR_INDEX=0;	       
	   }       
  RXC_ISR_INDEX++;	
}



ISR(TIMER1_OVF_vect)
{
	PORTC=0x00;
	PORTD&=(~(1<<PD7));
	PORTD&=(~(1<<PD6));
	TCNT1=0xFFC0;	
	static uint8_t row;
	static uint16_t cnt=0;cnt++;

	/*if(cnt ==80)
	{
		cnt =0;

		ii++;
		if(ii==5)
		{
			ii=0;
			iii++;
			if(iii>len)
				iii=0;
		}

		return;
	}*/

	
	int8_t col;

	int8_t chr = 0;						//iii;
	int8_t m=0;							//ii;

	for(col=0;col<DISP_COL_CNT;col++)
	{
		uint8_t data;
		
		if(m!=5)
		{
			data=pgm_read_byte(( ptr + ((string[chr]-' ')*5)+m));
		}
		else  
		{
			data=0x00;
		}
	
		if((data & (1<<row)))
			HC595DataHigh();
		else
			HC595DataLow();

		HC595Pulse();
		

		m++;

		if(m==6)
		{
			chr++;
			m=0;

			if(chr >=len)
				chr=0;
		}			
		
	}
	HC595Latch();
	SelectRow(row);
	

	row++;
	if(row==DISP_ROW_CNT)
	{
		
		row=0;
	}
}
