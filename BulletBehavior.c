#include "SoulEngine.h"

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;
static BEHAVIOR * pMyBehavior;
static COLLIDER * pMyCollider;

void Start();
void Update();

void BulletBehavior(BEHAVIOR * Owner, char * Trigger, void * Data)
{
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;
  pMyCollider = FindComponentStruct(pMyArchetype, Collider);
  pMyBehavior = FindComponentStruct(pMyArchetype, Behavior);

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
  *TimeDestroy = 4;
}

void Update()
{
  float * TimeDestroy = GetVar("TimeDestroy", pMyBehavior);
  *TimeDestroy -= deltaTime;
  if (*TimeDestroy <= 0 || 
    (pMyCollider->GhostStay && pMyCollider->pCollidedWithGhost->pArchetype->Tag == WALL))
  {
    DestroyUnit(pMyUnit);
  }
}