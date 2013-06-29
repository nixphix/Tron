// global

volatile uint16_t NBA0_AD=300,NBA1_AD=301,NBA2_AD=302,NBA3_AD=303,NBA4_AD=304,NBA5_AD=305,NBA6_AD=306,NBA7_AD=307, 
                 NBB0_AD=400,NBB1_AD=401,NBB2_AD=402,NBB3_AD=403,NBB4_AD=404,NBB5_AD=405,NBB6_AD=406,NBB7_AD=407,
				 NAME_AD=500;
	
volatile uint8_t NBA[8] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},NBB[8] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}, NAME_CMD=0x0;
volatile int CHARS = 8;

//

void USART_Init( unsigned int __baud )
{
		/* Set baud rate */
		UBRRH = (uint8_t)(__baud>>8);
        UBRRL = (uint8_t)__baud;
		/* Enable receiver and transmitter */
		UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
		/* Set frame format: 8data, 2stop bit */
		UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( uint8_t __data )
{
		/* Wait for empty transmit buffer */
		while ( !( UCSRA & (1<<UDRE)) );
		
		/* Put data into buffer, sends the data */
		UDR = __data;
}

uint8_t USART_Receive(void)
{
		/* Wait for data to be received */
		while (!(UCSRA & (1<<RXC)));
		
		/* Get and return received data from buffer */
		return UDR;
}


