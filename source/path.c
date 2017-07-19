#include "logic/path.h"

void
node_create(node_t* node, intersection_t intersection)
{
  node->type = intersection;
  node->last_took = 0;
}

bool
node_is_explored(node_t* node)
{
  uint8_t max_visited;
  switch (node->type) {
    case BACK_TURN:
      max_visited = 0;
      break;
    case LEFT_TURN:
    case RIGHT_TURN:
      max_visited = 1;
      break;
    case LEFT_FORWARD_TURN:
    case RIGHT_FORWARD_TURN:
    case T_TURN:
      max_visited = 2;
      break;
    case CROSS:
      max_visited = 3;
      break;
  }

  return (node->n_visited == max_visited);
}

direction_t
node_next_direction(node_t* node)
{
  uint8_t visited = node->n_visited;
  switch (node->type) {
    case LEFT_TURN:
      if (visited == 0)
        return LEFT_DIR;
      else
        return RIGHT_DIR;
    case RIGHT_TURN:
      if (visited == 0)
        return RIGHT_DIR;
      else
        return LEFT_DIR;
    case LEFT_FORWARD_TURN:
      if (visited == 0)
        return LEFT_DIR;
      else
        return FORWARD_DIR;
    case RIGHT_FORWARD_TURN:
      if (visited == 0)
        return FORWARD_DIR;
      else
        return RIGHT_DIR;
    case T_TURN:
      if (visited == 0)
        return LEFT_DIR;
      else
        return RIGHT_DIR;
    case CROSS_TURN:
      if (visited == 0)
        return LEFT_DIR;
      else if (visited == 1)
        return FORWARD_DIR;
      else
        return RIGHT_DIR;
    case BACK_TURN:
      return BACK_DIR;
  }
}
