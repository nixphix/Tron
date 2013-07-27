#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>

#include "dep/font5x7.h"
#include "dep/functions.c"

#define DISP_ROW_CNT 7
#define DISP_COL_CNT 80// it takes 7 columns for a character but frst 5 is only data and next 2 is null

#define HC595_PORT 	PORTB
#define HC595_DDR 	DDRB

#define HC595_DS_POS PB0
#define HC595_SH_CP_POS PB1
#define HC595_ST_CP_POS PB2

#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))
#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))

volatile PGM_P ptr=smallFont;
volatile uint8_t len;  // max number of char in message

//Message to display
volatile char message[80],messageA[40],messageB[40] ;//={0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,'H','O','M','E',0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,'A','W','A','Y',0x1F,0x1F,0x1F,0x1F,0x1F,};
volatile char teamA[6], teamB[6], REFRESH_FLAG = 0 ;//updated by USART RXC ISR


void HC595Init(void);
void HC595Pulse(void);
void HC595Latch(void);
void SelectRow(uint8_t);
void BuildMsg(void);
void BuildStringA(uint8_t lpad,uint8_t rpad,char msg[]);
void BuildStringB(uint8_t lpad,uint8_t rpad,char msg[]);


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
	//PORTD&=(~(1<<PD6)); PC5 is assigned 
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
			PORTC|=(1<<PC5);
			break;
		case 6:
			PORTD|=(1<<PD7);
			break;
	}
}
	
void BuildMsg()
{
  uint8_t len = strlen(teamA),lpad=0,rpad=0;

    switch(len)
	{
	   case 0:
	    lpad=7;rpad=6;
	    strcpy(teamA,"HOME");
	    break;
		
	   case 1:
	    lpad=rpad=17;
		break;
		
	   case 2:
	    lpad=14;rpad=13;
		break;
		
	   case 3:
	    lpad=rpad=10;
		break;
		
	   case 4:
	    lpad=7;rpad=6;
		break;
		
	   case 5:
	    lpad=rpad=3;
		break;
		
	   case 6:
	    lpad=rpad=0;
		break;
		
	   default:
        lpad=rpad=0;	
	}
    BuildStringA(lpad,rpad,teamA);
	
	
	len = strlen(teamB);
    switch(len)
	{
	   case 0:
	    lpad=6;rpad=7;
	    strcpy(teamB,"AWAY");
	    break;
		
	   case 1:
	    lpad=rpad=17;
		break;
		
	   case 2:
	    lpad=13;rpad=14;
		break;
		
	   case 3:
	    lpad=rpad=10;
		break;
		
	   case 4:
	    lpad=6;rpad=7;
		break;
		
	   case 5:
	    lpad=rpad=3;
		break;
		
	   case 6:
	    lpad=rpad=0;
		break;
		
	   default:
        lpad=rpad=0;	
	}
    BuildStringB(lpad,rpad,teamB);
	
	strcpy(message,strupr(messageA));
	strcat(message,strupr(messageB));
	
}

void BuildStringA(uint8_t lpad,uint8_t rpad,char msg[])
{
  uint8_t msglen = strlen(msg)>6?6:strlen(msg);
  int8_t index = 0;
  for(int8_t i=lpad+rpad+msglen,j=0;i>0;index++,i--)
  {
     if(lpad)
	 {
	   lpad--;
	   messageA[index] = 0x1F; 
	 }
	 else if(msglen)
	 {
	   messageA[index] = msg[j++];
	   msglen--;
	 }
	 else if(rpad)
	 {
	   rpad--;
	   messageA[index] = 0x1F; 
	 }
  }  
  messageA[++index] = '\0'; // To mark end of String 
 
}

void BuildStringB(uint8_t lpad,uint8_t rpad,char msg[])
{
  uint8_t msglen = strlen(msg)>6?6:strlen(msg);
  int8_t index = 0;
  for(int8_t i=lpad+rpad+msglen,j=0;i>0;index++,i--)
  {
     if(lpad)
	 {
	   lpad--;
	   messageB[index] = 0x1F; 
	 }
	 else if(msglen)
	 {
	   messageB[index] = msg[j++];
	   msglen--;
	 }
	 else if(rpad)
	 {
	   rpad--;
	   messageB[index] = 0x1F; 
	 }
  } 
  messageB[++index] = '\0'; // To mark end of String 
  
}
	
int main(void)
{
	
	TIMSK=(1<<TOIE1); 
	TCNT1=0xFFC0; 
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); 
	DDRC=0xFF;
	DDRD=0XF0;
	
	BuildMsg();
	
	USART_Init(103);
	USART_Intr();
	len = strlen(message);
	HC595Init();

	sei();
	//ii=iii=0; not used
	
	while(1)
	{
	  if(REFRESH_FLAG==1)
	  {
	    BuildMsg();
	    len = strlen(message);
	    REFRESH_FLAG = 0;
	  }
	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{
	PORTC=0x00;
	PORTD&=(~(1<<PD7));
	//PORTD&=(~(1<<PD6)); PC5 is assigned to this
	TCNT1=0xFFC0;	
	static uint8_t row;
	
	int8_t col;
	int8_t chr = 0;						//iii;
	int8_t m=0;							//ii;
	uint8_t data;


	for(col=0;col<DISP_COL_CNT;col++)
	{
	
		if(message[chr] == 0x1F)  
		{
			data = 0x00;
			chr++;
			m--;
		}
		else if(m<5)
		{
			
			data=pgm_read_byte(( ptr+((message[chr]-' ')*5)+m));
		}
		else
		{
			data = 0x00;
		}
	
		if((data & (1<<row)))
			HC595DataHigh();
		else
			HC595DataLow();

		HC595Pulse();

		if(++m==7) // the number of columns for a single character
		{
			chr++;
			m=0;

			if(chr >=32)
				{chr=0;}
		}			
		
	}
	HC595Latch();
	SelectRow(row);
	

	if(++row==DISP_ROW_CNT)
	{
		
		row=0;
	}
}

ISR(USART_RXC_vect) // 0 - Addr, 1 - Data, 2 - CHKSUM
{
     RXC_ISR_DATA[RXC_ISR_INDEX]=UDR;
   RXC_ISR_INDEX++;
 
   switch(RXC_ISR_INDEX)
   {
	  case 1:
	  
	       CHK_SUM=(0x0F&RXC_ISR_DATA[0]);
		break;	
      case 2:
       		
   	       CHK_SUM^=RXC_ISR_DATA[1];
        break;
	  case 3:
	       if(RXC_ISR_DATA[2]==CHK_SUM)
		    {
			  switch(RXC_ISR_DATA[0])
			   {  
				  case 100://NBA
				  case 101:
				  case 102:
				  case 103:
				  case 104:
				  case 105:
				     teamA[RXC_ISR_DATA[0]-100]=RXC_ISR_DATA[1];
					 REFRESH_FLAG=1;
					 break;
					 
				  case 150://NBB
				  case 151:
				  case 152:
				  case 153:
				  case 154:
				  case 155:
				     teamB[RXC_ISR_DATA[0]-150]=RXC_ISR_DATA[1];
					 REFRESH_FLAG=1;
					 break;			   
			   }
			}
	  default:	
		   RXC_ISR_INDEX=0;	
   }     
}