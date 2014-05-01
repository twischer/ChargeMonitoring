/*
 * This file implements an average filter with a configurable depth
 */
#include "avgfilter.h"

int16_t AVGFilter_values[AVGFILTER_DEPTH];


void AVGFilter_reset(void)
{
	for (uint8_t i=0; i<AVGFILTER_DEPTH; i++)
		AVGFilter_values[i] = 0;
}


void AVGFilter_add(const int16_t value)
{
	static uint8_t nextValueIndex = 0;
	AVGFilter_values[nextValueIndex] = value;
	
	nextValueIndex = (nextValueIndex + 1) % AVGFILTER_DEPTH;
}

const int16_t AVGFilter_get(void)
{
	int32_t averageValue = 0;
	for (uint8_t i=0; i<AVGFILTER_DEPTH; i++)
		averageValue += AVGFilter_values[i];
	
	averageValue /= AVGFILTER_DEPTH;
	
	return averageValue;
}
