#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
	
DDRC = 0xFF;
DDRB = 0xFF;
PORTC = 0x55;
while(1)
{
   PORTC = 0x55;
   PORTB = 0x55;
   _delay_ms(100);
   PORTC ^= 0xFF;
   PORTB ^= 0xFF;
   _delay_ms(100);
}

return 0;
}