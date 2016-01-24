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
  //float * TimeTillDestroy = AddVar(Float, "TimeTillDestroy", pMyBehavior);
  //*TimeUntilDestroy = 5;

}

void Update()
{
  float * TimeTillDestroy = GetVar(Float, "TimeTillDestroy", pMyBehavior);
  *TimeTillDestroy -= AEFrameRateControllerGetFrameTime();
  if (*TimeTillDestroy < 0)
  {

  }

}