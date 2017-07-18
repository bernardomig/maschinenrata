/**
 * @file path.h
 * @brief All path-related functions, to store and optimize the path
 */

#ifndef _PATH_H_
#define _PATH_H_

#include <stdint.h>

/**
 * @brief Enumeration with all the intersections possible.
 */
typedef enum _intersection_t_ {
  LEFT_TURN,
  RIGHT_TURN,
  LEFT_FORWARD_TURN,
  RIGHT_FORWARD_TURN,
  T_TURN,
  CROSS_TURN,
  BACK_TURN
} intersection_t;

typedef enum _direction_t_ {
  LEFT_DIR,
  FORWARD_DIR,
  RIGHT_DIR,
  BACK_DIR
} direction_t;

typedef struct _node_t_
{
  direction_t last_took;
  intersection_t type;
};

/**
 * @brief The size of the path, as max number of intersections.
 */
#define PATH_SIZE 20

#endif
