/*#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
	
DDRC = 0xFF;
DDRB = 0xFF;
PORTC = 0x55;
while(1)
{
   PORTC = 0x55;
   PORTB = 0x55;
   _delay_ms(100);
   PORTC ^= 0xFF;
   PORTB ^= 0xFF;
   _delay_ms(100);
}

return 0;
}
*/
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<inttypes.h>
#include<string.h>
#include "dep/global.c" //bug free code
#include "dep/main.h"
#include "dep/GLCD.c"
#include "dep/functions.c"
#include "dep/sportronix.h"

#define F_CPU 16000000UL

uint8_t keypad_4keys(void);
static int8_t indexA=0,indexB=0;
volatile unsigned int rx_char=0;
void sendNB(void);
void ControlPanel(void);


void GCDP_Tx(int state)
{
  GCSP=state;
  USART_Transmit(state);   
}
void lcdBallPoss(int aro_tog)
{
    if(aro_tog==1) 
	{
	  setcolumn(83);
      setpage(2);
	  lcddata(&font5x7[0],5);
	  setcolumn(41);
      setpage(2);
	  lcddata(&font5x7[470],5);
	}
	else 
	{
	  setcolumn(41);
      setpage(2);
	  lcddata(&font5x7[0],5);
	  setcolumn(83);
      setpage(2);
	  lcddata(&font5x7[475],5);
    }	
}

void renderDisp(void)
{
     //clrlcd();
	 lcdScore(26,3,AS);
	 lcdScore(90,3,BS);
	 lcdnumdata(61,7,QT);
	 lcdnumdata(8,7,AF);
	 lcdnumdata(113,7,BF);
	 lcdBallPoss(ARO);
}

void dispclear(void)
{
	 clrlcd();
	 lcdputs2(50,2,ar3);		//score 
	 lcdputs2(8,6,ar4);
	 lcdputs2(113,6,ar4);		//FOUL
	 lcdputs2(55,6,ar6);
	 lcdputN(0,0,teama);
	 lcdputN(64,0,teamb);
}

void t1_rst(void)
{
		TCNT1=0xBDB;
		o_sec=0;
}
	
