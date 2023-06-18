#include "water.h"

static struct Water_T* waters = NULL;

static orxFLOAT SCREEN_WIDTH;
static orxFLOAT SCREEN_HEIGHT;
static orxFLOAT H_OFFSET;
static orxFLOAT V_OFFSET;

static orxFLOAT SCROLL_SPEED = 2.0;
static orxVECTOR START_POS;

Water_T* kkb_water_blank() {
  return malloc(sizeof(Water_T));
}

Water_T* kkb_water_create(orxSTRING obj_id, int id) {
  Water_T* water = kkb_water_blank();
  water->self = orxObject_CreateFromConfig(obj_id);
  water->id = id;
  HASH_ADD_INT(waters, id, water);
  return water;
}

void kkb_water_init() {
  orxConfig_Load("water/water.ini");
  orxDisplay_GetScreenSize(&SCREEN_WIDTH, &SCREEN_HEIGHT);
  H_OFFSET = SCREEN_WIDTH / 2 * -1;
  V_OFFSET = SCREEN_HEIGHT / 2 * -1;
}

void kkb_water_setup_scroll() {
  Water_T* water1 = kkb_water_create("Water", 1);
  Water_T* water2 = kkb_water_create("Water", 2);

  orxTEXTURE* tex = orxObject_GetWorkingTexture(water1->self);
  orxFLOAT width, height;
  orxTexture_GetSize(tex, &width, &height);

  orxVECTOR scaler = { SCREEN_WIDTH / width , SCREEN_HEIGHT / height, 1 };

  orxVECTOR vector1 = { H_OFFSET, V_OFFSET, 0.9 };
  orxObject_SetPosition(water1->self, &vector1);
  orxObject_SetScale(water1->self, &scaler);

  orxVECTOR vector2 = { H_OFFSET, V_OFFSET - SCREEN_HEIGHT, 0.9 };
  orxObject_SetPosition(water2->self, &vector2);
  orxObject_SetScale(water2->self, &scaler);
  START_POS = vector2;
}

void kkb_water_update(const orxCLOCK_INFO* clock) {
  struct Water_T* next;
  for (next = waters; next != NULL; next = next->hh.next) {
    orxVECTOR pos;
    orxObject_GetPosition(next->self, &pos);
    pos.fY = pos.fY + SCROLL_SPEED;
    if (pos.fY >= V_OFFSET * -1) {
      orxObject_SetPosition(next->self, &START_POS);
    } else {
      orxObject_SetPosition(next->self, &pos);
    }
  }
}

void kkb_water_destroy(Water_T* water) {
  HASH_DEL(waters, water);
}

const struct KKB_WATER Water = {
  .create = kkb_water_create,
  .init = kkb_water_init,
  .setup_scroll = kkb_water_setup_scroll,
  .update = kkb_water_update
};
