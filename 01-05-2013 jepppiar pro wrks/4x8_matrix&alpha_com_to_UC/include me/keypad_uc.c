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
			else if(1==1)
			return 0xFF;
}		

 
uint8_t check1_32keys(void)
{
	
	pad =0b11111110;
	debounce();
	
	if(bit_is_clear(PINB,c1))
	return(AS9);
	else if(bit_is_clear(PINB,c2))
	return(ASR);
	else if(bit_is_clear(PINB,c3))
	return(AS1);
	else if(bit_is_clear(PINB,c4))
	return(AS2);
	else if(bit_is_clear(PINB,c5))
	return(AS3);
	else if(bit_is_clear(PINB,c6))
	return(AF1);
	else if(bit_is_clear(PINB,c7))
	return(AFR);
	else if(bit_is_clear(PINB,c8))
	return(AF9);
	else if(1==1)
	return 0xFF;
	
}
 
 
uint8_t check2_32keys(void)
{
	pad=0b11111101;
	debounce();
	
	if(bit_is_clear(PINB,c1))
	return(AT9);
	else if(bit_is_clear(PINB,c2))
	return(ATR);
	else if(bit_is_clear(PINB,c3))
	return(AT1);
	else if(bit_is_clear(PINB,c4))
	return(QT9);
	else if(bit_is_clear(PINB,c5))
	return(QTR);
	else if(bit_is_clear(PINB,c6))
	return(QT1);
	else if(bit_is_clear(PINB,c7))
	return(R24);
	else if(bit_is_clear(PINB,c8))
	return(R13);
	else if(1==1)
	return 0xFF;
	
}
 
uint8_t check3_32keys(void)
{
	pad=0b11111011;
	debounce();

	
	if(bit_is_clear(PINB,c1))
	return(BS9);
	else if(bit_is_clear(PINB,c2))
	return(BSR);
	else if(bit_is_clear(PINB,c3))
	return(BS1);
	else if(bit_is_clear(PINB,c4))
	return(BS2);
	else if(bit_is_clear(PINB,c5))
	return(BS3);
	else if(bit_is_clear(PINB,c6))
	return(BF1);
	else if(bit_is_clear(PINB,c7))
	return(BFR);
	else if(bit_is_clear(PINB,c8))
	return(BF9);
	else if(1==1)
	return 0xFF;
	
}
 
uint8_t check4_32keys(void)
{
	pad =0b11110111;
	debounce();
		
		
	if(bit_is_clear(PINB,c1))
	return(BT9);
	else if(bit_is_clear(PINB,c2))
	return(BTR);
	else if(bit_is_clear(PINB,c3))
	return(BT1);
	else if(bit_is_clear(PINB,c4))
	return(GC9);
	else if(bit_is_clear(PINB,c5))
	return(GCR);
	else if(bit_is_clear(PINB,c6))
	return(GC1);
	else if(bit_is_clear(PINB,c7))
	return(GSP);
	else if(bit_is_clear(PINB,c8))
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




uint8_t check_key_alpha(void)
{
uint8_t _a,_b,_c,_d;
			_a=check1_alpha();
			
			_b=check2_alpha();
			
			_c=check3_alpha();
			
			_d=check4_alpha();
			
			if(_a!=0xFF)
			return _a;
			else if(_b!=0xFF)
			return _b;
			else if(_c!=0xFF)
			return _c;
			else if(_d!=0xFF)
			return _d;
			else if(1==1)
			return 0xFF;
}		

 
uint8_t check1_alpha(void)
{
	
	pad =0b11111110;
	
	debounce();
	if(bit_is_clear(PINA,c1_alpha))
	return(1);
	else if(bit_is_clear(PINA,c2_alpha))
	return(2);
	else if(bit_is_clear(PINA,c3_alpha))
	return(3);
	else if(bit_is_clear(PINA,c4_alpha))
	return('A');
	
	return 0xFF;
}
 
 
uint8_t check2_alpha(void)
{
	pad=0b11111101;
	
	debounce();
	if(bit_is_clear(PINA,c1_alpha))
	return(4);
	else if(bit_is_clear(PINA,c2_alpha))
	return(5);
	else if(bit_is_clear(PINA,c3_alpha))
	return(6);
	else if(bit_is_clear(PINA,c4_alpha))
	return('B');
	
		return 0xFF;
	
}
 
uint8_t check3_alpha(void)
{
	pad=0b11111011;
	
	debounce();
	if(bit_is_clear(PINA,c1_alpha))
	return(7);
	else if(bit_is_clear(PINA,c2_alpha))
	return(8);
	else if(bit_is_clear(PINA,c3_alpha))
	return(9);
	else if(bit_is_clear(PINA,c4_alpha))
	return('C');
	
		return 0xFF;
}
 
uint8_t check4_alpha(void)
{
	pad =0b11110111;
	
	debounce();
	if(bit_is_clear(PINA,c1_alpha))
	return('#');
	else if(bit_is_clear(PINA,c2_alpha))
	return(0);
	else if(bit_is_clear(PINA,c3_alpha))
	return('*');
	else if(bit_is_clear(PINA,c4_alpha))
	return('D');
	
		return 0xFF;
}

void keypad_alpha(void)
{
PORTA=0xF0;																//set all the input to one
	nkey=check_key_alpha();
		if(nkey!=0xFF){d_nkey=nkey;}			
		if(d_nkey != d_okey) { b=1;}										// when a diff key is pressed the backspace shld not be printed
		
		if (nkey == 0xFF) { nkey = 98, okey = 99;}  						// this differntiates btw a key gap 
		if(nkey == okey) { nkey = 0xFF; }
			
		if ((nkey!= 0xFF)&(nkey!=98)&(nkey!='C'))
		{		
				if(nkey=='D')
							{
							USART_Transmit(0x06);
							}
							
							
				if(nkey=='A'){
							//USART_Transmit(0x08);
							//{i--;teama[i]=0;}									this part was replaced to utolise the key to say "OK"
							USART_Transmit(0x07);								
							}
				
				
				if((nkey!='D')&(nkey!='A'))
				{
				
			
				data=((nkey)-1); 								   			// data calculations
				if(odata==data){++e;if(e>2){e=0;}}else { e=0;}  			// e calculations
			
				
				if(b==0)													//  when it amounts a second/new key, ths ll not execute 
				{USART_Transmit(0x08);										//  leaving behind the desired alphabet
				if(i<6)
				i--;														// this will store the desired alphabet in the array
				}
				
				
				{if(i<6)
				{
				teama[i]=(keymap[data][e]);
				i++;
				}}
				
				t1_rst(); 													// reset timer
				
				
				USART_Transmit((keymap[data][e]));
				
				}			
				
				okey=nkey;
				d_okey=okey;
				odata=data;
				
				
		}
		if(nkey=='C')
		{
			USART_Transmit(0x05);
			_ext=1;
		}
		
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