int main(void)
{
	
	DDRA=0x0F;
	PORTA=0X0F;
	DDRC=0xFF;
	PORTC=0xFF;
	DDRB=0x00;
	DDRD=0xFF;
    GLCD_Init128();	
	USART_Init(103);
	TIMSK=(1<<TOIE1); // enabled global and timer overflow interrupt;
	TCNT1=0xBDB; // set initial value to remove time error (16bit counter register)
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
	//GCSP=1;
	Timer3EN();
	GCDP_Tx(1);

	picture(&sportronix[0]);
	sei();
	
	
	while(1)
	{
		_av=keypad_4keys();
		
		// get into this loop only when the home button is pressed, also clear the existing team names since there s no option for backspace		
		
		if(menu == 3)
		{
		  USART_RxIntEN();
          dispclear();
		  renderDisp();
		  menu++;
		}
		else if(menu == 1)
		{
			// for displaying teama name
			lcdputs2(16,1,ar7);	
			lcdputs2(16,7,ar9);	
            	
			setcolumn(40);
			setpage(2);
			 
			for(int k=0;((k<6));k++)
			{
				if(teama[k]!=0x01)
				{
					lcdsim_chardata((40+(k*8)),4,teama[k]);
				}
				else
				{
					lcd_bs(6);
				}
			}
		}		
		else if(menu == 2)  // for displaying teamb name
		{
			lcdputs2(16,1,ar8);	
			lcdputs2(16,7,ar9);	
			
			setcolumn(40);
			setpage(2);
         
			for(int k=0;((k<6));k++)	//&(teamb[k]!=0x01)
			{
				if(teamb[k]!=0x01)
				{
					lcdsim_chardata((40+(k*8)),4,teamb[k]);
				}
				else
				{
					lcd_bs(6);
				}
			}
		}
		// now analyze the team names and put them into a PROPER array format.
		if(_av==2) // home button
		{
			//_ch=0;
			if(menu==0)
			{
			   ControlPanel();
			}
			menu++;
			sendNB(); // Send the data to main board
			if(menu!=0)
			{
				clrlcd();
			}
			if(menu>3)
			{
				menu=1;			// exit out of the menu and capture the team names
				_ch=0;
				USART_RxIntDE();
				//Timer3DIS();
			}
			//USART_Transmit(2);
		}
		else if(_av == 0)                    // buzzer
		{
		    if(menu > 2)
			{
			   USART_Transmit(2); //2s
			}
			else if(menu == 1)
			{
				if(indexA>=0)
				{
					indexA--;
					if(indexA<0)
					{
						indexA=0;
					}
					teama[indexA]=0x01;
				}
			}
            else if(menu == 2)	
			{
				
				if(indexB>=0)
				{
					indexB--;
					if(indexB<0)
					{
						indexB=0;
					}
					teamb[indexB]=0x01;
				}
			}	
		}	
		else if((_av==1)&(menu == 1))   // A-Z
		{
			if(o_sec==1)
			{
				_ch=0;
			}
			 
			if(_ch<0)
			{
			    _ch=37;
			}	
            if(_ch>37)
			{
			    _ch=0;
			}
			 
			km = keymap[_ch];
			_ch++;
			if(o_sec==0)
			{
				if(indexA>0)
				{
					indexA--; 
					if(indexA<0)
					{
						indexA=0;
					}
				}
			}
			 
			teama[indexA]=(km);
			 
			indexA++;
			if(indexA>=6)
			{
			  indexA=6;
			}
			//USART_Transmit(1);
			
			t1_rst();
		}
		else if((_av == 1) & (menu == 2))   // A-Z
		{
			if(o_sec==1)
			{
				_ch=0;
			}
			 
			if(_ch<0)
			{
			    _ch=37;
			}	
            if(_ch>37)
			{
			    _ch=0;
			}
			 
			km = keymap[_ch];
			_ch++;
			if(o_sec==0)
			{
				if(indexB>0)
				{
					indexB--; 
					if(indexB<0)
					{
						indexB=0;
					}
				}
			}
			 
			teamb[indexB]=(km);	
           		
			indexB++;
			if(indexB>=6)
			{
			  indexB=6;
			}
			//USART_Transmit(1);
			
			t1_rst();
		}
		else if((_av == 3) & (menu == 1))		// Z-A
		{
			//USART_Transmit(3);
			if(o_sec==1)
			{
				_ch=0;
			}
			
			_ch--;
			if(_ch<0)
			{
			    _ch=37;
			}
			 
			if(_ch<0)
			{
			    _ch=37;
			}	
            if(_ch>37)
			{
			    _ch=0;
			}
			km = keymap[_ch];
			 
			if(o_sec==0)
			{
				if(indexA>0)
				{
					indexA--; 
					if(indexA<0)
					{
						indexA=0;
					}
				}
			}
			 
			teama[indexA]=(km);
			 
			indexA++;
			if(indexA>=6)
			{
			  indexA=6;
			}
			//USART_Transmit(1);
			t1_rst();
		}
		else if((_av == 3) & (menu == 2))		// Z-A
		{
			//USART_Transmit(3);
			if(o_sec==1)
			{
				_ch=0;
			}
			
			_ch--;
			if(_ch<0)
			{
			    _ch=37;
			}
			 
			if(_ch<0)
			{
			    _ch=37;
			}	
            if(_ch>37)
			{
			    _ch=0;
			}
			km = keymap[_ch];
			 
			if(o_sec==0)
			{
				if(indexB>0)
				{
					indexB--; 
					if(indexB<0)
					{
						indexB=0;
					}
				}
			}
			 
			teamb[indexB]=(km);
			 
			indexB++;
			if(indexB>=6)
			{
			  indexB=6;
			}
			//USART_Transmit(1);
			
			t1_rst();
		}
		
	}
}


ISR(TIMER1_OVF_vect) 
{
	
  o_sec=1;
  TCNT1=0xBDC; // set initial value to remove time error (16bit counter register)  
}

ISR(TIMER3_OVF_vect) 
{
	
  if(GCSP==0)
  {
    USART_Transmit(seconds);
    seconds--;
  }
  else if(seconds==0)
  {
    seconds=24;
	USART_Transmit(seconds);
  }
  else if(seconds==14)
  {
    USART_Transmit(seconds);
  }
  
  if(seconds==-1)
  {
    // GCSP=1;
	GCDP_Tx(1);
    seconds=0;
	USART_Transmit(seconds);
	_delay_us(5);
	USART_Transmit(2); //2s
	//waitTime=2;
	// ring the buzzer
    //transmit usat(TF_AD,seconds)
  }
  // set initial value to remove time error (16bit counter register)
  TCNT3=0xBDC;
}

	
void debounce(void)
{
_delay_ms(10);
}


