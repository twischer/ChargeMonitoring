#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include "defines.h"
#include "lcd.h"

struct FixedPoint
{
	int16_t integer;
	uint16_t decimal;
};

const uint16_t MAX_REMAINING_HOURS = 999;

// the current which is running now (in mA)
volatile int32_t current = -3056;
// the remaining capacity of the batteries (in mAh)
volatile int32_t remainingCapacity = 600678;

FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);


const struct FixedPoint getFixedPoint(const int32_t value, const uint16_t divider);


int main(void)
{
	lcd_init();
	stdout = &lcd_str;
	
	while (true)
	{
		// benötigt über 1k weniger als printf
		//  char test[20];
		//  ltoa( current, test, 10 );
		
		const struct FixedPoint convertedCurrent = getFixedPoint(current, 1000);
		printf_P( PSTR(" %04d,%03d A  "), convertedCurrent.integer, convertedCurrent.decimal );
		
		const struct FixedPoint convertedCapacity = getFixedPoint(remainingCapacity, 1000);
		printf_P( PSTR("%04d,%03d Ah\n"), convertedCapacity.integer, convertedCapacity.decimal );
		
		
		const uint32_t remainingTimeInMinutes = remainingCapacity * 60 / -current;
		if ( (remainingTimeInMinutes / 60) <= MAX_REMAINING_HOURS)
		{
			const struct FixedPoint convertedTime = getFixedPoint(remainingTimeInMinutes, 60);
			printf_P( PSTR("Remaining time: %03d:%02d h\n"), convertedTime.integer, convertedTime.decimal );
		}
		else
		{
			printf_P( PSTR("Rem. time not available\n") );
		}
	}
	
	return 0;
}


const struct FixedPoint getFixedPoint(const int32_t value, const uint16_t divider)
{
	struct FixedPoint convertedValue;
	convertedValue.integer = (int16_t)(value / divider);
	convertedValue.decimal = abs(value) % divider;
	
	return convertedValue;
}
