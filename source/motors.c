#include "hardware/motors.h"

motor_t motor_left;
motor_t motor_right;

void
motors_init()
{
  // Set the drireciton of ports as Output
  DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);

  // Setup the TCO control register A
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
  TCCR0B |= (1 << CS00) | (1 << CS02);
}

void
motors_update()
{
  OCR0A = motor_left.speed;
  OCR0B = motor_right.speed;
  if (motor_left.direction) {
    PORTD |= (1 << PB7);
  } else {
    PORTD &= ~(1 << PB7);
  }
  if (motor_right.direction) {
    PORTD |= (1 << PB4);
  } else {
    PORTD &= ~(1 << PB4);
  }
}

void
motors_forward()
{
  motor_left.speed = MOTORS_LINEAR_SPEED;
  motor_left.direction = 0;
  motor_right.speed = MOTORS_LINEAR_SPEED;
  motor_right.direction = 0;
  motors_update();
}

void
motors_reverse()
{
  motor_left.speed = MOTORS_LINEAR_SPEED;
  motor_left.direction = 1;
  motor_right.speed = MOTORS_LINEAR_SPEED;
  motor_right.direction = 1;
  motors_update();
}

void
motors_lean_pos()
{
  motor_right.speed = MOTORS_ROTATE_SPEED;
  motor_right.direction = 0;
  motor_left.speed = MOTORS_ROTATE_SPEED * 0.3;
  motor_right.direction = 0;
  motors_update();
}

void
motors_lean_neg()
{
  motor_right.speed = MOTORS_ROTATE_SPEED * 0.3;
  motor_right.direction = 0;
  motor_left.speed = MOTORS_ROTATE_SPEED;
  motor_right.direction = 0;
  motors_update();
}

void
motors_tilt_pos()
{
  motor_right.speed = MOTORS_ROTATE_SPEED;
  motor_right.direction = 0;
  motor_left.speed = MOTORS_ROTATE_SPEED * 0.9;
  motor_right.direction = 0;
  motors_update();
}

void
motors_tilt_neg()
{
  motor_right.speed = MOTORS_ROTATE_SPEED * 0.9;
  motor_right.direction = 0;
  motor_left.speed = MOTORS_ROTATE_SPEED;
  motor_right.direction = 0;
  motors_update();
}

void
motors_stop()
{
  motor_left.speed = 0;
  motor_right.speed = 0;
  motors_update();
}

void
motors_rotate_pos()
{
  motor_left.speed = MOTORS_ROTATE_SPEED;
  motor_left.direction = 0;
  motor_right.speed = MOTORS_ROTATE_SPEED;
  motor_right.direction = 1;
  motors_update();
}

void
motors_rotate_neg()
{
  motor_left.speed = MOTORS_ROTATE_SPEED;
  motor_left.direction = 1;
  motor_right.speed = MOTORS_ROTATE_SPEED;
  motor_right.direction = 0;
  motors_update();
}
