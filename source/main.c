#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "hardware/linesensors.h"
#include "hardware/motors.h"
#include "hardware/uart.h"

#include "logic/main.h"

int
main()
{
  setup();
  sei();
  for (;;)
    loop();
}

void
setup()
{
  motors_init();
  linesensors_init();
  linesensors_set_threshold((linesensors_t){ 170, 160, 160, 160, 170 });
  uart_init();
}

void
print_sensors(linesensors_t ls)
{

  char str[100];

  sprintf(str, "%i|%i|%i|%i|%i\n", ls.left, ls.midl, ls.mid, ls.midr, ls.right);

  uart_puts(str);
}

void
print_lines(line_t ls)
{
  char str[100];

  sprintf(str,
          "%i|%i|%i|%i|%i\n",
          (ls >> 0) & 1,
          (ls >> 1) & 1,
          (ls >> 2) & 1,
          (ls >> 3) & 1,
          (ls >> 4) & 1);

  uart_puts(str);
}

// void loop()
// {
//     linesensors_update();
//     print_sensors(linesensors);

//     _delay_ms(300);
// }

void
loop()
{
  if (linesensors_get() & RIGHT) {
    while (linesensors_get() & RIGHT) {
      motors_forward();
    }
    if (linesensors_get() & CENTERED) {
      motors_rotate_pos();
      while (linesensors_get() & CENTERED)
        ;
      while (!(linesensors_get() & RIGHT_TILTED))
        ;
    } else {
      motors_rotate_pos();
      while (!(linesensors_get() & RIGHT_TILTED))
        ;
    }
  } else if (linesensors_get() & LEFT) {
    uart_puts("L\n");
    while (linesensors_get() & LEFT) {
      motors_forward();
    }
    if (linesensors_get() & CENTERED) {
      motors_rotate_neg();
      while (linesensors_get() & CENTERED)
        ;
      while (!(linesensors_get() & RIGHT_TILTED))
        ;
    } else {
      motors_rotate_neg();
      while (!(linesensors_get() & RIGHT_TILTED))
        ;
    }
  } else if (linesensors_get() & CENTERED) {
    line_t ls = linesensors_get();
    if (ls & LEFT_TILTED)
      motors_lean_pos();
    else if (ls & RIGHT_TILTED)
      motors_lean_neg();
    else
      motors_forward();
  } else {
    motors_reverse();
  }
}
