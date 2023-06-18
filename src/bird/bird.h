#ifndef KKB_BIRD_H
#define KKB_BIRD_H

#include "orx.h"
#include "uthash.h"

typedef struct Bird_T {
  int id;
  orxOBJECT* self;
  UT_hash_handle hh_internal;
  UT_hash_handle hh_flock;
} Bird_T;

struct KKB_BIRD {
  Bird_T* (*create)(orxSTRING, int);
  void (*init)(void);
  void (*update)(const orxCLOCK_INFO*);
  void (*destroy)(Bird_T*);
  void (*crash)(Bird_T*);
};

extern const struct KKB_BIRD Bird;

#endif