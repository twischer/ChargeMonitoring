/*
 * This file implements an iir filter
 */
#include "iirfilter.h"

int32_t IIRFilter_lastResultFixPoint;


void IIRFilter_reset(void)
{
	IIRFilter_lastResultFixPoint = 0;
}


const int16_t IIRFilter_calc(const int16_t value)
{
	const int32_t valueFixPoint = (int32_t)(value) << FIXPOINT_DOT;
	
	const int32_t diffFixPoint = (valueFixPoint - IIRFilter_lastResultFixPoint) / COEFFICIENT;
	const int32_t resultFixPoint = IIRFilter_lastResultFixPoint + diffFixPoint;
	IIRFilter_lastResultFixPoint = resultFixPoint;
	
	const int16_t result = resultFixPoint >> FIXPOINT_DOT;
	return result;
}
