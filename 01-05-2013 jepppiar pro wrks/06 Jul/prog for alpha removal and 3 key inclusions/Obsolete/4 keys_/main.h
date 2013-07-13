#define dport PORTA	// LCD data port
#define cport PORTF

#define en 0x07		// enable signal  (cntrl)   for smd 0x07
#define rs 0x05		// read/write signal (cntrl1) for smd 0x05
#define rw 0x06		// register select signal (cntrl1) 0x06
#define cs1 0x03    // 0x03
#define cs2 0x04     //0x04
#define chip1				0x01
#define chip2				0x02
#define chip3				0x03
int _av=0, _ch=0;

#define ZA PB0		// Z TO A BUTTON (-)
#define HOME PB1    // HOME BUTTON
#define AZ PB2      // A TO Z BUTTON (+)
#define BUZZ PB3    // BUZZER BUTTON

#define MAX_CHAR 6 // Maximum no of character that can be displayed in the LED displays. Font: 7x5+2

uint8_t _okey_,_nkey_,d_nkey=5,ext=0,menu=0,d_menu=0,o_sec=0;	// 


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
volatile unsigned char teama[MAX_CHAR]={'0','0','0','0','0','0'};
volatile unsigned char teamb[MAX_CHAR]={'0','0','0','0','0','0'};
volatile unsigned char teamc[2]={'a','b',};

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