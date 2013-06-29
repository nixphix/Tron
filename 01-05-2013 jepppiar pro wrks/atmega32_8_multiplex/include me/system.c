#define START_BYTE 0x53
#define dis1_PORT PORTA   //timer
#define dis2_PORT PORTB
 void SYS_INIT(void)
  {
		//MCUCSR |= (1<<JTD);	//This statement will enable PORTC and it should be written twice
		//MCUCSR |= (1<<JTD);
		DDRB = 0xFF;	// all ports declared o/p
		DDRC = 0xFF;
		DDRD = 0xF0;
		DDRA = 0xFF;
   }   
   
   void scoAdigit(uint8_t one,uint8_t arr,uint8_t Buz)	
   {
		/*if(d==1&&e==1&&f==0)			{dis2_PORT = 0b01111000;}
		else if(d==1,e==0&&f==0)		{dis2_PORT = 0b01100000;}
		else if(d==0,e==1&&f==0)		{dis2_PORT = 0b00011000;}
		else if(d==0,e==0&&f==0)		{dis2_PORT = 0b00000000;}
		else if(d==0,e==0&&f==1)		{dis2_PORT = 0b00000010;}
		else if(d==1,e==0&&f==1)		{dis2_PORT = 0b11000010;}
		else if(d==0,e==1&&f==1)		{dis2_PORT = 0b00011010;}
		else if(d==1,e==1&&f==1)		{dis2_PORT = 0b01111010;}*/
		 dis2_PORT = 0x00;
        if(one==1)	
		 {
		   dis2_PORT = (1<<PB5)|(1<<PB6);
		 }
		 else if(one==0)	
		 {
		   dis2_PORT = (0<<PB5)|(0<<PB6);
		 }
		 
		
       if(arr==1)	
		 {
		   dis2_PORT |= 0b00011000;
		 }
       if(Buz==1)	
		 {
		   dis2_PORT |= 0b00000100;
		 }		 
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
		   dis2_PORT = (1<<PB5)|(1<<PB6);
		 }
		else if(one==0)	
		 {
		   dis2_PORT = (0<<PB5)|(0<<PB6);
		 }
       if(arr==0)	
		 {
		   dis2_PORT |= 0b00011000;
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


