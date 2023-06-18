#ifndef KKB_FLOCK_H
#define KKB_FLOCK_H

#include "orx.h"
#include "uthash.h"
#include "bird/bird.h"

typedef struct Flock_T {
  int id;
  int speed; // TODO move to config
  int rowcount; // TODO move to config

  int missing;
  int direction;
  orxFLOAT resetpoint;

  Bird_T* birds;
  orxOBJECT* self;
  UT_hash_handle hh;
} Flock_T;

struct KKB_FLOCK {
  Flock_T* (*create)(orxSTRING, int);
  void (*init)(void);
  void (*update)(const orxCLOCK_INFO*);
  void (*stop)(void);
  void (*start)(void);
  void (*obliterate)(void);
};

extern const struct KKB_FLOCK Flock;

#endif
