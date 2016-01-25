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
static SPRITE * pMySprite;

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
  pMySprite = FindComponentStruct(pMyArchetype, Sprite);

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
  char ** WalkAnim = AddVar(String, "WalkAnim", pMyBehavior);
  char ** IdleAnim = AddVar(String, "IdleAnim", pMyBehavior);
  char ** JumpAnim = AddVar(String, "JumpAnim", pMyBehavior);
  char ** FallAnim = AddVar(String, "FallAnim", pMyBehavior);
  *WalkAnim = "WitchWalking01.png";
  *IdleAnim = "WitchIdle.png";
  *JumpAnim = "WitchJumping.png";
  *FallAnim = "WitchFalling.png";
}

static void Update()
{
  BOOL * FacingRight = GetVar("FacingRight", pMyBehavior);
  char ** WalkAnim = GetVar("WalkAnim", pMyBehavior);
  char ** IdleAnim = GetVar("IdleAnim", pMyBehavior);
  char ** JumpAnim = GetVar("JumpAnim", pMyBehavior);
  char ** FallAnim = GetVar("FallAnim", pMyBehavior);

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

  if (pMyCollider->Grounded)
  {
    if (pMyPhysics->Velocity.x != 0)
      pMySprite->CurrentAnimation = *WalkAnim;
    else
      pMySprite->CurrentAnimation = *IdleAnim;
  }
  else
  {
    if (pMyPhysics->Velocity.y > 0)
      pMySprite->CurrentAnimation = *JumpAnim;
    else
      pMySprite->CurrentAnimation = *FallAnim;
  }
}

UpdateAnimation()
{

}