void _SYSTEM_INIT(void)
{	
	USART_Init(103);
	TIMSK=(1<<TOIE1); 														// enabled global and timer overflow interrupt;
	TCNT1=0xBDB; 															// set initial value to remove time error (16bit counter register)
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); 								// start timer/ set clock
	sei();
	DDRA=0x0F;
	PORTA=0X0F;
	DDRB=0x00;
}
	
	
	
void debounce(void)
{
    _delay_ms(10);
}

	
uint8_t check_key_32keys(void)
{
    uint8_t _a_,_b_,_c_,_d_;
			_a_=check1_32keys();
			
			_b_=check2_32keys();
			
			_c_=check3_32keys();
			
			_d_=check4_32keys();
			
			if(_a_!=0xFF)
			return _a_;
			else if(_b_!=0xFF)
			return _b_;
			else if(_c_!=0xFF)
			return _c_;
			else if(_d_!=0xFF)
			return _d_;
			else 
			return 0xFF;
}		

 
uint8_t check1_32keys(void)
{
	
	//pad =0b11111110;
	PULLUP_ROW(ROW1);
	debounce();
	
	if(bit_is_clear(PINB,COLUMN1))
	return(AS9);
	else if(bit_is_clear(PINB,COLUMN2))
	return(ASR);
	else if(bit_is_clear(PINB,COLUMN3))
	return(AS1);
	else if(bit_is_clear(PINB,COLUMN4))
	return(AS2);
	else if(bit_is_clear(PINB,COLUMN5))
	return(AS3);
	else if(bit_is_clear(PINB,COLUMN6))
	return(AF1);
	else if(bit_is_clear(PINB,COLUMN7))
	return(AFR);
	else if(bit_is_clear(PINB,COLUMN8))
	return(AF9);
	else if(1==1)
	return 0xFF;
	
}
 
 
uint8_t check2_32keys(void)
{
	//pad=0b11111101;
    PULLUP_ROW(ROW2);
	debounce();
	
	if(bit_is_clear(PINB,COLUMN1))
	return(AT9);
	else if(bit_is_clear(PINB,COLUMN2))
	return(ATR);
	else if(bit_is_clear(PINB,COLUMN3))
	return(AT1);
	else if(bit_-is_clear(PINB,COLUMN4))
	return(QT9);
	else if(bit_is_clear(PINB,COLUMN5))
	return(QTR);
	else if(bit_is_clear(PINB,COLUMN6))
	return(QT1);
	else if(bit_is_clear(PINB,COLUMN7))
	return(R24);
	else if(bit_is_clear(PINB,COLUMN8))
	return(R13);
	else if(1==1)
	return 0xFF;
	
}
 
uint8_t check3_32keys(void)
{
	//pad=0b11111011;
	PULLUP_ROW(ROW3);
	debounce();

	
	if(bit_is_clear(PINB,COLUMN1))
	return(BS9);
	else if(bit_is_clear(PINB,COLUMN2))
	return(BSR);
	else if(bit_is_clear(PINB,COLUMN3))
	return(BS1);
	else if(bit_is_clear(PINB,COLUMN4))
	return(BS2);
	else if(bit_is_clear(PINB,COLUMN5))
	return(BS3);
	else if(bit_is_clear(PINB,COLUMN6))
	return(BF1);
	else if(bit_is_clear(PINB,COLUMN7))
	return(BFR);
	else if(bit_is_clear(PINB,COLUMN8))
	return(BF9);
	else if(1==1)
	return 0xFF;
	
}
 
uint8_t check4_32keys(void)
{
	//pad =0b11110111;
	PULLUP_ROW(ROW4);
	debounce();
		
		
	if(bit_is_clear(PINB,COLUMN1))
	return(BT9);
	else if(bit_is_clear(PINB,COLUMN2))
	return(BTR);
	else if(bit_is_clear(PINB,COLUMN3))
	return(BT1);
	else if(bit_is_clear(PINB,COLUMN4))
	return(GC9);
	else if(bit_is_clear(PINB,COLUMN5))
	return(GCR);
	else if(bit_is_clear(PINB,COLUMN6))
	return(GC1);
	else if(bit_is_clear(PINB,COLUMN7))
	return(GSP);
	else if(bit_is_clear(PINB,COLUMN8))
	return(BPT);
	else if(1==1)
	return 0xFF;
}


		
void USART_Init( unsigned int __baud )
{
		UBRRH = (uint8_t)(__baud>>8);
        UBRRL = (uint8_t)__baud;
		UCSRB = (1<<RXEN)|(1<<TXEN);
		UCSRC = (1<<RXCIE)|(1<<UCSZ1)|(3<<UCSZ0)|(1<<URSEL);
}

void USART_Transmit( unsigned int __data )
{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR = __data;
}

uint8_t USART_Receive(void)
{
		while (!(UCSRA & (1<<RXC)));
		return UDR;
}





	
ISR(TIMER1_OVF_vect) 
{
  TCNT1=0xBDB; 																// set initial value to remove time error (16bit counter register)
  odata=100;
  b=1;
}

void t1_rst(void)
{
TCNT1=0xBDB;
b=0;
}



void keypad_32keys(void)
{
		PORTA=0x00;
		PORTB=0xFF;		//set all the input to one
		
		_nkey_=check_key_32keys();
					
		if (_nkey_ == 0xFF) { _nkey_ = 98, _okey_ = 99;}  						// this differntiates btw a key gap 
		if(_nkey_ == _okey_) { _nkey_ = 0xFF; }
		
		if ((_nkey_!= 0xFF)&(_nkey_!=98))
		{		USART_Transmit(_nkey_);										
				_okey_=_nkey_;
		}		
}