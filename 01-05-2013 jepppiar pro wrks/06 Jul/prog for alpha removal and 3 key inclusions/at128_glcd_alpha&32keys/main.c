#include<avr/io.h>
#define FCPU 160000000UL
#include<util/delay.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include<main.h>
#include<at128_USART.c>
#include<GLCD.c>
#include"global.c"
#include<sportronix.h>

volatile unsigned int rx_char=0;//,ext=0,i=0,j=0,alpha=0;

int main(void)

{ 
	USART_Init(103);
	DDRD=0xff;
	DDRE=0xff;
	
	
	GLCD_Init128();	
	picture(&sportronix[0]);
	_delay_ms(2500);
	clrlcd();
	sei();
	
	
	lcdputs2(46,2,ar3);		//score
	
	lcdputs2(5,6,ar4);
	lcdputs2(110,6,ar4);		//FOUL
	lcdputs2(58,6,ar6);
	
	lcdnumdata(25,3,AS);
	 lcdnumdata(85,3,BS);
	 lcdnumdata(55,7,QT);
	 lcdnumdata(5,7,AF);
	 lcdnumdata(110,7,BF);

	while(1)
	{
	}
}





ISR(USART0_RX_vect)  // @ CTrL side
{
rx_char=UDR0;


PORTD^=0xff;
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
		  AS++;
		  USART_Tx128(AS_AD,AS);
	
		  
		 break;
		 
	     case AS2:
		  AS+=2;
		  USART_Tx128(AS_AD,AS);
		  
  
		 break;	 
         
	     case AS3:
		  AS+=3;
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
		  if(BS<0) {BS=0;}
		  USART_Tx128(BS_AD,BS); 
		

		 break;
		 
	     case BSR:
		  BS=0;
		  USART_Tx128(BS_AD,BS);
		  
		 break;
		 
	     case BS1:
		  BS++;
		  USART_Tx128(BS_AD,BS); 
	
		 
		 break;
		 
	     case BS2:
		  BS+=2;
		 
		  USART_Tx128(BS_AD,BS);
		
		 break;
		 
	     case BS3:
		  BS+=3;
		  
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
		  USART_Tx128(GC_AD,gcm);   
		    
		 break;
		 
	     case GCR: 
		  //timer reset;
		  USART_Tx128(GC_AD,gcrst);
			   
		 break;
		 
	     case GC1: 
		 // minutes+1, seconds=0
		  USART_Tx128(GC_AD,gcp); 
			  
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
	  lcdnumdata(25,3,AS);
	 lcdnumdata(85,3,BS);
	 lcdnumdata(55,7,QT);
	 lcdnumdata(5,7,AF);
	 lcdnumdata(110,7,BF);
 }

