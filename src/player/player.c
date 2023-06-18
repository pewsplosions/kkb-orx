#include "player.h"

struct Player_T* players = NULL;

static orxFLOAT SCREEN_WIDTH;
static orxFLOAT SCREEN_HEIGHT;
static orxFLOAT V_OFFSET;
static orxFLOAT MAX_LEFT, MAX_RIGHT;

static orxFLOAT SPEED = 80;

static orxFLOAT STOP = 0;


int kkb_player_next_number() {
  int lowest_available = 0;
  // struct Player_T* next;
  // for (next = players; next != NULL; next = next->hh.next) {
  //   if (next->player_number == lowest_available) {
  //     lowest_available = lowest_available + 1;
  //   }
  // }
  // TODO make this work correctly later... don't really need it for now.
  return lowest_available + 1;
}

Player_T* kkb_player_blank() {
  return malloc(sizeof(Player_T));
}

Player_T* kkb_player_create(orxSTRING obj_id) {
  Player_T* player = kkb_player_blank();
  player->self = orxObject_CreateFromConfig(obj_id);
  orxVECTOR start_pos = { 0, V_OFFSET - 100, 0 };
  orxObject_SetPosition(player->self, &start_pos);
  player->player_number = kkb_player_next_number();
  player->force = 0;
  HASH_ADD_INT(players, player_number, player);
  return player;
}

void kkb_player_init() {
  orxConfig_Load("player/player.ini");
  orxDisplay_GetScreenSize(&SCREEN_WIDTH, &SCREEN_HEIGHT);
  V_OFFSET = SCREEN_HEIGHT / 2;
  MAX_LEFT = SCREEN_WIDTH / 2 * -1;
  MAX_RIGHT = SCREEN_WIDTH / 2;
  STOP = 0;
}

void kkb_player_update(const orxCLOCK_INFO* clock) {
  if (!STOP) {
    struct Player_T* player;
    for (player = players; player != NULL; player = player->hh.next) {
      if (orxInput_HasBeenActivated("MoveLeft") && player->force > -3) {
        player->force = player->force - 1;
      }

      if (orxInput_HasBeenActivated("MoveRight") && player->force < 3) {
        player->force = player->force + 1;
      }

      if (player->force < 0) {
        orxObject_SetTargetAnim(player->self, "PlaneLeft");
      } else if (player->force > 0) {
        orxObject_SetTargetAnim(player->self, "PlaneRight");
      } else {
        orxObject_SetTargetAnim(player->self, "PlaneIdle");
      }

      orxVECTOR pos;
      orxObject_GetPosition(player->self, &pos);
      pos.fX = pos.fX + (player->force * SPEED * clock->fDT);
      if (pos.fX > MAX_LEFT && pos.fX < MAX_RIGHT) orxObject_SetPosition(player->self, &pos);
    }
  }
}

void kkb_player_destroy() {
  struct Player_T* player, *tmp;
  HASH_ITER(hh, players, player, tmp) {
    HASH_DEL(players, player);
    orxObject_SetLifeTime(player->self, 0.0);
    free(player);
  }
}

void kkb_player_crash() {
  STOP = 1;
  struct Player_T* player;
  for (player = players; player != NULL; player = player->hh.next) {
    orxObject_SetTargetAnim(player->self, "PlaneCrash");
  }
}

const struct KKB_PLAYER Player = {
  .create = kkb_player_create,
  .init = kkb_player_init,
  .update = kkb_player_update,
  .destroy = kkb_player_destroy,
  .crash = kkb_player_crash
};
