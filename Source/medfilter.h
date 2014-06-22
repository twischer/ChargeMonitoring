/*
 * This file implements an median filter with a configurable depth
 */
#include <stdint.h>


void MedFilter_reset(void);
const int16_t MedFilter_calc(const int16_t value);
