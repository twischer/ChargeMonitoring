/*
 * This file implements an average filter with a configurable depth
 */
#include <stdint.h>

#define AVGFILTER_DEPTH		128

void AVGFilter_reset(void);
void AVGFilter_add(const int16_t value);
const int16_t AVGFilter_get(void);
