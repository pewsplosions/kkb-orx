#ifndef KKB_CLOUDS_H
#define KKB_CLOUDS_H

#include "orx.h"
#include "uthash.h"

typedef struct Clouds_T {
  int id;
  orxOBJECT* self;
  UT_hash_handle hh;
} Clouds_T;

struct KKB_CLOUDS {
  Clouds_T* (*create)(orxSTRING, int);
  void (*init)(void);
  void (*setup_scroll)(void);
  void (*update)(const orxCLOCK_INFO*);
};

extern const struct KKB_CLOUDS Clouds;

#endif