uint8_t check_key_4keys(void)
{
	
	//PORTA =0b11111110;
	PORTC = 0X00;//&=~(1<<PC7);
	
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
		//PORTA=0x00;
	    PORTC = 0x00;  //&=~(1<<PC7);
		
		PORTB = 0xFF;  //set all the input to one
		
		_nkey_=check_key_4keys();		
		
		//if(_nkey_ == d_nkey) { USART_Transmit(9); }
		
		if(_nkey_ == 0xFF)   { _nkey_ = 98, _okey_ = 99;} // this differntiates btw a key gap , sends oxff when a key is nt pressed
		if(_nkey_ == _okey_) { _nkey_ = 0xFF;}
		
		if((_nkey_!= 0xFF)&(_nkey_!=98))
		{		
		//USART_Transmit(keymap[_nkey_]); //  a variable should hold this value									
		_okey_=_nkey_;
		return (_nkey_);
		//d_nkey = _nkey_;
		}
		return 0xFF;
}

ISR(USART0_RX_vect)  // @ CTrL side
{
rx_char=UDR0;


//PORTD^=0xff; For LEDs in protoboard (red)
	 switch(rx_char)
	  {
	    
	     case AS9:
		  AS--;	
			if(AS<0)
			{AS=0;}
		  USART_Transmit(AS);
		 
		 break;
		 
	     case ASR:
		  AS=0;
		  USART_Transmit(AS);
		  
		 break;	  
	     
	  	 case AS1:
		  if(AS<199)
		  {AS++;}
		  else
		  {AS=0;}
		  USART_Transmit(AS);
		  
		 break;
		 
	     case AS2:
		  if(AS<198)
		  {AS+=2;}
		  else
		  {AS=0;}
		  USART_Transmit(AS);
		  
         
		 break;	 
         
	     case AS3:
		  if(AS<197)
		  {AS+=3;}
		  else
		  {AS=0;}		
          USART_Transmit(AS);
		  
		  
		 break;
		 
		 case AF9:
		  AF--;
		  if(AF<0)
		  {AF=0;}
		  USART_Transmit(AF); 
		 break;
		 
	     case AFR:
		  AF=0;
		  USART_Transmit(AF);
		  
		 break;	  
	     
	  	 case AF1:
		  AF++;
		  if(AF>5)
		  {AF=5;}
		  USART_Transmit(AF);   
		 
		 break;
		 
	     case AT9:
		  AT--;
		  if(AT<0) {AT=0;}
		  USART_Transmit(AT);   
		 break;	 
          
	     case ATR:
		  AT=0;
		  USART_Transmit(AT);  
		 break;
		 
		 case AT1:
		  AT++;
		   if(AT>5) {AT=5;}
		  USART_Transmit(AT);   
		 break;
		 
	     case BS9:
		  BS--;
		  if(BS<0)
		  {BS=0;}
		  USART_Transmit(BS); 
		 
         
		 break;
		 
	     case BSR:
		  BS=0;
		  USART_Transmit(BS);
		  
		 break;
		 
	     case BS1:
		  if(BS<199)
		  {BS++;}
		  else
		  {BS=0;}
		  USART_Transmit(BS); 
	     
		 
		 break;
		 
	     case BS2:
		  if(BS<198)
		  {BS+=2;}
		  else
		  {BS=0;}
		  USART_Transmit(BS);
		 
		 break;
		 
	     case BS3:
		  if(BS<197)
		  {BS+=3;}
		  else
		  {BS=0;}		  
		  USART_Transmit(BS);
	     
		 break;
		 
	     case BF9:
		  BF--;
		  if(BF<0)
		  {BF=0;}
		  
		  USART_Transmit(BF); 
		 
		 break;
		 
	     case BFR:
		  BF=0;
		  USART_Transmit(BF);
			
		 break;
		 
	     case BF1:
		  BF++;
		  if(BF>5)
		  {BF=5;}
		  USART_Transmit(BF); 
			
		 break;
		 
	     case BT9:
		  BT--;
		  if(BT<0) {BT=0;}
		  USART_Transmit(BT);   
		 break;
		 
	     case BTR:
		  BT=0;
		  USART_Transmit(BT);   
		 break;
		 
	     case BT1:
		  BT++;
		  if(BT>5) {BT=5;}
		  USART_Transmit(BT);   
		 break;
		 
	     case QT9:
		  QT--;
		  if(QT<0) {QT=0;}
		  USART_Transmit(QT); 
 
		 break;
		 
	     case QTR:
		  QT=0;
		  USART_Transmit(QT);   
		
		 break;
		 
	     case QT1:
		  QT++;
		  if(QT>5) {QT=5;}
		  USART_Transmit(QT); 

		 break;
		 
	     case GC9: 
		  //minutes-1, seconds=0
		  USART_Transmit(gcm);   // gcm=9
		  seconds=24;
		  TCNT3=0xFFFE;// force timer overflow
		  //GCDP_Tx(1);
		  GCSP=1;		    
		 break;
		 
	     case GCR: 
		  //timer reset;
		  USART_Transmit(gcrst); // gcrst=0
		  seconds=24;
		  TCNT3=0xFFFE;// force timer overflow
		  //GCDP_Tx(1);
		   GCSP=1;	
 		 break;
		 
	     case GC1: 
		 // minutes+1, seconds=0
		  USART_Transmit(gcp); // gcp=1
		  seconds=24;
		  TCNT3=0xFFFE;// force timer overflow
		  //GCDP_Tx(1);
		  GCSP=1;			  
		 break;
		 
	     case GSP:  // actually it is arrow
		  
		  ARO^=1;
		  lcdBallPoss(ARO);
		  USART_Transmit(ARO);   
		 break;
		 
	     case R24: // actually it is gsp
		  //USART_Transmit(GCSP_AD,0); 
		  GCSP^=1;
		  GCDP_Tx(GCSP);
		  
		 break;
		 
	     case R13: // actually it is 24 sec reset
		  
		   USART_Transmit(TF_rp); 
           seconds=24;
		   TCNT3=0xFFFE;// force timer overflow
 		 break;
		 
	     case BPT:  // 14s reset
		   
		  USART_Transmit(TF_rp);
		  if(seconds<14)
		  {
           seconds=14;
		   TCNT3=0xFFFE;// force timer overflow
		   GCDP_Tx(1);		   
		  }
		 break;
	  }
	 renderDisp();
 }

