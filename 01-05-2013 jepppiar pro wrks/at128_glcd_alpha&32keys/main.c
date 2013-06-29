#include<avr/io.h>
#define FCPU 160000000UL
#include<util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include<main.h>
#include<at128_USART.c>
#include<GLCD.c>
#include "global.c"
//#include<surya.h>
#include<sportronix.h>


volatile unsigned int rx_char=0,ext=0,i=0,j=0,alpha=0;

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
	
	while(ext==0)
	{
	
	lcdputs2(16,3,ar7);		
	setcolumn(40);
	setpage(4);
	
	for(int k=0;((k<10));k++)
	{
	
	if(teama[k]!='0')
	{
	lcdsim_chardata((40+(k*8)),4,teama[k]);
	}
	else {lcd_bs(6);}
	}
	}
	
	
	clrlcd();
	_delay_ms(10);
	while(ext==1)
	{
	lcdputs2(16,3,ar8);		
	setcolumn(40);
	setpage(4);

	for(int k=0;((k<10));k++)	//&(teamb[k]!='0')
	{
	if(teamb[k]!='0')
	{lcdsim_chardata((40+(k*8)),4,teamb[k]);}
	else {lcd_bs(6);}
	}
	}
	
	clrlcd();
	setcolumn(40);
	setpage(4);
	for(int k=0;((k<10));k++)
	{
	if(teama[k]!='0')
	{
	lcdsim_chardata((40+(k*8)),4,teama[k]);
	}
	else {lcd_bs(6);}
	}
	
	
	{
	clrlcd();
	//lcdputs2(33,0,ar0); since score mate is not needed, 
	setcolumn(16);
	setpage(1);
	for(int k=0;((k<10));k++)
	{
	if(teama[k]!='0')
	{
	lcdsim_chardata((40+(k*8)),4,teama[k]);
	USART_Transmit(teama[k]);
	}
	else {lcd_bs(6);}
	}
	
	USART_Transmit(0x38);
	setcolumn(80);
	setpage(1);
	for(int k=0;((k<10));k++)
	{
	if(teamb[k]!='0')
	{
	lcdsim_chardata((40+(k*8)),4,teamb[k]);
	}
	else {lcd_bs(6);}
	}
	
	
	lcdputs2(46,2,ar3);		//score
	
	lcdputs2(5,6,ar4);
	lcdputs2(110,6,ar4);		//FOUL
	lcdputs2(58,6,ar6);
	}
	

	while(1)
	{
	 lcdnumdata(25,3,AS);
	 lcdnumdata(85,3,BS);
	 lcdnumdata(55,7,QT);
	 lcdnumdata(5,7,AF);
	 lcdnumdata(110,7,BF);

 }
}





ISR(USART0_RX_vect)  // @ CTrL side
{
rx_char=UDR0;
if(rx_char==0x05){alpha=1;}
if(alpha!=1)
{
 
  if(rx_char==0x08)
 {
 if (ext==0){i--;}
 if (ext==1){j--;}
 }
 if(rx_char==0x07){ext++;}
 if(ext==0)
 { 

			if(rx_char==0x06){i--;if(i<0){i=0;}teama[i]='0';}
			if(((rx_char)!=(0x08))&((rx_char)!=(0x06))&((rx_char)!=(0x07)))
			{
			teama[i]=rx_char;
			i++;
			if(i>9){i=0;}
			if(i<0){i=0;}
			}
 }
 
if(ext==1)
{
		if(rx_char==0x06){j--;if(j<0){j=0;}teamb[j]='0';}
		if(((rx_char)!=(0x08))&((rx_char)!=(0x06))&((rx_char)!=(0x07)))
		{
		teamb[j]=rx_char;
		j++;
		if(j>9){j=0;}
		if(j<0){j=0;}
		}
	
}
	
}

else 
{


	 PORTD^=0XFF;
//	 USART_Transmit("8");
	 switch(rx_char)
	  {
	 
	     case AS9:
		 // AS--;		  
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
		  if(AF>9)
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
		 //USART_Transmit(TF_rv); why? to praba
		 break;
		 
	     case BF9:
		  BS--;
		  USART_Tx128(BF_AD,BF);   
		 break;
		 
	     case BFR:
		  BS=0;
		  USART_Tx128(BF_AD,BF);   
		 break;
		 
	     case BF1:
		  BF++;
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
		   USART_Tx128(TFR_AD,TF_rp); 
		 break;
		 
	     case GCR: 
		  //timer reset;
		  USART_Tx128(GC_AD,gcrst);
			USART_Tx128(TFR_AD,TF_rv);    
		 break;
		 
	     case GC1: 
		 // minutes+1, seconds=0
		  USART_Tx128(GC_AD,gcp); 
			 USART_Tx128(TFR_AD,TF_rp); 
		 break;
		 
	     case GSP:
		  
		  USART_Tx128(GCSP_AD,0);   
		   USART_Tx128(TFR_AD,TF_sp); 
		 break;
		 
	     case R24: ///////////////////////////////////////////////////////////////
		  
		  USART_Tx128(TFR_AD,TF_rv); 
		 break;
		 
	     case R13: ///////////////////////////////////////////////////////////////
		  //;
		   USART_Tx128(THR_AD,TF_tr);    
		 break;
		 
	     case BPT:
		  ARO^=1;
		  USART_Tx128(ARO_AD,ARO);   
		 break;
	  }
 }

}