//  32keys declaration
#define PULLUP_ROW(X) PORTA = ((PORTA & 0xF0)||(1<<X))
#define ROW1 PA0
#define ROW2 PA1
#define ROW3 PA2
#define ROW4 PA3
 
#define COLUMN1 PB0
#define COLUMN2 PB1
#define COLUMN3 PB2
#define COLUMN4 PB3
#define COLUMN5 PB4
#define COLUMN6 PB5
#define COLUMN7 PB6
#define COLUMN8 PB7

uint8_t check1_32keys(void);
uint8_t check2_32keys(void);  
uint8_t check3_32keys(void);
uint8_t check4_32keys(void);
uint8_t check_key_32keys(void);
void keypad_32keys(void);

uint8_t _okey_,_nkey_;	// for 32key

void t1_rst(void);
volatile uint8_t odata=100,b=0;



	uint8_t nkey,okey,data=0,e=0,i=0,d_nkey,d_okey;	// for alpha
	unsigned int teama[6]={0,0,0,0,0,0};
	const uint8_t keymap[9][3] = {{65,66,67,},{68,69,70,},{71,72,73,},{74,75,76,},{77,78,79,},{80,81,82,},{83,84,85,},{86,87,88,},{89,90,'.',}};
	//const uint8_t keymap[9][3] = {{'A','B','C',},{'D','E','F',},{'G','H','I',},{'J','K','L',},{'M','N','O',},{'P','Q','R',},{'S','T','U',},{'V','W','X',},{'Y','Z','.',}};


// general declarations


void USART_Init(unsigned int);
void USART_Transmit(unsigned int);
uint8_t USART_Receive(void);
uint8_t _ext=0;

/*#define AS9  0x05
#define ASR  0x06
#define AS1  0x07
#define AS2  0x08
#define AS3  0x09
#define AF9  0x0A
#define AFR  0x0B
#define AF1  0x0C
#define AT9  0x0D
#define ATR  0x0E
#define AT1  0x0F 
#define BS9  0x10
#define BSR  0x11
#define BS1  0x12
#define BS2  0x13
#define BS3  0x14
#define BF9  0x15 
#define BFR  0x16
#define BF1  0x17
#define BT9  0x18 
#define BTR  0x19
#define BT1  0x1A
#define QT9  0x1B
#define QTR  0x1C
#define QT1  0x1D
#define GC9  0x1E
#define GCR  0x1F
#define GC1  0x20
#define GSP  0x21
#define R24  0x22
#define R13  0x23
#define BPT  0x24
*/

#define AS9  0x05
#define ASR  0x06
#define AS1  0x07
#define AS2  0x08
#define AS3  0x09
#define AF9  0x0A
#define AFR  0x0B
#define AF1  0x0C
#define AT9  0x0D
#define ATR  0x0E
#define AT1  0x0F // BOTH A TIM AND B FOUL
#define BS9  0x10
#define BSR  0x11
#define BS1  0x12
#define BS2  0x13
#define BS3  0x14
#define BF9  0x15 // QUARTER
#define BFR  0x16
#define BF1  0x17
#define BT9  0x18 // B FOUL
#define BTR  0x19
#define BT1  0x1A
#define QT9  0x1B
#define QTR  0x1C
#define QT1  0x1D
#define GC9  0x1E
#define GCR  0x1F
#define GC1  0x20
#define GSP  0x21
#define R24  0x22
#define R13  0x23
#define BPT  0x24
