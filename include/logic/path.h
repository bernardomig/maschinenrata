/**
 * @file path.h
 * @brief All path-related functions, to store and optimize the path
 */

#ifndef _PATH_H_
#define _PATH_H_

#include <stdint.h>

/**
 * @brief The size of the path, in number of intersections.
 */
#define PATH_SIZE 20

/**
 * @brief Intersection possibilities
 */
typedef enum _turn_t_ {
  TURN_LEFT,     /**< Turns left. */
  TURN_STRAIGHT, /**< Goes straight. */
  TURN_RIGHT,    /**< Turns right. */
  TURN_BACK      /**< Goes back. */
} turn_t;

/**
 * @brief Hold all the path information. Can be further optimized with
 * #path_optimise.
 */
typedef struct _path_t_
{
  turn_t nodes[PATH_SIZE]; /**< The node array, containing all the
                               intersections that the robot passed on. */
  uint8_t current;         /**< Pointer containing the last intersection. Use
                             #path_append to append a new intersection. */
} path_t;

/**
 * @brief Initializes the path to zero.
 * @param path The path to be initialized.
 */
void
path_init(path_t* path);

/**
 * @brief Appends a turn to the path.
 * @param path The path.
 * @param trnn The turn to be appended.
 */
void
path_append(path_t* path, turn_t turn);

/**
 * @brief Optimizes the path to the one with less turns.
 * @param path The path to be optimized.
 */
void
path_optimise(path_t* path);

#endif
