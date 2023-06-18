#ifndef KKB_WATER_H
#define KKB_WATER_H

#include "orx.h"
#include "uthash.h"

typedef struct Water_T {
  int id;
  orxOBJECT* self;
  UT_hash_handle hh;
} Water_T;

struct KKB_WATER {
  Water_T* (*create)(orxSTRING, int);
  void (*init)(void);
  void (*setup_scroll)(void);
  void (*update)(const orxCLOCK_INFO*);
};

extern const struct KKB_WATER Water;

#endif
