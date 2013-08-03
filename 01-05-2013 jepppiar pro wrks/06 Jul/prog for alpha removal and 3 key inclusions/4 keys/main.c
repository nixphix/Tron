#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<inttypes.h>
#include<string.h>
#include "dep/global.c" //bug free code
#include "dep/main.h"
#include "dep/GLCD.c"
#include "dep/at128_USART.c"
#include "dep/sportronix.h"

#define F_CPU 16000000UL

uint8_t keypad_4keys(void);

static uint8_t indexA=0,indexB=0;
volatile unsigned int rx_char=0;
void sendNB(void);

void renderDisp(void)
{
     //clrlcd();
	 
	 lcdnumdata(25,3,AS);
	 lcdnumdata(85,3,BS);
	 lcdnumdata(55,7,QT);
	 lcdnumdata(5,7,AF);
	 lcdnumdata(110,7,BF);
	 
	 lcdputs2(46,2,ar3);		//score 
	 lcdputs2(5,6,ar4);
	 lcdputs2(110,6,ar4);		//FOUL
	 lcdputs2(58,6,ar6);
	 

}

void t1_rst(void)
{
		TCNT1=0xBDB;
		o_sec=0;
}
	
int main(void)
{

	USART_Init(103);
	TIMSK=(1<<TOIE1); // enabled global and timer overflow interrupt;
	TCNT1=0xBDB; // set initial value to remove time error (16bit counter register)
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10); // start timer/ set clock
	DDRA=0x0F;
	PORTA=0X0F;
	DDRC=0xFF;
	PORTC=0xFF;
	DDRB=0x00;
	DDRD=0xFF;
	
	GLCD_Init128();	
	picture(&sportronix[0]);
	sei();
	
	
	while(1)
	{
		_av=keypad_4keys();
		
		// get into this loop only when the home button is pressed, also clear the existing team names since there s no option for backspace		
		
		if(menu == 3)
		{
		 USART_RxIntEN();
		 clrlcd();
		 renderDisp();
	/*	 lcdputs2(46,2,ar3);		//score 
	     lcdputs2(5,6,ar4);
	     lcdputs2(110,6,ar4);		//FOUL
	     lcdputs2(58,6,ar6);
	     
	     lcdnumdata(25,3,AS);
	     lcdnumdata(85,3,BS);
	     lcdnumdata(55,7,QT);
	     lcdnumdata(5,7,AF);
	     lcdnumdata(110,7,BF);*/ // moved to renderImg()
		  menu++;
		}
		else if(menu == 1)
		{
			// for displaying teama name
			lcdputs2(16,3,ar7);		
			setcolumn(40);
			setpage(4);
			 
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
			lcdputs2(16,3,ar8);	
			setcolumn(40);
			setpage(4);
         
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
			}
			//USART_Transmit(2);
		}
		else if(_av == 0) // buzzer USART_Transmit(0);
		{
		    if(menu == 3)
			{
			   USART_Tx128(BUZ_AD,1);
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
	    PORTC = 0x00;//&=~(1<<PC7);
		
		PORTB=0xFF;		//set all the input to one
		
		_nkey_=check_key_4keys();
		
		
		
		if(_nkey_ == d_nkey) { USART_Transmit(9); }
		
		if (_nkey_ == 0xFF) { _nkey_ = 98, _okey_ = 99;} // this differntiates btw a key gap , sends oxff when a key is nt pressed
		if(_nkey_ == _okey_) { _nkey_ = 0xFF;}
		
		if ((_nkey_!= 0xFF)&(_nkey_!=98))
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
		  USART_Tx128(AS_AD,AS);
		 
		 break;
		 
	     case ASR:
		  AS=0;
		  USART_Tx128(AS_AD,AS);
		  
		 break;	  
	     
	  	 case AS1:
		  if(AS<199)
		  {AS++;}
		  else
		  {
		    AS=0;
			clrlcd();
		  }
		  USART_Tx128(AS_AD,AS);
		  
		 break;
		 
	     case AS2:
		  if(AS<198)
		  {AS+=2;}
		  else
		  {AS=0;}
		  USART_Tx128(AS_AD,AS);
		  
         
		 break;	 
         
	     case AS3:
		  if(AS<197)
		  {AS+=3;}
		  else
		  {AS=0;}		
          USART_Tx128(AS_AD,AS);
		  
		  
		 break;
		 
		 case AF9:
		  AF--;
		  if(AF<0)
		  {AF=0;}
		  USART_Tx128(AF_AD,AF); 
		 break;
		 
	     case AFR:
		  AF=0;
		  USART_Tx128(AF_AD,AF);
		  
		 break;	  
	     
	  	 case AF1:
		  AF++;
		  if(AF>5)
		  {AF=0;}
		  USART_Tx128(AF_AD,AF);   
		 
		 break;
		 
	     case AT9:
		  AT--;
		  if(AT<0) {AT=0;}
		  USART_Tx128(AT_AD,AT);   
		 break;	 
          
	     case ATR:
		  AT=0;
		  USART_Tx128(AT_AD,AT);  
		 break;
		 
		 case AT1:
		  AT++;
		   if(AT>5) {AT=0;}
		  USART_Tx128(AT_AD,AT);   
		 break;
		 
	     case BS9:
		  BS--;
		  if(BS<0)
		  {BS=0;}
		  USART_Tx128(BS_AD,BS); 
		 
         
		 break;
		 
	     case BSR:
		  BS=0;
		  USART_Tx128(BS_AD,BS);
		  
		 break;
		 
	     case BS1:
		  if(BS<199)
		  {BS++;}
		  else
		  {
		   BS=0;
		   clrlcd();
		  }
		  USART_Tx128(BS_AD,BS); 
	     
		 
		 break;
		 
	     case BS2:
		  if(BS<198)
		  {BS+=2;}
		  else
		  {BS=0;}
		  USART_Tx128(BS_AD,BS);
		 
		 break;
		 
	     case BS3:
		  if(BS<197)
		  {BS+=3;}
		  else
		  {BS=0;}		  
		  USART_Tx128(BS_AD,BS);
	     
		 break;
		 
	     case BF9:
		  BF--;
		  if(BF<0)
		  {BF=0;}
		  
		  USART_Tx128(BF_AD,BF); 
		 
		 break;
		 
	     case BFR:
		  BF=0;
		  USART_Tx128(BF_AD,BF);
			
		 break;
		 
	     case BF1:
		  BF++;
		  if(BF>5)
		  {BF=0;}
		  USART_Tx128(BF_AD,BF); 
			
		 break;
		 
	     case BT9:
		  BT--;
		  if(BT<0) {BT=0;}
		  USART_Tx128(BT_AD,BT);   
		 break;
		 
	     case BTR:
		  BT=0;
		  USART_Tx128(BT_AD,BT);   
		 break;
		 
	     case BT1:
		  BT++;
		  if(BT>5) {BT=0;}
		  USART_Tx128(BT_AD,BT);   
		 break;
		 
	     case QT9:
		  QT--;
		  if(QT<0) {QT=0;}
		  USART_Tx128(QT_AD,QT); 
 
		 break;
		 
	     case QTR:
		  QT=0;
		  USART_Tx128(QT_AD,QT);   
		
		 break;
		 
	     case QT1:
		  QT++;
		  if(QT>5) {QT=0;}
		  USART_Tx128(QT_AD,QT); 

		 break;
		 
	     case GC9: 
		  //minutes-1, seconds=0
		  USART_Tx128(GC_AD,gcm);   // gcm=9
		    
		 break;
		 
	     case GCR: 
		  //timer reset;
		  USART_Tx128(GC_AD,gcrst); // gcrst=0
			   
		 break;
		 
	     case GC1: 
		 // minutes+1, seconds=0
		  USART_Tx128(GC_AD,gcp); // gcp=1
			  
		 break;
		 
	     case GSP:  // actually it is arrow
		  
		  ARO^=1;
		  USART_Tx128(ARO_AD,ARO);   
		 break;
		 
	     case R24: // actually it is gsp
		  
		  USART_Tx128(GCSP_AD,0); 
		  
		 break;
		 
	     case R13: // actually it is 24 sec reset
		  
		   USART_Tx128(TFR_AD,TF_rp);    
		 break;
		 
	     case BPT:  // 
		   
		  USART_Tx128(THR_AD,TF_rp);
		 break;
	  }
	 renderDisp();
	 /*clrlcd();
	 lcdnumdata(25,3,AS);
	 lcdnumdata(85,3,BS);
	 lcdnumdata(55,7,QT);
	 lcdnumdata(5,7,AF);
	 lcdnumdata(110,7,BF);*/ // moved to renderImg()
 }

void sendNB(void)
{  
   uint8_t i=0;
   if(menu == 2)
   {
     while(teama[i]!=0x01) 
	 {
       USART_Tx128(i+100,teama[i]);
	   i++;
	 } 
	 teama[9]=i; //strlen
     USART_Tx128(106,i);// total no of char
   }
   else if(menu == 3)
   {
     while(teamb[i]!=0x01) 
	 {
       USART_Tx128(i+150,teamb[i]);
	   i++;
	 }  
	 teamb[9]=i;// strlen
     USART_Tx128(156,i);// total no of char
   }


}