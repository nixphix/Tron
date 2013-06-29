#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include "keypad_uc.h"
#include<keypad_uc.c>



int main(void)
{
	_SYSTEM_INIT();
	
	while((_ext==0))
	{
	keypad_alpha();
	}
	
	while(1)
	{
	keypad_32keys();	
	}
	return 0;
	
}