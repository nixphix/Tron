#define START_BYTE 0x53
#define dis1_PORT PORTA   //timer
#define dis2_PORT PORTB
 void SYS_INIT(void);
 void Buzz(uint8_t B);
 void Timer1Init(void);
 void Timer0EN(void);
 void Timer0DIS(void);
 void Timer1EN(void);
 void Timer1DIS(void);
  

 void SYS_INIT(void)
  {
		//MCUCSR |= (1<<JTD);	//This statement will enable PORTC and it should be written twice
		//MCUCSR |= (1<<JTD);
		DDRB = 0xFF;	// all ports declared o/p
		DDRC = 0xFF;
		DDRD = 0xF0;
		DDRA = 0xFF;
   }   
   
  void Buzz (uint8_t B)
  {
     if(B==1)
	 {
	   PORTD |= _BV(PD6);
	   Timer0EN();
	 }
     else
	 {
	   PORTD &= ~(_BV(PD6));
	   Timer0DIS();
	 }
  }
  
  void Timer1Init(void)
  {
    TIMSK|=(1<<TOIE1); // enabled global and timer overflow interrupt;
	TCNT1=0xBDB; // set initial value to remove time error (16bit counter register)
	//TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
  }
  
  void Timer1EN(void) /// Set init val to t1 and enable timer ovf interrupt
  {
  	 TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
	 TCNT1 = 0xBDC; // set initial value to remove time error (16bit counter register)
  }
  
  void Timer1DIS(void) /// disables timer ovf interrupt
  {
	 TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10)); // no clock source
  }
  
  void Timer0EN(void)
  {
     // Prescaler = FCPU/1024
     TCCR0 |= (1<<CS02)|(1<<CS00);
     //Enable Overflow Interrupt Enable
     TIMSK |= (1<<TOIE0);
     //Initialize Counter
     TCNT0=0;
  }
  
  void Timer0DIS(void)
  {
     // Prescaler = FCPU/1024
     TCCR0 &= ~((1<<CS02)|(1<<CS00));
  }
  

  
  void scoAdigit(uint8_t one,uint8_t arr)	
  {
		 dis2_PORT = 0x00;//portb
        if(one==1)	
		 {
		   dis2_PORT |= (1<<PB1)|(1<<PB2);
		 }
		 else if(one==0)	
		 {
		   dis2_PORT &= ~((1<<PB1)|(1<<PB2));
		 }
       if(arr==1)	
		 {
		   dis2_PORT |= (1<<PB4); // PB3 ,4
		  // dis2_PORT |= 0b00011000; // PB3 ,4
		 }
      /* if(Buz==1)	
		 {
		   dis2_PORT |= 0b00000100; // PB2
		 }	*/	 
  }
  
  void scoBdigit(uint8_t one,uint8_t arr)	
  {
		/*if(d==1&&e==0)			{dis2_PORT = 0b01111000;}
		else if(d==1&e==1)		{dis2_PORT = 0b01100000;}
		else if(d==0e==0)		{dis2_PORT = 0b00011000;}
		else if(d==0,e==1)		{dis2_PORT = 0b00000000;}*/
		dis2_PORT = 0x00;
	   if(one==1)	
		 {
		   dis2_PORT = (1<<PB1)|(1<<PB2);
		 }
		else if(one==0)	
		 {
		   dis2_PORT &= ~((1<<PB1)|(1<<PB2));
		 }
       if(arr==0)	
		 {
		   dis2_PORT |= (1<<PB4); // PB3 ,4
		 }	
		
  } 
 
void display1(uint8_t d)		//Define a function to display the number passed on the seven segment display
{
	switch(d)			//Open a switch case
	{
		case 1: 
		
		dis1_PORT = 0b00110000;
		
		break;			
	
		case 2: 
		
		dis1_PORT = 0b01101101;
			
		break;
		
		case 3: 
		
		dis1_PORT = 0b01111001;
		
		break;
		
		case 4: 
		
		dis1_PORT = 0b00110011;
		
		break;
		
		case 5: 
		
		dis1_PORT = 0b01011011;
		
		break;
		
		case 6: 
		
		dis1_PORT = 0b01011111;
		
		break;
		
		case 7: 
		
		dis1_PORT = 0b01110000;
		
		break;
		
		case 8: 
		
		dis1_PORT = 0b01111111;
		
		break;
		
		case 9: 
		dis1_PORT = 0b01110011;
		
		break;
		
		case 0: 
		
		dis1_PORT = 0b01111110;
		
	}

}

void display2(uint8_t e)		//Define a function to display the number passed on the seven segment display
{
	switch(e)			//Open a switch case
	{
		case 1: 
		
		dis2_PORT = 0b00110000;
		
		break;			
	
		case 2: 
		
		dis2_PORT = 0b01101101;
			
		break;
		
		case 3: 
		
		dis2_PORT = 0b01111001;
		
		break;
		
		case 4: 
		
		dis2_PORT = 0b00110011;
		
		break;
		
		case 5: 
		
		dis2_PORT = 0b01011011;
		
		break;
		
		case 6: 
		
		dis2_PORT = 0b01011111;
		
		break;
		
		case 7: 
		
		dis2_PORT = 0b01110000;
		
		break;
		
		case 8: 
		
		dis2_PORT = 0b01111111;
		
		break;
		
		case 9: 
		dis2_PORT = 0b01110011;
		
		break;
		
		case 0: 
		
		dis2_PORT = 0b01111110;
		
	}

}


