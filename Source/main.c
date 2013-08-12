

#include "defines.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include "lcd.h"

FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{

  lcd_init();

  stdout = &lcd_str;

  printf_P( PSTR("Hello world!\nAlles klar\n") );

  return 0;
}
