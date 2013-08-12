#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include "defines.h"
#include "lcd.h"


// the current which is running now (in mA)
volatile int32_t current = -30456;
// the remaining capacity of the batteries (in mAh)
volatile int32_t remainingCapacity = 25678;

FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);


int main(void)
{
  lcd_init();
  stdout = &lcd_str;
  
  
  printf_P( PSTR("%07d mA "), current );
  printf_P( PSTR("%07d mAh\n"), remainingCapacity );
  
  const uint32_t remainingTimeInMinutes = remainingCapacity * 60 / -current;
  const uint32_t remainingHours = remainingTimeInMinutes / 60;
  const uint32_t remainingMinutes = remainingTimeInMinutes % 60;
  
  printf_P( PSTR("Remaining time %04d:%02d h\n"), remainingHours, remainingMinutes );

  return 0;
}
