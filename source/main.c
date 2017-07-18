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
  linesensors_set_threshold((linesensors_t){ 100, 100, 100, 100, 120 });
  uart_init();
}

line_t curr, prev;

char to_send[25];

void
loop()
{
  // get the new linesensors input
  line_t new = linesensors_get();
  if (new == curr)
    return;

  // the previous value of the sensors gets the last value
  // the new value are assigned to curr
  prev = curr;
  curr = new;

  // compensate for the tilting error
  if ((prev & RIGHT) && (prev & LEFT) && (curr & (LEFT | RIGHT))) {
    curr |= (LEFT | RIGHT);
  }

  // detect a right turn
  if (((prev & RIGHT) && !(curr & RIGHT)) &&
      (!(prev & LEFT) && !(curr & LEFT)) && !(curr & CENTERED)) {
    uart_puts("R\n");
    motors_rotate_pos();
    while (!(linesensors_get() & LEFT_TILTED))
      ;
  }
  // detect a left turn
  else if (((prev & LEFT) && !(curr & LEFT)) &&
           (!(prev & RIGHT) && !(curr & RIGHT)) && !(curr & CENTERED)) {
    uart_puts("L\n");
    motors_rotate_neg();
    while (!(linesensors_get() & RIGHT_TILTED))
      ;
  }
  // detect a right and forward turn
  else if (((prev & LEFT) && !(curr & LEFT)) &&
           (!(prev & RIGHT) && !(curr & RIGHT)) && (curr & CENTERED)) {
    uart_puts("RF\n");
    motors_rotate_neg();
    while ((linesensors_get() & ON_TRACK))
      ;
    while (!(linesensors_get() & ON_TRACK))
      ;
  }
  // detect a left and forward turn
  else if (((prev & RIGHT) && !(curr & RIGHT)) &&
           (!(prev & LEFT) && !(curr & LEFT)) && (curr & CENTERED)) {
    uart_puts("LF\n");
    motors_rotate_pos();
    while ((linesensors_get() & ON_TRACK))
      ;
    while (!(linesensors_get() & ON_TRACK))
      ;
  }
  // detect a T turn
  else if (((prev & RIGHT) && !(curr & RIGHT)) &&
           ((prev & LEFT) && !(curr & LEFT)) && !(curr & CENTERED)) {
    uart_puts("LR\n");
    motors_rotate_pos();
    while (!(linesensors_get() & ON_TRACK))
      ;
  }
  // detect a cross turn
  else if (((prev & RIGHT) && !(curr & RIGHT)) &&
           ((prev & LEFT) && !(curr & LEFT)) && (curr & CENTERED)) {
    uart_puts("LFR\n");
    motors_rotate_pos();
    while ((linesensors_get() & ON_TRACK))
      ;
    while (!(linesensors_get() & ON_TRACK))
      ;
  }
  // move if the right and left sensors are on
  else if (curr & (LEFT | RIGHT)) {
    motors_forward();
  }
  // move forward
  else if (curr & CENTERED) {

    if (curr & RIGHT_TILTED) {

      if (curr & ON_TRACK) {
        motors_tilt_neg();
      } else {
        motors_lean_neg();
      }

    } else if (curr & LEFT_TILTED) {

      if (curr & ON_TRACK) {
        motors_tilt_pos();
      } else {
        motors_lean_pos();
      }

    } else {
      motors_forward();
    }
  } else {
    motors_forward();
  }
}
