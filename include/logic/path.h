/**
 * @file path.h
 * @brief All path-related functions, to store and optimize the path.
 */

#ifndef _PATH_H_
#define _PATH_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief All the possible directions of the robot.
 */
typedef enum _direction_t_ {
  LEFT_DIR,    /**< Left direction. */
  FORWARD_DIR, /**< Forward direction. */
  RIGHT_DIR,   /**< Right direction. */
  BACK_DIR     /**< Back direction. */
} direction_t;

/**
 * @brief Enumeration with all the intersections possible.
 */
typedef enum _intersection_t_ {
  LEFT_TURN,          /**< the left turn. */
  RIGHT_TURN,         /**< the right turn. */
  LEFT_FORWARD_TURN,  /**< the left and forward turn. */
  RIGHT_FORWARD_TURN, /**< the right and forward turn. */
  T_TURN,             /**< the left and right turn. */
  CROSS_TURN,         /**< the left, forward and right turn. */
  BACK_TURN           /**< the back turn. */
} intersection_t;

typedef struct _node_t_
{
  uint8_t n_visited; /**< Number of times the node was visited. By default its
                        left and then it will go conter-clockwise. */
  intersection_t
    type; /**< The type of the intersection seen for the first time when it is
             reached. */
} node_t;

/**
 * @brief Creates a new node in a node already in memory.
 * @param node The pointer to the node to be created.
 * @param intersection The intersection of the node.
 */
void
node_create(node_t* node, intersection_t intersection);

/**
 * @brief Checks if a node is fully explored.
 * @param node the node to be checked.
 * @return true if the node is fully explored, else false.
 */
bool
node_is_explored(node_t* node);

/**
 * @brief Updates the node and returns the next direction to turn.
 * @param node The node to be updated.
 * @return The next direction to turn.
 */
direction_t
node_next_direction(node_t* node);

/**
 * @brief The maximum number of nodes in the path.
 */
#define PATH_SIZE 20

/**
 * @brief Path of all the nodes visited by the robot.
 * If it reaches a dead-end, the robot will backtrack (go back by the same
 * way), until it reaches a node that was not fully explored.
 */
typedef struct _path_t_
{
  node_t nodes[PATH_SIZE]; /**< Array with all the nodes in the path. */
  uint8_t current;         /**< Pointer to the last node visited. */
  bool back;               /**< Boolean to know if the robot is backtracking. */
  bool done;               /**< True if the path is complete, meaning that is already
                              optimized and it's read-only. */
} path_t;

/**
 * @brief Creates a new path. Needs to be previously allocated.
 * @param path The path structure to be (re)created.
 */
void
path_create(path_t* path);

/**
 * @brief Updates the path when enconters a intersection.
 * This intersection can be new or can be resulted because of the
 * backtracking. Ether way, the right direction to take will be returned.
 * @param path Pointer to the path that will be added.
 * @param intersection The intersection that will be added.
 * @return The direction to take.
 */
direction_t
path_next(path_t* path, intersection_t intersection);

#endif
