#include "SoulEngine.h"
#include "Physics.h"

void UpdatePhysics(PHYSICS * pPhysics)
{
  UNIT * pOwner = pPhysics->pArchetype->pUnit;
  TRANSFORM * pMyTransform = pOwner->pTransform;
  VECTOR * pVelocity = &(pPhysics->Velocity);

  float gravityRate = pPhysics->Gravity;
  float gravityMax = 0.5;
  float friction = pPhysics->Friction;
  int collidingY = 0;
  float maxSpeed = pPhysics->MaxSpeed;

  //Checking for collision with a platform at y = 0
  if (pMyTransform->Position.y <= 0)
  {
    //Fake collision.
    collidingY = 1;
  }
  else
  {
    collidingY = 0;
  }


  /**************** Gravity *****************/

  //if (pVelocity->y < gravityMax)
  //{
  pVelocity->y -= gravityRate;
  //}

  ////////////////////////////////////////////


  /******* Apply Friction to Velocity *******/

  pVelocity->x -= (friction * pVelocity->x);
  pVelocity->y -= (friction * pVelocity->y);

  ////////////////////////////////////////////


  /*************** Collision ****************/

  if (collidingY)
  {
    if (pVelocity->y < 0)
    {
      pVelocity->y = 0;
    }
  }

  ////////////////////////////////////////////


  /************ Normalize Velocity **********/

  //if (Velocity.x != 0 && Velocity.y != 0)
  //{
  //mineVar = sqrt( (Velocity.x * Velocity.x) + (Velocity.y * Velocity.y) );

  //Velocity.x = (Velocity.x) / mineVar;
  //Velocity.y = (Velocity.y) / mineVar;
  //}

  ////////////////////////////////////////////


  /******* Apply Velocity to Player *********/

  pMyTransform->Position.x += pVelocity->x;
  pMyTransform->Position.y += pVelocity->y;

  ////////////////////////////////////////////
}