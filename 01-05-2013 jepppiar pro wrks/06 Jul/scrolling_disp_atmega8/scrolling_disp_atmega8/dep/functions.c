

volatile uint8_t RXC_ISR_DATA[4], RXC_ISR_INDEX=0, END_BYTE=0x45, CHK_SUM=0;


static inline void USART_Init(unsigned int __baud) __attribute__((always_inline));
static inline void USART_Intr(void) __attribute__((always_inline));
static inline void NB_Tx(uint8_t __Addr,uint8_t __data) __attribute__((always_inline)); // for Tx from At32 to At8
void USART_Transmit( uint8_t __data );


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
NB_Tx(uint8_t __Addr,uint8_t __data)
{
   uint8_t __i,__CHK_SUM=((0X0F&__Addr)^(__data)),__Pkt[3]={__Addr,__data,__CHK_SUM} ;
   for(__i=0;__i<3;__i++)
    {   
	  while ( !( UCSRA & (1<<UDRE)) );
		/* Put data into buffer, sends the data */
		UDR = __Pkt[__i];
	}
}


void 
USART_Transmit( uint8_t __data )
{
		/* Wait for empty transmit buffer */
		while ( !( UCSRA & (1<<UDRE)) );
		
		/* Put data into buffer, sends the data */
		UDR = __data;
}