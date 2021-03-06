#define dport PORTA	// LCD data port
#define cport PORTF

#define en 0x07		// enable signal  (cntrl)
#define rs 0x05		// read/write signal (cntrl1)
#define rw 0x06		// register select signal (cntrl1)
#define cs1 0x03
#define cs2 0x04
#define chip1				0x01
#define chip2				0x02
#define chip3				0x03


void lcddata(char *value, unsigned int limit);
void lcdnumdata(unsigned char,unsigned char,unsigned char); 
void lcdsim_chardata(unsigned char _y,unsigned char _x,unsigned char _z); 
void lcdchardata(unsigned char __y,unsigned char __x,unsigned char * _z);
void lcd_bs(unsigned char ___y);



unsigned char c,z=0;
unsigned char ar0[]="SCORE MATE";
unsigned char ar1[]="TEAM A";
unsigned char ar2[]="TEAM B";                 
unsigned char ar3[]="SCORE";
unsigned char ar4[]="F";
unsigned char ar5[]="T-OUT";
unsigned char ar6[]="QTR";
unsigned char ar7[]="Enter TEAM A NAME";
unsigned char ar8[]="Enter TEAM B NAME";
volatile unsigned int teama[10]={'0','0','0','0','0','0','0','0','0','0'};
volatile unsigned int teamb[10]={'0','0','0','0','0','0','0','0','0','0'};
volatile unsigned int teamc[2]={'a','b',};

/*	    lcdputs2(33,0,ar0);
		lcdputs2(16,1,ar1);
		lcdputs2(80,1,ar2);
		lcdputs2(16,3,ar3);		//score
		lcdputs2(96,3,ar3);		//score
		lcdputs2(1,6,ar4);
		lcdputs2(100,6,ar4);		//FOUL
		
		//lcdputs2(56,6,ar6);		//QTR
	//	lcdputsdata(40,4,v);
	//	lcdnumdata(40,4,scorea); */																	//use this to print numbers