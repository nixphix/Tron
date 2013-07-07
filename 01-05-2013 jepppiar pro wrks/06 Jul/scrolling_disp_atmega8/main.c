#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "font5x7.h"

volatile PGM_P ptr=smallFont;

//volatile uint16_t ii,iii; not used

//Message to display
volatile char msg[]=" HOME  AWAY ";
volatile uint8_t len;


#define DISP_ROW_CNT 7
#define DISP_COL_CNT 80// it takes 7 columns for a character but frst 5 is only data and next 2 is null

#define HC595_PORT 	PORTB	
#define HC595_DDR 	DDRB

#define HC595_DS_POS PB0
#define HC595_SH_CP_POS PB1
#define HC595_ST_CP_POS PB2

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
		
int main(void)
{
	
	TIMSK=(1<<TOIE1); 
	TCNT1=0xFFC0; 
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); 
	DDRC=0xFF;
	DDRD=0XF0;
	sei();
	HC595Init();
	len=strlen(msg);
	//ii=iii=0; not used
	while(1)
	{
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
	//static uint16_t cnt=1; not used
	
	int8_t col;

	int8_t chr = 0;						//iii;
	int8_t m=0;							//ii;

	for(col=0;col<DISP_COL_CNT;col++)
	{
		uint8_t data;
		
		if(m<5)
		{
			
			data=pgm_read_byte(( ptr+((msg[chr]-' ')*5)+m));
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


		if(++m==7) // the number of columns for a single character
		{
			chr++;
			m=0;

			if(chr >=len)
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
