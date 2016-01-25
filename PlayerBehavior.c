#include "SoulEngine.h"
#include <math.h>

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;
static PHYSICS * pMyPhysics;
static BEHAVIOR * pMyBehavior;
static COLLIDER * pMyCollider;

static float gravityRate = 0;
static float gravityMax = 0.5;
static float lastPosX;
static float lastPosY;
static VECTOR Velocity;
static float friction = 0.05;
static int collidingY = 0;
static float maxSpeed = 10;
static float mineVar = 0;

void Start();
void Update();

void PlayerBehavior(BEHAVIOR * Owner, char * Trigger)
{
  pMyBehavior = Owner;
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;
  pMyPhysics = FindComponentStruct(pMyArchetype, Physics);
  pMyCollider = FindComponentStruct(pMyArchetype, Collider);

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
  AddVar(Bool, "FacingRight", pMyBehavior);
}

static void Update()
{
  BOOL * FacingRight = (BOOL*)GetVar("FacingRight", pMyBehavior);

  if (AEInputCheckTriggered(' '))
  {
    UNIT * pBullet = InstantiateUnit(pMyLevel, "Bullet", pMyTransform->Position);
    PHYSICS * pBulletPhysics = ((PHYSICS*)FindComponentStruct(pBullet->pArchetype, Physics));
    if (*FacingRight)
    {
      pBulletPhysics->Velocity.x = 15;
    }
    else
    {
      pBulletPhysics->Velocity.x = -15;
    }
    OutputDebugString("Bullet");
  }
  
  /************* Player Input ***************/

  // Jumping
  if (AEInputCheckTriggered('W') && (pMyCollider->Grounded 
    || pMyCollider->RightBlocked || pMyCollider->LeftBlocked))
  {
    pMyPhysics->Velocity.y = 20;

    if (pMyCollider->Grounded == False)
    {
      if (pMyCollider->RightBlocked) pMyPhysics->Velocity.x = -15;
      if (pMyCollider->LeftBlocked) pMyPhysics->Velocity.x = 15;
    }
  }

  if (AEInputCheckCurr('S'))
  {
    pMyPhysics->Velocity.y -= 0.5;
  }

  //Left movement
  if (AEInputCheckCurr('A') && pMyPhysics->Velocity.x > -maxSpeed)
  {
    *FacingRight = False;
    pMyPhysics->Velocity.x -= 1.0;
  }

  //Right movement
  if (AEInputCheckCurr('D') && pMyPhysics->Velocity.x < maxSpeed)
  {
    *FacingRight = True;
    pMyPhysics->Velocity.x += 1.0;
  }


  //Checking for collision with a platform at y = 0
  //if (pMyTransform->Position.y <= 0)
  //{
  //  collidingY = 1;
  //}
  //else
  //{
  //  collidingY = 0;
  //}
  //
  ///************* Player Input ***************/
  //
  // Jumping
  //if (AEInputCheckCurr('W') && pMyTransform->Position.y <= 0)
  //{
  //  Velocity.y = 0.7;
  //}
  //
  //if (AEInputCheckCurr('S'))
  //{
  //  Velocity.y -= 0.05;
  //}
  //
  ////Left movement
  //if (AEInputCheckCurr('A') && Velocity.x > -maxSpeed)
  //{
  //  Velocity.x -= 0.05;
  //}
  //
  ////Right movement
  //if (AEInputCheckCurr('D') && Velocity.x < maxSpeed)
  //{
  //  Velocity.x += 0.05;
  //}
  //
  //////////////////////////////////////////////
  //
  //
  ///**************** Gravity *****************/
  //
  //Velocity.y -= 0.025;
  //
  //////////////////////////////////////////////
  //
  //
  ///******* Apply Friction to Velocity *******/
  //
  //  Velocity.x -= (friction * Velocity.x);
  //  Velocity.y -= (friction * Velocity.y);
  //
  //////////////////////////////////////////////
  //
  //
  ///*************** Collision ****************/
  //
  //  if (collidingY)
  //  {
  //    if (Velocity.y < 0)
  //    {
  //      Velocity.y = 0;
  //    }
  //  }
  //
  //////////////////////////////////////////////
  //
  //
  ///************ Normalize Velocity **********/
  //
  ////if (Velocity.x != 0 && Velocity.y != 0)
  ////{
  //  //mineVar = sqrt( (Velocity.x * Velocity.x) + (Velocity.y * Velocity.y) );
  //  
  //  //Velocity.x = (Velocity.x) / mineVar;
  //  //Velocity.y = (Velocity.y) / mineVar;
  ////}
  //
  //////////////////////////////////////////////
  //
  //
  ///******* Apply Velocity to Player *********/
  //
  //pMyTransform->Position.x += Velocity.x;
  //pMyTransform->Position.y += Velocity.y;
  //
  //////////////////////////////////////////////
}