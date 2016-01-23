#include "SoulEngine.h"
#include <math.h>

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;
static PHYSICS * pMyPhysics;

static float gravityRate = 0;
static float gravityMax = 0.5;
static float lastPosX;
static float lastPosY;
static VECTOR Velocity;
static float friction = 0.05;
static int collidingY = 0;
static float maxSpeed = 0.3;
static float mineVar = 0;

void Start();
void Update();

void PlayerBehavior(BEHAVIOR * Owner, char * Trigger)
{
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;
  pMyPhysics = FindComponentStruct(pMyArchetype, Physics);

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
  //pMyTransform->Position = NewVector(2, 2);

}

static void Update()
{

  /************* Player Input ***************/

  // Jumping
  if (AEInputCheckCurr('W'))
  {
    pMyPhysics->Velocity.y = 10;
  }

  if (AEInputCheckCurr('S'))
  {
    pMyPhysics->Acceleration.y -= 1;
  }

  //Left movement
  if (AEInputCheckCurr('A') && pMyPhysics->Velocity.x > -maxSpeed)
  {
    pMyPhysics->Acceleration.x -= 10;
  }

  //Right movement
  if (AEInputCheckCurr('D') && pMyPhysics->Velocity.x < maxSpeed)
  {
    pMyPhysics->Acceleration.x += 10;
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