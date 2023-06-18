#include "bird.h"

static struct Bird_T* birds = NULL;

static orxFLOAT SCREEN_WIDTH;
static orxFLOAT SCREEN_HEIGHT;

Bird_T* kkb_bird_blank() {
  return malloc(sizeof(Bird_T));
}

Bird_T* kkb_bird_create(orxSTRING obj_id, int id) {
  Bird_T* bird = kkb_bird_blank();
  bird->self = orxObject_CreateFromConfig(obj_id);
  bird->id = id;
  HASH_ADD(hh_internal, birds, id, sizeof(int), bird);
  return bird;
}

void kkb_bird_init() {
  orxConfig_Load("bird/bird.ini");
  orxDisplay_GetScreenSize(&SCREEN_WIDTH, &SCREEN_HEIGHT);
}

void kkb_bird_update(const orxCLOCK_INFO* clock) {
  struct Bird_T* next;
  for (next = birds; next != NULL; next = next->hh_internal.next) {
    // TODO ???
  }
}

void kkb_bird_destroy(Bird_T* bird) {
  HASH_DELETE(hh_internal, birds, bird);
  // if (bird->self && bird->self != orxNULL) orxObject_Delete(bird->self);
  free(bird);
}

void kkb_bird_crash(Bird_T* bird) {
  orxObject_SetTargetAnim(bird->self, "BirdCrash");
}

const struct KKB_BIRD Bird = {
  .create = kkb_bird_create,
  .init = kkb_bird_init,
  .update = kkb_bird_update,
  .destroy = kkb_bird_destroy,
  .crash = kkb_bird_crash
};
