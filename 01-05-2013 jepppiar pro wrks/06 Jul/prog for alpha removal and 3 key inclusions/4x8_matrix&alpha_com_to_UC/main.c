#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"include_me/keypad_uc.h"
#include"include_me/keypad_uc.c"

int main(void)
{
	_SYSTEM_INIT();
	
	while(1)
	{
	  keypad_32keys();	
	}
	return 0;
	
}