#include "SoulEngine.h"

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;
static BEHAVIOR * pMyBehavior;

void Start();
void Update();

void $BehaviorName$(BEHAVIOR * Owner, char * Trigger)
{
  pMyBehavior = Owner;
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;

  if (Trigger == "Start")
  {
    Start();
  }

  if (Trigger == "Update")
  {
    Update();
  }
}

void Start()
{
  float * TimeDestroy = AddVar(Float, "TimeDestroy", pMyBehavior);
  *TimeDestroy = 5;
}

void Update()
{
  float * TimeDestroy = GetVar(Float, "TimeDestroy", pMyBehavior);
  *TimeDestroy -= AEFrameRateControllerGetFrameTime();
  if (*TimeDestroy < 0)
  {
    DestroyUnit(pMyUnit);
  }

}