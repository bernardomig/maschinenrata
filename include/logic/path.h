#ifndef _PATH_H_
#define _PATH_H_

typedef enum _intersection_t_ {
  CROSS,
  T_TURN,
  FRONT_LEFT,
  FRONT_RIGHT
} intersection_t;

typedef struct _path_t_
{
  intersection_t nodes[20];
  uint_t current;
} path_t;

void
path_init(path_t* path);

void
path_append(path_t* path, intersection_t intersection);

void
path_optimise(path_t* path);

#endif
