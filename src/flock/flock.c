#include "flock.h"

static struct Flock_T* flocks = NULL;

static orxFLOAT SCREEN_WIDTH;
static orxFLOAT SCREEN_HEIGHT;
static orxFLOAT H_OFFSET;
static orxFLOAT V_OFFSET;

static orxFLOAT BIRD_SIZE;
static orxFLOAT BIRD_TEX_SIZE = 120.0; // TODO hardcoded cheating. :|
static orxFLOAT BIRD_GAP = 10.0;
static orxVECTOR BIRD_SCALER;
static orxFLOAT BIRDS_PER_ROW = 20.0;

static orxVECTOR START_POS;
static orxFLOAT SCREEN_OUTSIDE_PADDING = 100;

static orxFLOAT STOP = 0;


static int bird_id = 0;
static int flock_id = 0;

void kkb_flock_create_gap(Flock_T* flock, int row) {
  if (row == 0) {
    flock->missing = orxMath_Floor(orxMath_GetRandomFloat(0, BIRDS_PER_ROW - 1));
    flock->direction = orxMath_Floor(orxMath_GetRandomFloat(0, 99));

    if (flock->missing <= 2) {
      flock->direction = 99;
    }
    if (flock->missing >= BIRDS_PER_ROW - 2) {
      flock->direction = 0;
    }
  } else {
    if (flock->direction >= 50) {
      flock->missing  = flock->missing + 1;
      if (flock->missing >= BIRDS_PER_ROW - 2) {
        flock->direction = 0;
      }
    } else if (flock->direction < 50) {
      flock->missing = flock->missing - 1;
      if (flock->missing <= 2) {
        flock->direction = 99;
      }
    }
  }
}

void kkb_flock_create_row(Flock_T* flock, int row) {
  int img_size = BIRD_SIZE;
  int vert_gap = img_size + BIRD_GAP;

  kkb_flock_create_gap(flock, row);

  for (int i = 0; i < BIRDS_PER_ROW; ++i) {
    bird_id = bird_id + 1;
    if (flock->direction >= 50) {
      if (i != flock->missing && i != flock->missing + 1 && i != flock->missing + 2) {
        Bird_T* bird = Bird.create("Bird", bird_id);
        orxObject_SetOwner(bird->self, flock->self);
        orxObject_SetParent(bird->self, flock->self);
        orxVECTOR pos = { (i * img_size), row * -vert_gap, -0.1 };
        orxObject_SetPosition(bird->self, &pos);
        orxObject_SetScale(bird->self, &BIRD_SCALER);
        HASH_ADD(hh_flock, flock->birds, id, sizeof(int), bird);
      }
    } else {
      if (i != flock->missing && i != flock->missing - 1 && i != flock->missing - 2) {
        Bird_T* bird = Bird.create("Bird", bird_id);
        orxObject_SetOwner(bird->self, flock->self);
        orxObject_SetParent(bird->self, flock->self);
        orxVECTOR pos = { (i * img_size), row * -vert_gap, -0.1 };
        orxObject_SetPosition(bird->self, &pos);
        orxObject_SetScale(bird->self, &BIRD_SCALER);
        HASH_ADD(hh_flock, flock->birds, id, sizeof(int), bird);
      }
    }
  }
}

void kkb_flock_create_flock(Flock_T* flock) {
  int row = 0;
  while (row < flock->rowcount) {
    kkb_flock_create_row(flock, row);
    row = row + 1;
  }
  flock->resetpoint = flock->rowcount * BIRD_SIZE; // TODO update 120 to get bird height.
}

Flock_T* kkb_flock_blank() {
  return malloc(sizeof(Flock_T));
}

Flock_T* kkb_flock_create(orxSTRING obj_id, int id) {
  flock_id = flock_id + 1;
  Flock_T* flock = kkb_flock_blank();
  flock->self = orxObject_CreateFromConfig(obj_id);
  orxVECTOR vector2 = { H_OFFSET, V_OFFSET - SCREEN_OUTSIDE_PADDING, 0 };
  START_POS = vector2;
  orxObject_SetPosition(flock->self, &vector2);
  flock->id = id;
  flock->birds = NULL;
  flock->speed = 200;
  flock->rowcount = 20;
  kkb_flock_create_flock(flock);
  HASH_ADD_INT(flocks, id, flock);
  return flock;
}

orxOBJECT* score_object;
void kkb_flock_init() {
  orxConfig_Load("flock/flock.ini");
  orxDisplay_GetScreenSize(&SCREEN_WIDTH, &SCREEN_HEIGHT);
  H_OFFSET = SCREEN_WIDTH / 2 * -1;
  V_OFFSET = SCREEN_HEIGHT / 2 * -1;
  BIRD_SIZE = SCREEN_WIDTH / BIRDS_PER_ROW;
  orxVECTOR vec = { BIRD_SIZE / BIRD_TEX_SIZE, BIRD_SIZE / BIRD_TEX_SIZE, 1 };
  BIRD_SCALER = vec;
  score_object = orxObject_CreateFromConfig("ScoreObject");
}

void kkb_flock_destroy(Flock_T* flock) {
  struct Bird_T* next, *tmp;
  HASH_ITER(hh_flock, flock->birds, next, tmp) {
    HASH_DELETE(hh_flock, flock->birds, next);
    Bird.destroy(next);
  }
  HASH_DEL(flocks, flock);
  orxObject_Delete(flock->self);
  free(flock);
}

orxS16 score = 0;
void kkb_update_score(int increase)
{
  score = increase == 0 ? 0 : score + increase;

  orxCHAR formatted_score[6];
  orxString_NPrint(formatted_score, sizeof(formatted_score), "%d", score);

  orxObject_SetTextString(score_object, formatted_score);
}

void kkb_flock_update(const orxCLOCK_INFO* clock) {
  if (!STOP) {
    struct Flock_T* next, *tmp;
    HASH_ITER(hh, flocks, next, tmp) {
      orxVECTOR pos;
      orxObject_GetPosition(next->self, &pos);
      pos.fY = pos.fY + (next->speed * clock->fDT);
      if (pos.fY >= (V_OFFSET - next->resetpoint - BIRD_SIZE - BIRD_GAP - SCREEN_OUTSIDE_PADDING) * -1) {
        kkb_flock_destroy(next); // TODO probably be better to just reuse the flock and its birds
        kkb_flock_create("Flock", flock_id);
        kkb_update_score(100);
      } else {
        orxObject_SetPosition(next->self, &pos);
      }
    }
  }
}

void kkb_flock_stop() {
  STOP = 1;
}

void kkb_flock_start() {
  STOP = 0;
}

void kkb_flock_obliterate() {
  struct Flock_T* next, *tmp;
  HASH_ITER(hh, flocks, next, tmp) {
    kkb_flock_destroy(next);
  }
  kkb_update_score(0);
  orxObject_Delete(score_object);
}

const struct KKB_FLOCK Flock = {
  .create = kkb_flock_create,
  .init = kkb_flock_init,
  .update = kkb_flock_update,
  .stop = kkb_flock_stop,
  .start = kkb_flock_start,
  .obliterate = kkb_flock_obliterate
};
