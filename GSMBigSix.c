#include "SoulEngine.h"
#include "GSMBigSix.h"
#include "Animation.h"
#include "Collision.h"

extern GAME * pTheGame;
//extern BEHAVIOR ** pBehaviorArray;
//extern int numBehaviors;

//Prototype the UpdatePhysics function
void UpdatePhysics(PHYSICS * pPhysics);



void LoadLevel()
{
	float gridSize = pTheGame->pGameStats->GridSize/2;
	AEGfxVertexList * pLMesh;
	LEVEL * pTheLevel = pTheGame->pGameStats->pRunningLevel;
	UNIT * tempUnit = pTheLevel->nextUnit;
	while (tempUnit)
	{
		MESH * pMesh = ((MESH*)FindComponentStruct(tempUnit->pInitArchetype, Mesh));
		SPRITE * pSprite = ((SPRITE*)FindComponentStruct(tempUnit->pInitArchetype, Sprite));

    pSprite->pTexture = AEGfxTextureLoad(pSprite->TextureFile);

    if (pMesh->pMeshLit == NULL)
    {
      // Informing the library that we're about to start adding triangles.
      AEGfxMeshStart();
      //This shape has 2 triangles.
      AEGfxTriAdd(
        -(pMesh->Size.x) * gridSize, -(pMesh->Size.y) * gridSize, 0xFFFFFFFF, 0.0f, 1 / pSprite->RowCol.y,
        (pMesh->Size.x) * gridSize, -(pMesh->Size.y) * gridSize, 0xFFFFFFFF, 1 / pSprite->RowCol.x, 1 / pSprite->RowCol.y,
        -(pMesh->Size.x) * gridSize, (pMesh->Size.y) * gridSize, 0xFFFFFFFF, 0.0f, 0.0f);
      AEGfxTriAdd(
        (pMesh->Size.x) * gridSize, -(pMesh->Size.y) * gridSize, 0xFFFFFFFF, 1 / pSprite->RowCol.x, 1 / pSprite->RowCol.y,
        (pMesh->Size.x) * gridSize, (pMesh->Size.y) * gridSize, 0xFFFFFFFF, 1 / pSprite->RowCol.x, 0.0f,
        -(pMesh->Size.x) * gridSize, (pMesh->Size.y) * gridSize, 0xFFFFFFFF, 0.0f, 0.0f);

      pLMesh = AEGfxMeshEnd();
      AE_ASSERT_MESG(pLMesh, "Failed to create default mesh");
      pMesh->pMeshLit = pLMesh;
    }
		tempUnit = tempUnit->nextUnit;
	}

}

/********************************************************
  INITIALIZE LEVEL
*********************************************************/
void InitializeLevel()
{
  LEVEL * pTheLevel = pTheGame->pGameStats->pRunningLevel;
  UNIT * temp = pTheLevel->nextUnit;

  while (temp)
  {
    InitializeUnit(temp);
    temp = temp->nextUnit;
  }
}


/********************************************************
  UPDATE LEVEL
*********************************************************/

void UpdateLevel()
{
  LEVEL * pTheLevel = pTheGame->pGameStats->pRunningLevel;
  UNIT * tempUnit = pTheLevel->nextUnit;
  while (tempUnit)
  {
    COMPONENT * tempComp = tempUnit->pArchetype->nextComponent;
    SPRITE * pSprite = NULL;
    MESH * pMesh = NULL;
    BEHAVIOR * pBehavior = NULL;
    PHYSICS * pPhysics = NULL;
    COLLIDER * pCollider = NULL;
    SOUND * pSound = NULL;

    while (tempComp)
    {
      switch (tempComp->Type)
      {
      case Sprite:
        pSprite = (SPRITE *)tempComp->pStruct;
        break;

      case Mesh:
        pMesh = (MESH *)tempComp->pStruct;
        break;

      case Behavior:
        pBehavior = (BEHAVIOR *)tempComp->pStruct;
        break;

      case Physics:
        pPhysics = (PHYSICS *)tempComp->pStruct;
        break;

      case Collider:
        pCollider = (COLLIDER *)tempComp->pStruct;
        break;

      case Sound:
        pSound = (SOUND *)tempComp->pStruct;
        break;

      default:
        break;
      }

      tempComp = tempComp->nextComponent;
    }
	  
    if (pSprite->Animated == TRUE)
	  {
	  	Animate(pSprite);
	  }
    
    if (pBehavior)
    {
      pBehavior->BehaviorScript(pBehavior, "Update");
    }

    if (pPhysics)
    {
      UpdatePhysics(pPhysics);
    }

	if (pCollider)
	{
		//Currently assuming offset is the center of the collider, though I realize this is unlikely
		//Needs to be tweaked and tested.
		UpdateCollision(pCollider);
	}

    tempUnit = tempUnit->nextUnit;


  }
}


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
    collidingY = 1;
  }
  else
  {
    collidingY = 0;
  }

  /************* Player Input ***************/

  // Jumping
  if (AEInputCheckCurr('W') && pMyTransform->Position.y <= 0)
  {
    pVelocity->y = 0.7;
  }

  if (AEInputCheckCurr('S'))
  {
    pVelocity->y -= 0.05;
  }

  //Left movement
  if (AEInputCheckCurr('A') && pVelocity->x > -maxSpeed)
  {
    pVelocity->x -= 0.05;
  }

  //Right movement
  if (AEInputCheckCurr('D') && pVelocity->x < maxSpeed)
  {
    pVelocity->x += 0.05;
  }

  ////////////////////////////////////////////


  /**************** Gravity *****************/

  if (pVelocity->y < gravityMax)
  {
    pVelocity->y -= gravityRate;
  }

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


/********************************************************
  DRAW LEVEL
*********************************************************/
void DrawLevel()
{
  LEVEL * pTheLevel = pTheGame->pGameStats->pRunningLevel;
  UNIT * tempUnit = pTheLevel->nextUnit;
  while (tempUnit)
  {
    COMPONENT * tempComp = tempUnit->pArchetype->nextComponent;
    SPRITE * pSprite = NULL;
    MESH * pMesh = NULL;
    while (tempComp)
    {
      switch (tempComp->Type)
      {
        case Sprite:
          pSprite = (SPRITE *)tempComp->pStruct;
          break;

        case Mesh:
          pMesh = (MESH *) tempComp->pStruct;
          break;

        default:
          break;
      }

      tempComp = tempComp->nextComponent;
    }
      
    if (pMesh && pSprite)
    {
      DrawObject(pMesh, pSprite);
    }
    
    tempUnit = tempUnit->nextUnit;
    
  }
}

void DrawObject(MESH * pMesh, SPRITE * pSprite)
{
  TRANSFORM * pMyTransform = pSprite->pArchetype->pUnit->pTransform;
  float gridMultiplier = pTheGame->pGameStats->GridSize;
  //Drawing object
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

  //Set Position of object
  AEGfxSetPosition(pMyTransform->Position.x * gridMultiplier, pMyTransform->Position.y * gridMultiplier);

  //Set texture for object
  AEGfxTextureSet(pSprite->pTexture, pSprite->Offset.x, pSprite->Offset.y);

  //Drawing the mesh (list of triangles)
  AEGfxSetTransparency(1.0);
  AEGfxMeshDraw(pMesh->pMeshLit, AE_GFX_MDM_TRIANGLES);

}