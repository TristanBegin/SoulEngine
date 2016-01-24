#include "SoulEngine.h"
#include "Physics.h"

void UpdatePhysics(PHYSICS * pPhysics, COLLIDER *pCollider)
{
  UNIT * pOwner = pPhysics->pArchetype->pUnit;
  TRANSFORM * pMyTransform = pOwner->pTransform;
  VECTOR * pVelocity = &(pPhysics->Velocity);
  VECTOR * pAcceleration = &(pPhysics->Acceleration);

  float gravityRate = pPhysics->Gravity;
  float gravityMax = 500;
  float friction = pPhysics->Friction;
  int collidingY = 0;
  float maxSpeed = pPhysics->MaxSpeed;
  double frameTime = AEFrameRateControllerGetFrameTime();


  if (pMyTransform->Position.y <= 0)
  {
    pCollider->Grounded = TRUE;
  }
  else
    pCollider->Grounded = FALSE;


  /**************** Gravity *****************/

  //if (pVelocity->y < gravityMax)
  //{
  pAcceleration->y -= gravityRate * frameTime;
  //}

  ////////////////////////////////////////////


  /******* Apply Friction to Velocity *******/

  pVelocity->x -= (friction * pVelocity->x);
  //pVelocity->y -= (friction * pVelocity->y);

  ////////////////////////////////////////////


  /******* Account for Collision ************/

  //if (pCollider->Grounded)
  //{
  //  pVelocity->y += -(pVelocity->y);
  //}

  if (pCollider->Grounded)
  {
    if (pAcceleration->y < 0)
    {
      pAcceleration->y = 0;
    }
  }

  if (pMyTransform->Position.y < 0)
  {
    pMyTransform->Position.y = 0;
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

  pVelocity->x += pAcceleration->x * frameTime;
  pVelocity->y += pAcceleration->y * frameTime;

  pMyTransform->Position.x += pVelocity->x * frameTime;
  pMyTransform->Position.y += pVelocity->y * frameTime;

  ////////////////////////////////////////////
}