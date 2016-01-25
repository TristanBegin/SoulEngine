#include "SoulEngine.h"
#define COLLIDED_OBJECT pMyCollider->pCollidedWithGhost->pArchetype->pUnit
static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;
static PHYSICS * pMyPhysics;
static BEHAVIOR * pMyBehavior;
static COLLIDER * pMyCollider;
static MESH * pMyMesh;

void Start();
void Update();

void EnemyBehavior(BEHAVIOR * Owner, char * Trigger)
{
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;
  pMyBehavior = Owner;
  pMyPhysics = FindComponentStruct(pMyArchetype, Physics);
  pMyCollider = FindComponentStruct(pMyArchetype, Collider);
  pMyMesh = FindComponentStruct(pMyArchetype, Mesh);

  if (Trigger == "Start")
  {
    Start();
  }

  if (Trigger == "Update")
  {
    Update();
  }
}

static void Start()
{
  float * Health = AddVar(Float, "Health", pMyBehavior);
  *Health = 100;
}

static void Update()
{
  float * Health = GetVar("Health", pMyBehavior);

  if (*Health <= 0)
  {
    DestroyUnit(pMyUnit);
  }

  pMyMesh->Opacity = *Health / 100;

  if (pMyCollider->GhostEnter && COLLIDED_OBJECT->Tag == BAD)
  {
    pMyPhysics->Velocity.y = 5;
    *Health -= 10;
    DestroyUnit(COLLIDED_OBJECT);
  }

}