void sendNB(void)
{  
   uint8_t i=0;
   if(menu == 2)
   {
     while(teama[i]!=0x01) 
	 {
       USART_Transmit(teama[i]);
	   i++;
	 } 
	 teama[9]=i; //strlen
     USART_Transmit(i);// total no of char
   }
   else if(menu == 3)
   {
     while(teamb[i]!=0x01) 
	 {
       USART_Transmit(teamb[i]);
	   i++;
	 }  
	 teamb[9]=i;// strlen
     USART_Transmit(i);// total no of char
   }


}

void ControlPanel(void)
{
  	 clrlcd();
	 lcdputs2(16,0,cp0);//title
	 lcdputs2(5,2,cp3);
	 lcdputs2(60,2,cp1);//on
	 setcolumn(90);
     setpage(2);
	 cpSelection=1;//def
	 cpStatus[0]=1;//def
	 lcddata(&font5x7[475],5);//arro >
	 while(1)
	 {
	 	_av=keypad_4keys();
		if(_av==2)
		{
		  break;
		}
		else if(_av==1)
		{
		  switch(cpSelection)
		  {
		  
		    case 1:
			 if(cpStatus[0]==1)
			 {
				 lcdputs2(60,2,cp2);//off
				 setcolumn(90);
				 setpage(2);
				 cpSelection=1;
				 lcddata(&font5x7[475],5);//arro >
				 cpStatus[0] = 0;
				 Timer3DIS();
			 }
			 else
			 {
				 lcdputs2(60,2,cp1);//on
				 setcolumn(90);
				 setpage(2);
				 cpSelection=1;
				 lcddata(&font5x7[475],5);//arro >	
				 cpStatus[0] = 1;
				 Timer3EN();
			 }
			 break;
		  }
		}
     
	 
	 }
}