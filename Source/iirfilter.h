/*
 * This file implements an average filter with a configurable depth
 */
#include <stdint.h>

#define FIXPOINT_DOT	13


// every 16ms
#define SAMPLE_FREQUENCY	62
#define CUT_OFF_FREQUENCY	1

#define COEFFICIENT_CALC	(SAMPLE_FREQUENCY / (2 * M_PI * CUT_OFF_FREQUENCY))
#define COEFFICIENT			50


void IIRFilter_reset(void);
const int16_t IIRFilter_calc(const int16_t value);
