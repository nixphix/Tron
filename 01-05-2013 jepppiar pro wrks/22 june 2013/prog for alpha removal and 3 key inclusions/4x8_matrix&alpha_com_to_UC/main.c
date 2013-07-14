#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"T1/keypad_uc.h"
#include<T1/keypad_uc.c>



int main(void)
{
	_SYSTEM_INIT();
	
	while(1)
	{
	keypad_32keys();	
	}
	return 0;
	
}