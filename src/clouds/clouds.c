#include "clouds.h"

static struct Clouds_T* clouds = NULL;

static orxFLOAT SCREEN_WIDTH;
static orxFLOAT SCREEN_HEIGHT;
static orxFLOAT H_OFFSET;
static orxFLOAT V_OFFSET;

static orxVECTOR START_POS;

Clouds_T* kkb_clouds_blank() {
  return malloc(sizeof(Clouds_T));
}

Clouds_T* kkb_clouds_create(orxSTRING obj_id, int id) {
  Clouds_T* cloudst = kkb_clouds_blank();
  cloudst->self = orxObject_CreateFromConfig(obj_id);
  cloudst->id = id;
  HASH_ADD_INT(clouds, id, cloudst);
  return cloudst;
}

void kkb_clouds_init() {
  orxConfig_Load("clouds/clouds.ini");
  orxDisplay_GetScreenSize(&SCREEN_WIDTH, &SCREEN_HEIGHT);
  H_OFFSET = SCREEN_WIDTH / 2 * -1;
  V_OFFSET = SCREEN_HEIGHT / 2 * -1;
}

void kkb_clouds_setup_scroll() {
  Clouds_T* clouds1 = kkb_clouds_create("Clouds", 1);
  Clouds_T* clouds2 = kkb_clouds_create("Clouds", 2);
  Clouds_T* clouds3 = kkb_clouds_create("Clouds", 3);

  orxTEXTURE* tex = orxObject_GetWorkingTexture(clouds1->self);
  orxFLOAT width, height;
  orxTexture_GetSize(tex, &width, &height);

  orxVECTOR scaler = { SCREEN_WIDTH / width , SCREEN_HEIGHT / height, 1};

  orxVECTOR vector1 = { H_OFFSET, V_OFFSET, 0.5 };
  orxObject_SetPosition(clouds1->self, &vector1);
  orxObject_SetScale(clouds1->self, &scaler);
  orxObject_SetAlpha(clouds1->self, 0.2);

  orxVECTOR vector2 = { H_OFFSET, V_OFFSET - SCREEN_HEIGHT, 0.3 };
  orxObject_SetPosition(clouds2->self, &vector2);
  orxObject_SetScale(clouds2->self, &scaler);
  orxObject_SetAlpha(clouds2->self, 0.4);

  orxVECTOR vector3 = { H_OFFSET, V_OFFSET - SCREEN_HEIGHT - SCREEN_HEIGHT / 2, 0.1 };
  orxObject_SetPosition(clouds3->self, &vector3);
  orxObject_SetScale(clouds3->self, &scaler);
  orxObject_SetAlpha(clouds3->self, 0.6);

  START_POS = vector2;
}

void kkb_clouds_update(const orxCLOCK_INFO* clock) {
  struct Clouds_T* next;
  for (next = clouds; next != NULL; next = next->hh.next) {
    orxVECTOR pos;
    orxObject_GetPosition(next->self, &pos);
    pos.fY = pos.fY + next->id * 1.5;
    if (pos.fY >= V_OFFSET * -1) {
      orxObject_SetPosition(next->self, &START_POS);
    } else {
      orxObject_SetPosition(next->self, &pos);
    }
  }
}

void kkb_clouds_destroy(Clouds_T* cloudst) {
  HASH_DEL(clouds, cloudst);
}

const struct KKB_CLOUDS Clouds = {
  .create = kkb_clouds_create,
  .init = kkb_clouds_init,
  .setup_scroll = kkb_clouds_setup_scroll,
  .update = kkb_clouds_update
};
