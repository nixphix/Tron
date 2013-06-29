#define START_BYTE 0x53

 void SYS_INIT(void)
  {
		DDRB = 0xFF;	// all ports declared o/p
		DDRC = 0xFF;
   }   
   


void DataLine(uint8_t d)		//data line for 7 seg led 
{
	switch(d)			//Open a switch case
	{
		case 1: 
		
		PORTC = 0b00110000 >> 1;
		PORTB &= ~_BV(5); // BIT 5 ZERO
		
		break;			
	 
		case 2: 
		
		PORTC = 0b01101101 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE
	 
		break;
		
		case 3: 
		
		PORTC = 0b01111001 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE
		
		break;
		
		case 4: 
		
		PORTC = 0b00110011 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE
		break;
		
		case 5: 
		
		PORTC = 0b01011011 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE
		break;
		
		case 6: 
		
		PORTC = 0b01011111 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE		
		break;
		
		case 7: 
		
		PORTC = 0b01110000 >> 1;
		PORTB &= ~_BV(5);  // BIT 5 ZERO
		break;
		
		case 8: 
		
		PORTC = 0b01111111 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE
		break;
		
		case 9: 
		PORTC = 0b01110011 >> 1;
		PORTB |= _BV(5);  // BIT 5 ONE
		break;
		
		case 0: 
		
		PORTC = 0b01111110 >> 1;
		PORTB &= ~_BV(5);  // BIT 5 ZERO
		
	}

}

void SelectLine(uint8_t e)		//Selects the digit 
{
	switch(e)			//PB1 drives high side of CN2  & PB2 drives high side of CN1
	{                   // CN1 will be tens digit and CN2 will be ones digit
		case 0: //digit change pause
		
		PORTB &= ~(_BV(1)|_BV(2));
		
		break;
		
		case 1: // ones
		
		PORTB |=_BV(1) ;
		
		break;			
	 
		case 2: // tens
		
		PORTB |=_BV(2);
	 
		break;
		
		case 3: // buzz
		
		PORTB |=_BV(4) ; // PB4 is connected to uln2803 8th pin // Buzzer connection:  (CN2 8th pin) -> (-)Buzzer(+) -> (12Vcc)
		
		break;
		
		case 4: // Reset buzzer
		
		PORTB &= ~_BV(4);
		
	}

}


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