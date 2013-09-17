#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include "defines.h"
#include "lcd.h"


#define ADC_OFFSET			18
// defines mA for one adc change
#define ADC_TO_CURRENT		131

#define TIMER0_PRESCALER	64

#define MAX_REMAINING_HOURS	999

#define BATTERY_COUNT		8


#define NAH_TO_MAH			1000000

// The frequency of the timer 0 (in Hz)
#define TIMER0_FREQUENCY	( F_CPU / (TIMER0_PRESCALER * 256) )
// the delay of timer 0 (in µs)
// should by 8192µs for 2MHz and a prescaler of 64 on an 8 bit timer
#define TIMER0_DELAY_US		(1000000 / TIMER0_FREQUENCY)
// the delay of timer 0 (in µmin)
// should be 136 µmin
#define TIMER0_DELAY_UMIN	(TIMER0_DELAY_US / 60)


#define BUFFER_LENGTH		9
#define POINT_POSITION		(BUFFER_LENGTH - 5)

// contains the last value which was read from the adc
volatile uint16_t lastADCValue = 0;
// the current which is running now (in mA)
volatile int32_t current = 0;
// the capacity change of the last mesurments (in nAh)
volatile int32_t capacityDifference = 0;
// voltages of all batteries (in mV)
volatile int16_t voltages[BATTERY_COUNT];


FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

void showVoltagesOfAllBatteries(void);
void showCurrentAndTime(const int32_t remainingCapacity);

void convertValueToString(const int32_t value, char string[]);


// TODO adc auf 62,5kHz mit 16er Teiler

// TODO Timer auf 1kHz
// - adc wert an current kopieren
// - static int32_t newCapacityDiff in nAh
// wenn > 1mAh, 1mAh auf remainingCapacity addieren
// - adc neu starten (single conversion um energie zu sparen)


ISR (TIMER0_OVF_vect)
{
//	lastADCValue = ADC;
	
	// start new conversion
	ADCSRA |= (1 << ADSC);
	
	// calulate the current of the mesurnment (in mA)
	const int16_t adcWithoutOffset = (int16_t)(lastADCValue) - ADC_OFFSET;
	current = -30;//(int32_t)(adcWithoutOffset) * ADC_TO_CURRENT;
	
	// capacity difference of this current mesurment (in mAµmin = nAmin)
	int32_t newCapacityDiff = current * TIMER0_DELAY_UMIN;
	// capacity (in nAh)
	newCapacityDiff /= 60;
	// capacitiy difference of the last mesurments (in nAh)
	capacityDifference += newCapacityDiff;
	
	
	lastADCValue = TCNT0;
}


int main(void)
{
	lcd_init();
	stdout = &lcd_str;
	
	
	// TODO überprüfen ob mit attiny26 auch das gleiche
	// Timer 0 konfigurieren
	TCCR0 = (0 << CS02) | (1 << CS01) | (1 << CS00); // Prescaler 64
	
	// Overflow Interrupt erlauben
	TIMSK |= (1 << TOIE0);
	
	// Global Interrupts aktivieren
	sei();
	
	
	// the remaining capacity of the batteries (in mAh)
	int32_t remainingCapacity = 6000;
	
	// reset capacitiy calulation if the remaning capacitiy is negative and the batteries are charging now
	// possiblly the batteries has grown or was charged externally before
	if (remainingCapacity < 0 && current > 0)
	{
		remainingCapacity = 0;
	}
	
	
	bool showVoltages = false;
	while (true)
	{
		// copy remaining capacity from nAh to mAh buffer
		if (capacityDifference > NAH_TO_MAH)
		{
			capacityDifference -= NAH_TO_MAH;
			remainingCapacity++;
		}
		else if (capacityDifference < -NAH_TO_MAH)
		{
			capacityDifference += NAH_TO_MAH;
			remainingCapacity--;
		}
		
		// show the requestet informations on the lcd
		if (showVoltages)
		{
			showVoltagesOfAllBatteries();
		}
		else
		{
			showCurrentAndTime(remainingCapacity);
		}
	}
	
	return 0;
}


void showVoltagesOfAllBatteries(void)
{
	for (uint8_t i=0; i<BATTERY_COUNT; i++)
	{
		// TODO use convertValueToString and parameter length and decimal point position
		char convertedString[BUFFER_LENGTH];
		printf_P( PSTR("%s "), convertedString );
	}
}


void showCurrentAndTime(const int32_t remainingCapacity)
{
	char convertedString[BUFFER_LENGTH];
	convertValueToString(current, convertedString);
	printf_P( PSTR("%s A   "), convertedString );
	
	convertValueToString(remainingCapacity, convertedString);
	printf_P( PSTR("%s Ah\n"), convertedString );
	
	
	const uint32_t remainingTimeInMinutes = remainingCapacity * 60 / -current;
	const uint16_t remainingHours = (uint16_t)(remainingTimeInMinutes / 60);
	if (true)// TODO only for testing (remainingHours <= MAX_REMAINING_HOURS)
	{
		const uint8_t remainingMinutes = (uint8_t)(remainingTimeInMinutes % 60);
		printf_P( PSTR("%04d Rem. time: %03d:%02d h\n"), lastADCValue, remainingHours, remainingMinutes );
	}
	else
	{
		printf_P( PSTR("Rem. time not available.\n") );
	}
}


void convertValueToString(const int32_t value, char string[])
{
	ltoa(value, string, 10);
	
	// do not move and not override the first character if it is a minus
	const uint8_t endOfCopiing = (string[0] == '-') ? 1 : 0;
	
	int8_t leftNumbers = strlen(string);
	for (int8_t i=BUFFER_LENGTH-1; i>=endOfCopiing; i--)
	{
		if (i == POINT_POSITION)
		{
			// add the decimal point to the string
			string[i] = '.';
		}
		else if (leftNumbers >= endOfCopiing)
		{
			// insert converted numbers
			string[i] = string[leftNumbers];
			leftNumbers--;
		}
		else
		{
			// fill up string with 0
			string[i] = '0';
		}
	}
}
