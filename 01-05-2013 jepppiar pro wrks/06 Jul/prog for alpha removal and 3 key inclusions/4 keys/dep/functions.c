#define START_BYTE 0x53

void Timer3EN(void)
{
    ETIMSK = (1<<TOIE3);
	TCNT3  = 0xBDB;
	TCCR3B = (1<<CS32)|(0<<CS31)|(0<<CS30);
}

void Timer3DIS(void)
{
    ETIMSK & = ~(1<<TOIE3);
	TCCR3B & = ~((1<<CS32)|(1<<CS31)|(1<<CS30));
}

void USART_Init( unsigned int baud )
{
		/* Set baud rate */
		UBRR0H = (unsigned char)(baud>>8);
		UBRR0L = (unsigned char)baud;
		/* Enable receiver and transmitter */
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);
		/* Set frame format: 8data, 2stop bit */
		UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(3<<UCSZ00);
		
		
}

void USART_RxIntEN(void)
{
        UCSR0B|=(1 << RXCIE0 );
}

void USART_RxIntDE(void)
{
        UCSR0B&=~(1 << RXCIE0 );
}

void USART_Transmit( unsigned int data )
{
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) );
		
		/* Put data into buffer, sends the data */
		UDR0 = data;
}

void  USART_Tx128(uint8_t __Addr,uint8_t __data)
 {
   
   uint8_t __i,__CHK_SUM=((0X0F&__Addr)^(__data)),__Pkt[4]={START_BYTE,__Addr,__data,__CHK_SUM} ;
   for(__i=0;__i<4;__i++)
    {   
	  while ( !( UCSR0A & (1<<UDRE0)) );
		/* Put data into buffer, sends the data */
		UDR0 = __Pkt[__i];
	}
 }

unsigned char USART_Receive( void )
{

		/* Wait for data to be received */
		while ( !(UCSR0A & (1<<RXC0)) );
		
		/* Get and return received data from buffer */
		
		return UDR0;
}