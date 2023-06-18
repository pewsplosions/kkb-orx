#include "orx.h"
#include "player/player.h"
#include "water/water.h"
#include "clouds/clouds.h"
#include "bird/bird.h"
#include "flock/flock.h"

#ifdef __orxMSVC__
__declspec(dllexport) unsigned long NvOptimusEnablement        = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif // __orxMSVC__

orxSTATUS orxFASTCALL PhysicsEventHandler(const orxEVENT *_pstEvent)
{
  if (_pstEvent->eID == orxPHYSICS_EVENT_CONTACT_ADD) {
    orxOBJECT *pstRecipientObject, *pstSenderObject;

    pstSenderObject = orxOBJECT(_pstEvent->hSender);
    pstRecipientObject = orxOBJECT(_pstEvent->hRecipient);

    orxSTRING senderObjectName = (orxSTRING)orxObject_GetName(pstSenderObject);
    orxSTRING recipientObjectName = (orxSTRING)orxObject_GetName(pstRecipientObject);

    if ((orxString_Compare(senderObjectName, "Plane") == 0 && orxString_Compare(recipientObjectName, "Bird") == 0) ||
        (orxString_Compare(senderObjectName, "Bird") == 0 && orxString_Compare(recipientObjectName, "Plane") == 0)) {
          Flock.stop();
          Player.crash();
          orxOBJECT* bird = (orxString_Compare(senderObjectName, "Bird") == 0) ? pstSenderObject : pstRecipientObject;
          orxObject_SetTargetAnim(bird, "BirdCrash");
          // TODO figure out bird and crash it too.
    }
  }
  return orxSTATUS_SUCCESS;
}

orxSTATUS orxFASTCALL AnimEventHandler(const orxEVENT *_pstEvent){
  orxANIM_EVENT_PAYLOAD *pstPayload;
  pstPayload = (orxANIM_EVENT_PAYLOAD *)_pstEvent->pstPayload;

  orxOBJECT* pstSenderObject = orxOBJECT(_pstEvent->hSender);
  
  switch(_pstEvent->eID){
    case orxANIM_EVENT_CUSTOM_EVENT: {
      orxSTRING name = pstPayload->stCustom.zName;
      if (orxString_Compare(name, "PlaneCrashed") == 0) {
        Player.destroy();
      }

      if (orxString_Compare(name, "BirdCrashed") == 0) {
        orxObject_SetLifeTime(pstSenderObject, 0.0); // TODO probably need efficient way to get Bird_T and destroy it.
      }

      break;
    }
  }
 
  return orxSTATUS_SUCCESS;
}

void orxFASTCALL Update(const orxCLOCK_INFO *_pstClockInfo, void *_pContext) {
  if (orxInput_IsActive("Quit")) orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
  if (orxInput_HasBeenActivated("RestartGame")) {
    Flock.obliterate();
    Player.destroy();
    Player.init();
    Player.create("Plane");
    Flock.init();
    Flock.create("Flock", 0);
    Flock.start();
  }

  Player.update(_pstClockInfo);
  Water.update(_pstClockInfo);
  Clouds.update(_pstClockInfo);
  Bird.update(_pstClockInfo);
  Flock.update(_pstClockInfo);
}

orxSTATUS orxFASTCALL Init() {
  orxClock_Register(orxClock_Get(orxCLOCK_KZ_CORE), Update, orxNULL, orxMODULE_ID_MAIN, orxCLOCK_PRIORITY_NORMAL);
  orxMath_InitRandom((orxS32)orxSystem_GetRealTime());

  orxEvent_AddHandler(orxEVENT_TYPE_PHYSICS, PhysicsEventHandler);
  orxEvent_AddHandler(orxEVENT_TYPE_ANIM, AnimEventHandler);

  orxViewport_CreateFromConfig("MainViewport");
  orxObject_CreateFromConfig("Scene");

  Player.init();
  Player.create("Plane");

  Water.init();
  Water.setup_scroll();

  Clouds.init();
  Clouds.setup_scroll();

  Bird.init();

  Flock.init();
  Flock.create("Flock", 0);
  return orxSTATUS_SUCCESS;
}

orxSTATUS orxFASTCALL Run() { return orxSTATUS_SUCCESS; }

void orxFASTCALL Exit() { }

orxSTATUS orxFASTCALL Bootstrap() {
  orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../src", orxFALSE);
  orxConfig_Load("gamed.ini"); // TODO figure out how to properly get this based on build type.

  return orxSTATUS_FAILURE; // Failure is to stop ORX from attempting to load defaults.
}

int main(int argc, char **argv) {
  orxConfig_SetBootstrap(Bootstrap);
  orx_Execute(argc, argv, Init, Run, Exit);
  return EXIT_SUCCESS;
}
