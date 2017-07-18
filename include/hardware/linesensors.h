/**
 * @file linesensors.h
 * @brief Functions to control the linesensors.
 *
 * This contains all the functions to interface with the line sensors.
 * Usage example:
 * @code
 * line_t ls = linesensors_get();
 * if(ls & RIGHT) {
 *   // do something, like rotating right.
 * }
 * @endcode
 */

#ifndef _LINESENSORS_H_
#define _LINESENSORS_H_

#include <stdint.h>

/**
 * @brief Stores the raw sensor data.
 *
 * This structure stores the integer value of the sensors, in integers with 8bit
 * size.
 */
typedef struct _linesensors_t_
{
  uint8_t left;  /**< The left sensor output */
  uint8_t midl;  /**< The mid-left sensor output */
  uint8_t mid;   /**< The mid-mid sensor output */
  uint8_t midr;  /**< The mid-right sensor output */
  uint8_t right; /**< The right sensor output */
} linesensors_t;

/**
 * @brief Stores the boolean state of the line sensors.
 *
 * Each bit of this integer represents a sensor state. The most significant is
 * the right sensor and the least significant is the left sensor.
 */
typedef uint8_t line_t;

/**
 * @brief Stores recurrent sensor status.
 *
 * A enumeration of useful constants that can be used to check for recurrent
 * sensor states. Usually
 * is used in conjunction with the binary and (&) to check the state, for
 * example, @code
 * linesensor_get() & LEFT @endcode checks if the left sensor is triggered.
 */
typedef enum _line_status_ {
  ALL_OFF = 0b00000,      /**< All sensors are off. */
  ALL_ON = 0b11111,       /**< All sensors are on. */
  ON_TRACK = 0b00100,     /**< The mid sensor is on. */
  CENTERED = 0b01110,     /**< The mid sensors are on. */
  RIGHT_TILTED = 0b01000, /**< The mid-right sensor is on. */
  LEFT_TILTED = 0b00010,  /**< The mid-left sensor is on. */
  RIGHT = 0b10000,        /**< The right sensor is on. */
  LEFT = 0b00001          /**< The left sensor is on. */
} line_status;

/**
 * @brief Variable that stores the current value of the sensors.
 *
 * Global variable that stores the current value of the sensors. Only updates
 * when a call to #linesensors_update is done.
 */
extern linesensors_t linesensors;

/**
 * @brief Initializes the line sensors.
 *
 * Initializes the ADC of the m328p to read the analog values of the pins
 * PB0-PB4.
 * The following configurations were done:
 *  - resolution of the ADC: 8bit;
 *  - clock divider: CLK/256;
 */
void
linesensors_init();

/**
 * @brief Reads all the sensors and updates #linesensors.
 *
 * All the line sensors are read sequentially from the left to the right and the
 * raw value of each is stored in the global variable #linesensors.
 */
void
linesensors_update();

/**
 * @brief Sets the threadhold for each sensor.
 *
 * When the function #linesensors_get is called, this threshold level is used to
 * create the boolean output of each sensor.
 * Each sensor value is compared to the threshold, and is evaluated to true if
 * it is bigger than the threshold. There are 5 thresholds, each one for each
 * sensor, for better control.
 *
 * @param threshold The threshold value, that will be copied to a static
 * variable.
 */
void
linesensors_set_threshold(linesensors_t _threshold);

/**
 * @brief Get the boolean values of the sensors.
 * @return the boolean output.
 */
line_t
linesensors_get();

#endif
