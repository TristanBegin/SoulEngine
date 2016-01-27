#include "SoulEngine.h"
#include <math.h>
#include "VectorMath.h"

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
static CAMERA * pMyCamera;

static UNIT * pWeapon;
static SPRITE * pWeaponSprite;
static COLLIDER * pWeaponCollider;
static PHYSICS * pWeaponPhysics;
static TRANSFORM * pWeaponTransform;

static enum AttackDirection
{
  AtNone,
  AtRight,
  AtLeft,
  AtDown,
  AtUp
};

void Start();
void Update();

void PlayerBehavior(BEHAVIOR * Owner, char * Trigger, void * Data)
{
  pMyBehavior = Owner;
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;
  pMyCamera = pMyLevel->pCamera;
  pMyPhysics = FindComponentStruct(pMyArchetype, Physics);
  pMyCollider = FindComponentStruct(pMyArchetype, Collider);
  pMySprite = FindComponentStruct(pMyArchetype, Sprite);

  pWeapon = FindUnitByName(pMyLevel, "PlayerWeapon");
  pWeaponSprite = FindComponentStruct(pWeapon->pArchetype, Sprite);
  pWeaponCollider = FindComponentStruct(pWeapon->pArchetype, Collider);
  pWeaponPhysics = FindComponentStruct(pWeapon->pArchetype, Physics);
  pWeaponTransform = pWeapon->pTransform;

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
  AddVar(Int, "Attacking", pMyBehavior);
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
  int * Attacking = GetVar("Attacking", pMyBehavior);

  UpdateAnimation();
  UpdateCamera();
  
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


  float weaponDistanceX = pWeaponTransform->Position.x - pMyTransform->Position.x;
  float weaponDistanceY = pWeaponTransform->Position.y - pMyTransform->Position.y;

  if (*Attacking == AtNone)
  {
    pWeaponTransform->Position = pMyTransform->Position;
    pWeaponPhysics->Acceleration = NewVector(0, 0);

    if (AEInputCheckCurr(VK_RIGHT))
    {
      *Attacking = AtRight;
      pWeaponPhysics->Acceleration.x = 100;
    }
  }
  else if (*Attacking = AtRight)
  {
    pWeaponTransform->Position.y = pMyTransform->Position.y;

    if (AEInputCheckCurr(VK_LEFT) || weaponDistanceX > 10)
    {
      pWeaponPhysics->Acceleration.x = -100;
    }
    if (pWeaponCollider->GhostEnter && pWeaponCollider->pCollidedWithGhost->pArchetype->Tag == PLAYER)
    {
      *Attacking = False;
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
  if (AEInputCheckCurr('A') && pMyPhysics->Velocity.x > -pMyPhysics->MaxSpeed)
  {
    *FacingRight = False;
    pMyPhysics->Velocity.x -= 1.0;
  }

  //Right movement
  if (AEInputCheckCurr('D') && pMyPhysics->Velocity.x < pMyPhysics->MaxSpeed)
  {
    *FacingRight = True;
    pMyPhysics->Velocity.x += 1.0;
  }



}

UpdateAnimation()
{
  char ** WalkAnim = GetVar("WalkAnim", pMyBehavior);
  char ** IdleAnim = GetVar("IdleAnim", pMyBehavior);
  char ** JumpAnim = GetVar("JumpAnim", pMyBehavior);
  char ** FallAnim = GetVar("FallAnim", pMyBehavior);

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

UpdateCamera()
{
  float DistanceX = pMyTransform->Position.x - pMyCamera->Position.x;
  float DistanceY = pMyTransform->Position.y - pMyCamera->Position.y;
  if (fabs(DistanceX) > 0.2) pMyCamera->Position.x += DistanceX * 2 * deltaTime;
  if (fabs(DistanceY) > 0.2)pMyCamera->Position.y += DistanceY * 2 * deltaTime;
}