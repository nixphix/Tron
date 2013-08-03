#include "font2.h"
void displayoff(void)
{
	cport =(0<<rs)|(0<<rw)|(0<<cs1)|(0<<cs2)|(0<<en);								//cs1=0;cs2=0,en=0;
}


		 
void displayon(void)
{ 
	displayoff();
	dport=0x3F;  
	cport =(0<<rs)|(0<<rw)|(1<<cs1)|(1<<cs2)|(1<<en);
	_delay_us(45);
	cport =(0<<rs)|(0<<rw)|(1<<cs1)|(1<<cs2)|(0<<en);
	_delay_us(45);
}
void setcolumn(uint8_t y)
{
	
	if(y<64)
	{
		displayoff();
		c=y;
		dport=(0x40|(y&63));	  																					//0x40 represents Column 0
		cport = (1<<cs1)|(0<<cs2)|(0<<rs)|(0<<rw)|(1<<en);																//rs=0;rw=0;
		_delay_us(45);
		cport = (1<<cs1)|(0<<cs2)|(0<<rs)|(0<<rw)|(0<<en);
		_delay_us(45);
	}
	else
	{ 
		displayoff();
		c=y;
		dport=0x40|((y-64)&63);	  																				//0x40 represents Column 0
		cport=(0<<cs1)|(1<<cs2)|(0<<rs)|(0<<rw)|(1<<en);																	//rs=0;rw=0;
		_delay_us(45);
		cport=(0<<cs1)|(1<<cs2)|(0<<rs)|(0<<rw)|(0<<en);	
		_delay_us(45);
		
	}
}
void setpage(uint8_t x)
{
	
	displayoff();
	dport= (0xb8|x);	   																							//0xb8 represents Page 0
	cport=(1<<cs1)|(1<<cs2)|(0<<rs)|(0<<rw)|(1<<en);																	//rs=0;rw=0;
	_delay_us(45);
	cport=(1<<cs1)|(1<<cs2)|(0<<rs)|(0<<rw)|(0<<en);	
	_delay_us(45);
}



void lcddata(char *value, unsigned int limit)
{
	
	unsigned int i;
	for(i=0;i<limit;i++)
	{
		if(c<64)
		{
			dport=value[i];
			cport= (0<<cs2)|(1<<cs1)|(1<<rs)|(0<<rw)|(1<<en);				//rs=1;rw=0;
			_delay_us(45);
			cport= (0<<cs2)|(1<<cs1)|(1<<rs)|(0<<rw)|(0<<en);
			_delay_us(45);
			c++;
		}

		else
		{
			setcolumn(c); 
			dport=value[i];
			cport = (0<<cs1)|(1<<cs2)|(1<<rs)|(0<<rw)|(1<<en);			//ctrl1 =(1<<rs)|(0<<rw);			//rs=1;rw=0;
			_delay_us(45);
			cport = (0<<cs1)|(1<<cs2)|(1<<rs)|(0<<rw)|(0<<en);
			_delay_us(45);
			c++;
		}
	if(c>127)
	return;
	}
	
	
}
void clrlcd(void)
{
    uint8_t i,j;
    for (i=0;i < 8;i++)
    {
	setpage(i);
	setcolumn(0);
	for (j= 0 ;j < 128; j++)
           lcddata(&z,1);
    }
}


void lcdchardata(uint8_t __y,uint8_t __x,uint8_t * _z)
{
	setcolumn(__y);
	setpage(__x);
	int _h,_i;
	for(int k=0;((k<10));k++) 
	{
	if(_z[k]!='0')
	{
	_h=((_z[k])-65);
	_i=((_h *5)+165);
	lcddata(&font5x7[_i],5);
	lcddata(&z,1);
	}
	else {lcd_bs(6);}
	
	
	
	}
}

void lcdsim_chardata(uint8_t _y,uint8_t _x,uint8_t _z)
{
	int _h,_i;
	
	
	_h=((_z-65));  //-65
	_i=((_h *5)+165);
	//setcolumn(_y);
	//setpage(_x);
	lcddata(&font5x7[_i],5);
	lcddata(&z,1);
	
}

void picture(uint8_t * val)
{
	uint8_t j;
	setcolumn(0);
	setpage(0);
	for (j=0;j<8;j++)
	{
		setpage(j);
		setcolumn(0);
		lcddata(&val[(j*128)],128);				 
	}
}


void lcdnumdata(uint8_t __y,uint8_t __x,uint8_t __z)
{

int u,v,w,t;
if(__z>99) 				// 198
{
	
	u=((__z/100)*5)+80;			// 1
	v=(__z%100);			//98
	w=((v/10)*5)+80;			//9
	t=((v%10)*5)+80;			//8
	setcolumn(__y);
	setpage(__x);	
lcddata(&font5x7[u],5);
	lcddata(&font5x7[w],5);
	lcddata(&font5x7[t],5);
}
if(__z<99)
{
	w=((__z/10)*5)+80;			//9
	t=((__z%10)*5)+80;			//8
	setcolumn(__y);
	setpage(__x);
	lcddata(&font5x7[w],5);
	lcddata(&z,1);
	lcddata(&font5x7[t],5);
	lcddata(&z,1);
	
}
}

void lcd_bs(uint8_t ___y)
{
 lcddata(&font5x7[0],___y);
}
void lcdputs1(uint8_t y,uint8_t x,uint8_t *str)
{
	uint8_t i;
	unsigned int a;
	setcolumn(y);
	setpage(x);
	for(i=0;str[i]!=0;i++)
	{
		a=(*(str+i));
		a*=8;
//		lcddata(&Character8x8[a],8);
		lcddata(&z,1);
	}
	
}

void lcdputs2(uint8_t y,uint8_t x,uint8_t *str)
{
	uint8_t i;
	unsigned int a;
	setcolumn(y);
	setpage(x);
	for(i=0;str[i]!=0;i++)
	{
		a=(*(str+i)-32);
		a*=5;
	lcddata(&font5x7[a],5);
		lcddata(&z,1);
	}
	
}

void lcdputsdata(uint8_t y,uint8_t x,uint8_t z)
{
setcolumn(y);
setpage(x);
lcddata(&font5x7[z],5);

}



void GLCD_Init128(void)
{
	DDRA=0xFF;
	DDRF=0xFF;
	
	clrlcd();
	_delay_us(50);
		displayon(); 
		setcolumn(0);
		setpage(0);	
	_delay_us(100);	

}

