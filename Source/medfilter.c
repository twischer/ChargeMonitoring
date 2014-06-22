/*
 * This file implements an median filter with a configurable depth
 */
#include "medfilter.h"

#define MEDFILTER_DEPTH		5


int16_t MedFilter_values[MEDFILTER_DEPTH];


void MedFilter_reset(void)
{
	for (uint8_t i=0; i<MEDFILTER_DEPTH; i++)
		MedFilter_values[i] = 0;
}


const int16_t MedFilter_calc(const int16_t value)
{
	static uint8_t nextValueIndex = 0;
	MedFilter_values[nextValueIndex] = value;
	nextValueIndex++;
	if (nextValueIndex >= MEDFILTER_DEPTH)
		nextValueIndex = 0;
	
#if (MEDFILTER_DEPTH == 3)
	if (MedFilter_values[0] > MedFilter_values[1]) {
		if (MedFilter_values[1] > MedFilter_values[2]) {
			return MedFilter_values[1];
		} else if (MedFilter_values[0] > MedFilter_values[2]) {
			return MedFilter_values[2];
		} else {
			return MedFilter_values[0];
		}
	} else {
		if (MedFilter_values[0] > MedFilter_values[2]) {
			return MedFilter_values[0];
		} else if (MedFilter_values[1] > MedFilter_values[2]) {
			return MedFilter_values[2];
		} else {
			return MedFilter_values[1];
		}
	}
#elif (MEDFILTER_DEPTH == 5)
	const int16_t a = MedFilter_values[0];
	const int16_t b = MedFilter_values[1];
	const int16_t c = MedFilter_values[2];
	const int16_t d = MedFilter_values[3];
	const int16_t e = MedFilter_values[4];
	
	const int16_t median = b < a ? d < c ? b < d ? a < e ? a < d ? e < d ? e : d
                                                                 : c < a ? c : a
                                                         : e < d ? a < d ? a : d
                                                                 : c < e ? c : e
                                                 : c < e ? b < c ? a < c ? a : c
                                                                 : e < b ? e : b
                                                         : b < e ? a < e ? a : e
                                                                 : c < b ? c : b
                                         : b < c ? a < e ? a < c ? e < c ? e : c
                                                                 : d < a ? d : a
                                                         : e < c ? a < c ? a : c
                                                                 : d < e ? d : e
                                                 : d < e ? b < d ? a < d ? a : d
                                                                 : e < b ? e : b
                                                         : b < e ? a < e ? a : e
                                                                 : d < b ? d : b
                                 : d < c ? a < d ? b < e ? b < d ? e < d ? e : d
                                                                 : c < b ? c : b
                                                         : e < d ? b < d ? b : d
                                                                 : c < e ? c : e
                                                 : c < e ? a < c ? b < c ? b : c
                                                                 : e < a ? e : a
                                                         : a < e ? b < e ? b : e
                                                                 : c < a ? c : a
                                         : a < c ? b < e ? b < c ? e < c ? e : c
                                                                 : d < b ? d : b
                                                         : e < c ? b < c ? b : c
                                                                 : d < e ? d : e
                                                 : d < e ? a < d ? b < d ? b : d
                                                                 : e < a ? e : a
                                                         : a < e ? b < e ? b : e
                                                                 : d < a ? d : a;
	
	return median;
#else
#error "No valid implemantaion for calulating the median found"
	return 0;
#endif
}

