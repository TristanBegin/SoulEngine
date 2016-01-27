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

static void Start();
static void Update();
static void OnGhostEnter(COLLISIONINFO * pCollisionInfo);

void EnemyBehavior(BEHAVIOR * Owner, char * Trigger, void * Data)
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

  if (Trigger == "OnGhostEnter")
  {
    OnGhostEnter(Data);
  }
}

static void Start()
{
  float * Health = AddVar(Float, "Health", pMyBehavior);
  *Health = 100;
  float * Speed = AddVar(Float, "Speed", pMyBehavior);
  *Speed = 5;
}

static void Update()
{
  float * Health = GetVar("Health", pMyBehavior);
  float * Speed = GetVar("Speed", pMyBehavior);

  if (*Health <= 0)
  {
    DestroyUnit(pMyUnit);
  }

  pMyMesh->Opacity = *Health / 100;

  if (pMyCollider->GhostEnter)
  {
    if (COLLIDED_OBJECT->Tag == BAD)
    {
      pMyPhysics->Velocity.y = 5;
      *Health -= 10;
      DestroyUnit(COLLIDED_OBJECT);
    }
    else if (COLLIDED_OBJECT->Tag == WEAPON)
    {
      pMyPhysics->Velocity.y = 5;
      *Health -= 10;
    }
  }

  if (!pMyCollider->RightGrounded)
  {
    *Speed = -5;
  }
  else if (!pMyCollider->LeftGrounded)
  {
    *Speed = 5;
  }
  if (pMyCollider->Grounded)
  {
    pMyPhysics->Velocity.x = *Speed;
  }
}

static void OnGhostEnter(COLLISIONINFO * CollisionInfo)
{

}