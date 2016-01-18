#include "SoulEngine.h"

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;

static float gravityRate = 0;
static float gravityMax = 0.5;
static float lastPosX;
static float lastPosY;
static VECTOR Velocity;
static float friction = 0.05;

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
  //pMyTransform->Position = NewVector(2, 2);

}

void Update()
{
  
  //Save position of the player before moving
  lastPosX = pMyTransform->Position.x;
  lastPosY = pMyTransform->Position.y;

  /************* Player Input ***************/

  // Jumping
  if (AEInputCheckCurr('W'))
  {
    Velocity.y += 0.05;
  }

  if (AEInputCheckCurr('S'))
  {
    Velocity.y -= 0.05;
  }

  //Left movement
  if (AEInputCheckCurr('A'))
  {
    Velocity.x -= 0.05;
  }

  //Right movement
  if (AEInputCheckCurr('D'))
  {
    Velocity.x += 0.05;
  }

  ////////////////////////////////////////////


  /**************** Gravity *****************/
  ////////////////////////////////////////////
  
  /*************** Collision ****************/
  ////////////////////////////////////////////
  
  /************ Normalize Velocity **********/
  ////////////////////////////////////////////

  /******* Apply Friction to Velocity *******/

    Velocity.x -= (friction * Velocity.x);
    Velocity.y -= (friction * Velocity.y);

  ////////////////////////////////////////////

  /******* Apply Velocity to Player *********/

  pMyTransform->Position.x += Velocity.x;
  pMyTransform->Position.y += Velocity.y;

  ////////////////////////////////////////////



  

  
  //////Otherwise, if the player's position is greater than 0
  ////else if (pMyTransform->Position.y >= 0)
  ////{
  ////  //If the rate of gravity isn't at max
  ////  if (gravityRate < gravityMax)
  ////  {
  ////    //Increase the rate 
  ////    
  ////    gravityRate += 0.005;
  ////  }

  ////  //Apply gravity to the player
  ////  pMyTransform->Position.y -= gravityRate;
  ////}

  //If the player is grounded
  //if (pMyTransform->Position.y <= 0.0001)
  //{
  //  //Resets the rate of gravity
  //  gravityRate = 0;
  //}

  



}