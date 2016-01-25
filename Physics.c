#include "SoulEngine.h"
#include "Physics.h"
#include <math.h>
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
  pAcceleration->y = -gravityRate;
  //}

  ////////////////////////////////////////////


  /******* Apply Friction to Velocity *******/

  pVelocity->x -= (friction * pVelocity->x);
  //pVelocity->y -= (friction * pVelocity->y);

  ////////////////////////////////////////////


  /******* Account for Collision ************/


  if (pCollider->Grounded)
  {
    if (pAcceleration->y < 0) pAcceleration->y = 0;
    if (pCollider->Grounded < 0.02f)
    {
      
      if (pVelocity->y < 0) pVelocity->y = 0;
    }
    else
    {
      if (pVelocity->y < 0) pVelocity->y = (pow((pCollider->Grounded - 0.05) * 20, 2)) / 2;
    }
  }

  if (pCollider->LeftBlocked)
  {
    if (pAcceleration->x < 0) pAcceleration->x = 0;
    if (pCollider->LeftBlocked < 0.1f)
    {

      if (pVelocity->x < 0) pVelocity->x = 0;
    }
    else
    {
      if (pVelocity->x < 0) pVelocity->x = (pow(pCollider->LeftBlocked * 15, 2)) / 2;
    }
  }

  if (pCollider->TopBlocked)
  {
    if (pAcceleration->y > 0) pAcceleration->y = 0;
    if (pCollider->TopBlocked < 0.1f)
    {

      if (pVelocity->y > 0) pVelocity->y = 0;
    }
    else
    {
      if (pVelocity->y > 0) pVelocity->y = -(pow(pCollider->TopBlocked * 15, 2)) / 2;
    }
  }

  if (pCollider->RightBlocked)
  {
    if (pAcceleration->x > 0) pAcceleration->x = 0;
    if (pCollider->RightBlocked < 0.1f)
    {

      if (pVelocity->x > 0) pVelocity->x = 0;
    }
    else
    {
      if (pVelocity->x > 0) pVelocity->x = -(pow(pCollider->RightBlocked * 15, 2)) / 2;
    }
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

  pVelocity->x += pAcceleration->x * frameTime;
  pVelocity->y += pAcceleration->y * frameTime;
  if (pVelocity->y > maxSpeed)
  {
    pVelocity->y = maxSpeed;
  }
  else if (pVelocity->y < -maxSpeed)
  {
    pVelocity->y = -maxSpeed;
  }
  if (pVelocity->x < 0.1 && pVelocity->x > -0.1)
  {
    pVelocity->x = 0;
  }

  pMyTransform->Position.x += pVelocity->x * frameTime;
  pMyTransform->Position.y += pVelocity->y * frameTime;

  ////////////////////////////////////////////
}