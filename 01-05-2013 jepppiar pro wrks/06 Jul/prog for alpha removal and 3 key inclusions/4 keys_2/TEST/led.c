#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
	
DDRC = 0xFF;
PORTC = 0x55;
while(1)
{
   _delay_ms(500);
   PORTC ^= 0xFF;
}

return 0;
}