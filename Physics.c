#include "SoulEngine.h"
#include "Physics.h"

void UpdatePhysics(PHYSICS * pPhysics, COLLIDER *pCollider)
{
  UNIT * pOwner = pPhysics->pArchetype->pUnit;
  TRANSFORM * pMyTransform = pOwner->pTransform;
  VECTOR * pVelocity = &(pPhysics->Velocity);
  VECTOR * pAcceleration = &(pPhysics->Acceleration);

  float gravityRate = pPhysics->Gravity;
  float gravityMax = 50;
  float friction = pPhysics->Friction;
  int collidingY = 0;
  float maxSpeed = pPhysics->MaxSpeed;
  double frameTime = AEFrameRateControllerGetFrameTime();

  /**************** Gravity *****************/

  //if (pVelocity->y < gravityMax)
  //{
  pAcceleration->y -= gravityRate * (float)frameTime;
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

  if (pCollider->LeftBlocked)
  {
    pVelocity->y = 0;
    pAcceleration->y = 0;
  }
  //
  //if (pMyTransform->Position.y < 0)
  //{
  //  pMyTransform->Position.y = 0;
  //}

  ////////////////////////////////////////////


  /************ Normalize Velocity **********/

  ////////////////////////////////////////////


  /******* Apply Velocity to Player *********/

  pVelocity->x += pAcceleration->x * (float)frameTime;
  pVelocity->y += pAcceleration->y * (float)frameTime;

  pMyTransform->Position.x += pVelocity->x * (float)frameTime;
  pMyTransform->Position.y += pVelocity->y * (float)frameTime;

  ////////////////////////////////////////////
}