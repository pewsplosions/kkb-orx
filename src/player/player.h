#ifndef KKB_PLAYER_H
#define KKB_PLAYER_H

#include "orx.h"
#include "uthash.h"

typedef struct Player_T {
  int player_number;

  int force;

  orxOBJECT* self;
  UT_hash_handle hh;
} Player_T;

struct KKB_PLAYER {
  Player_T* (*create)(orxSTRING);
  void (*init)(void);
  void (*update)(const orxCLOCK_INFO*);
  void (*destroy)(void);
  void (*crash)(void);
};

extern const struct KKB_PLAYER Player;

#endif