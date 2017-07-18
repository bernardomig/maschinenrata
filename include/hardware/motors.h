/**
 * @file motors.h
 * @brief This controls the motors of the robot.
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "config.h"

#include <avr/io.h>

typedef struct _motor_t_
{
  uint8_t speed; /**< the speed of the motor (can be a number between 0-255) */
  uint8_t direction; /**< the direction of rotation of the motor. a zero is cw,
                          else if ccw */
} motor_t;

extern motor_t motor_left;

extern motor_t motor_right;

void
motors_init();

void
motors_update();

void
motors_forward();

void
motors_reverse();

void
motors_stop();

void
motors_lean_pos();

void
motors_lean_neg();

void
motors_rotate_pos();

void
motors_rotate_neg();

#endif