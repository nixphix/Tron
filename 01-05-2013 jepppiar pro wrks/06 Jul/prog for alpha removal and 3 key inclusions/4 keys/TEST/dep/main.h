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
int _av=0, _ch=0, km=0;

#define c1 PB0		// four keys
#define c2 PB1
#define c3 PB2
#define c4 PB3

uint8_t _okey_,_nkey_,d_nkey=5,ext=0,d_menu=0,o_sec=0;	// 
int menu=0;

void lcddata(uint8_t *value, unsigned int limit);
void lcdnumdata(uint8_t,uint8_t,uint8_t); 
void lcdsim_chardata(uint8_t _y,uint8_t _x,uint8_t _z); 
void lcdchardata(uint8_t __y,uint8_t __x,uint8_t * _z);
void lcd_bs(uint8_t ___y);



uint8_t c,z=0,ASH_U=0,BSH_U=0,ASH_D=0,BSH_D=0;
uint8_t ar0[]="SCORE MATE";
uint8_t ar1[]="TEAM A";
uint8_t ar2[]="TEAM B";                 
uint8_t ar3[]="SCORE";
uint8_t ar4[]="F";
uint8_t ar5[]="T-OUT";
uint8_t ar6[]="QTR";
uint8_t ar7[]="Enter Team A Name";
uint8_t ar8[]="Enter Team B Name";
uint8_t ar9[]=" Buz > Backspace ";
uint8_t tA[]={'H','O','M','E',0x01};
uint8_t tB[]={'A','W','A','Y',0x01};

volatile uint8_t teama[10]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00};
volatile uint8_t teamb[10]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00};
//volatile uint8_t teamc[2]={'a','b',};


//ControlPanel
volatile int8_t cpSelection=0,cpStatus[5];

uint8_t cp0[]="  Control Panel  ";
uint8_t cp1[]=" ON  ";
uint8_t cp2[]=" OFF ";
uint8_t cp3[]="24s Clock:";


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