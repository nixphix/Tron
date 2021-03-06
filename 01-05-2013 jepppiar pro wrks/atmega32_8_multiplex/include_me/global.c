/*
#define AS9  0x05
#define ASR  0x06
#define AS1  '7'
#define _AS2  '8'
#define AS3  '9'
#define AF9  'A'
#define AFR  'B'
#define AF1  'C'
#define AT9  'D'
#define ATR  'E'
#define AT1  'F'
#define BS9  '10'
#define BSR  '11'
#define BS1  '12'
#define BS2  '13'
#define BS3  '14'
#define BF9  '15'
#define BFR  '16'
#define BF1  '17'
#define BT9  '18'
#define BTR  '19'
#define BT1  '1A'
#define QT9  '1B'
#define QTR  '1C'
#define QT1  '1D'
#define GC9  '1E'
#define GCR  '1F'
#define GC1  '20'
#define GSP  '21'
#define R24  '22'
#define R13  '23'
#define BPT  '24'
*/
#define AS9  0x05
#define ASR  0x06
#define AS1  0x07
#define _AS2  0x08
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


//Atmega32 Mainboard
uint8_t data=0, odata=100, e=0, nkey = 8 , okey = 9;
const uint8_t keymap[9][3] = {{'A','B','C',},{'D','E','F',},{'G','H','I',},{'J','K','L',},{'M','N','O',},{'P','Q','R',},{'S','T','U',},{'V','W','X',},{'Y','Z','.',}};
volatile uint8_t RXC_ISR_DATA[4], RXC_ISR_INDEX=0, END_BYTE=0x45, CHK_SUM=0;
volatile uint8_t VB_DATA=0;




volatile int ASH=0,BSH=0,AS=0,AF=0,AT=0,BS=0,BF=0,BT=0,QT=0,ARO=0,GCSP=1,GC=2,gcm=9,gcrst=0,gcp=1;

volatile uint8_t AS_AD=200,AF_AD=201,AT_AD=202,BS_AD=210,BF_AD=211,BT_AD=212,QT_AD=220,ARO_AD=221,GCSP_AD=222,GC_AD=223;

// LED Matrix 
volatile uint8_t NBA0_AD=100,NBA1_AD=101,NBA2_AD=102,NBA3_AD=103,NBA4_AD=104,NBA5_AD=105,NBB0_AD=150,NBB1_AD=151,NBB2_AD=152,NBB3_AD=153,NBB4_AD=154,NBB5_AD=155;

volatile uint8_t  FO_MAX=5, TO_MAX=5, QT_MAX=5, buzzer=0;
volatile int buzCount=0,GC_SET=0 ;