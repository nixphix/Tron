#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include<inttypes.h>
#define F_CPU 16000000UL
#include "global.c"
#include<main.h>
#include<GLCD.c>

#include<sportronix.h>


void USART_Init( unsigned int);
void USART_Transmit( unsigned int);
unsigned char USART_Receive( void );
uint8_t keypad_4keys(void);
int y=0, i=0,_i=0;


void USART_Init( unsigned int baud )
{
		/* Set baud rate */
		UBRR0H = (unsigned char)(baud>>8);
		UBRR0L = (unsigned char)baud;
		/* Enable receiver and transmitter */
		UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0 );
		/* Set frame format: 8data, 2stop bit */
		UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(3<<UCSZ00);
		
		
}

void USART_Transmit( unsigned int data )
{
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) );
		
		/* Put data into buffer, sends the data */
		UDR0 = data;
}

unsigned char USART_Receive( void )
{

		/* Wait for data to be received */
		while ( !(UCSR0A & (1<<RXC0)) );
		
		/* Get and return received data from buffer */
		
		return UDR0;
}


void t1_rst(void)
			{
			TCNT1=0xBDB;
			o_sec=0;
			}
			
void main()
{

	USART_Init(103);
	TIMSK=(1<<TOIE1); // enabled global and timer overflow interrupt;
	TCNT1=0xBDB; // set initial value to remove time error (16bit counter register)
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
	sei();
	DDRA=0x0F;
	PORTA=0X0F;
	DDRB=0x00;
	DDRD=0xFF;
	
	GLCD_Init128();	
	picture(&sportronix[0]);
	
	
	
	while(1)
	{
	_av=keypad_4keys();
	
	// get into this loop only when the home button is pressed, also clear the existing team names since there s no option for backspace
	
	
	
	if(menu==1)
	{
						// for displaying teama name
		
	lcdputs2(16,3,ar7);		
	setcolumn(40);
	setpage(4);
	
	for(int k=0;((k<10));k++)
	{
	
	if(teama[k]!='0')
	{
	lcdsim_chardata((40+(k*8)),4,teama[k]);
	}
	else {lcd_bs(6);}
	}
	}
	
	
	
	else if(menu==2)			// for displaying teamb name
	{
	
	lcdputs2(16,3,ar8);		
	setcolumn(40);
	setpage(4);

	for(int k=0;((k<10));k++)	//&(teamb[k]!='0')
	{
	if(teamb[k]!='0')
	{lcdsim_chardata((40+(k*8)),4,teamb[k]);}
	else {lcd_bs(6);}
	}
	}
	
	
	// now analyze the team names and put them into a PROPER array format.
	
	
	
	
	if(_av==0)			// buzzer USART_Transmit(0);
	{
	if(_i<6){_i--;if(_i<0){_i=0;}teama[_i]='0';}
	}
	
	else if(_av==1)   // A-Z
	{
	if(o_sec==1)
	{
	_ch=0;
	}
	
	_ch++;
	km = keymap[_ch];
	
	if(o_sec==0)
	{
	if(_i<6)
	{_i--; if(_i<0){_i=0;}}
	}
	
	
	teama[_i]=(km);
	_i++;
	
	
	//USART_Transmit(1);
	
	t1_rst();
	}
	
	else if(_av==2) // home button
	{
	//_ch=0;
	menu++;
	if(menu!=0)
	{
	clrlcd();
	_delay_ms(5);
	}
	if(menu>=3)
	{menu=0;												// exit out of the menu and capture the team names
	
	_ch=0;
	}
	
	USART_Transmit(2);
	}
	else if(_av==3)		// Z-A
	{
	USART_Transmit(3);

	}
	


}
}
ISR(TIMER1_OVF_vect) 
{
	
  TCNT1=0xBDC; // set initial value to remove time error (16bit counter register)
  //PORTD^=0xFF; // Here braodcast the 1 sec pulse to the slave boards.
  o_sec=1;
  
}

	
void debounce(void)
{
_delay_ms(10);
}


uint8_t check_key_4keys(void)
{
	
	PORTA =0b11111110;
	
	debounce();
	if(bit_is_clear(PINB,c1))
	return(3);
	else if(bit_is_clear(PINB,c2))
	return(2);
	else if(bit_is_clear(PINB,c3))
	return(1);
	else if(bit_is_clear(PINB,c4))
	return(0);
	
	return 0xFF;
}




uint8_t keypad_4keys(void)
{
		PORTA=0x00;
		PORTB=0xFF;		//set all the input to one
		
		_nkey_=check_key_4keys();
		
		
		
		if(_nkey_ == d_nkey) { USART_Transmit(9); }
			
		if (_nkey_ == 0xFF) { _nkey_ = 98, _okey_ = 99;}  						// this differntiates btw a key gap , sends oxff when a key is nt pressed
		if(_nkey_ == _okey_) { _nkey_ = 0xFF;}
		
		if ((_nkey_!= 0xFF)&(_nkey_!=98))
		{		
		
		//USART_Transmit(keymap[_nkey_]);											//  a variable should hold this value									
		_okey_=_nkey_;
		return (_nkey_);
		d_nkey = _nkey_;
				
		}
		
		
}

ISR(USART0_RX_vect) 
{ 
   char data1;
   data1=UDR0;
   //USART_Transmit(data1);
}