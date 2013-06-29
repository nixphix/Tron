#include <avr/io.h>

static int AlphaPointer=0,NBA_INDEX;
static char AlphaNum[36]={'A','B','C','D','E','F',G''H,'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0'};
static char NBA[8];
void main()
{

AlphaPointer=0;
while(1){
//PORTC 0-SET, 1-INC, 2-DEC
if(PINC==2)//PC1
{
	Lcd(AlphaPointer--);
	_delay(20);
}

if(PINC==4)//PC2
{
	Lcd(AlphaPointer++);
	_delay(20);
	
}

if(PINC==1)//PC0
{
	NBA[NBA_INDEX]=AlphaNum[AlphaPointer];
	_delay(20);
	
}

}

}

void Lcd(int num){
cd_clear_team_name();
lcd_display_NBA();
lcd_display_last(AlphaNum[num]);
}