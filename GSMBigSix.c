#include "SoulEngine.h"
#include "GSMBigSix.h"
#include "Animation.h"
#include "Collision.h"
#include "Physics.h"

extern GAME * pTheGame;
//extern BEHAVIOR ** pBehaviorArray;
//extern int numBehaviors;





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

		
		IMAGE * ptemp = pSprite->pImage;
		while(ptemp)
		{
			ptemp->pTexture = AEGfxTextureLoad(pSprite->pImage->TextureFile);
			ptemp = ptemp->pNextImage;
		}
		
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
    if (tempUnit->pArchetype)
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
        case KSound:
          pSound = (SOUND *)tempComp->pStruct;
          break;
        }

        tempComp = tempComp->nextComponent;
      }

      if (pSprite->Animated == TRUE)
      {
        Animate(pSprite);
      }

      if (pCollider)
      {
        UpdateCollision(pCollider);

        if (pPhysics)
        {
          UpdatePhysics(pPhysics, pCollider);
        }
      }

      if (pBehavior)
      {
        pBehavior->BehaviorScript(pBehavior, "Update");
      }
    }

    tempUnit = tempUnit->nextUnit;

  }
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
    if (tempUnit->pArchetype)
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
          pMesh = (MESH *)tempComp->pStruct;
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

  SPRITE * ptemp = pSprite;
  while (ptemp->pImage)
  {
	  if ((ptemp->CurrentAnimation == ptemp->pImage->TextureFile) || (ptemp->pImage->pNextImage == NULL))
	  {
		  //Set texture for object
		  AEGfxTextureSet(ptemp->pImage->pTexture, pSprite->Offset.x, pSprite->Offset.y);
		  break;
	  }
	  ptemp->pImage = ptemp->pImage->pNextImage;
  }
  

  //Drawing the mesh (list of triangles)
  AEGfxSetTransparency(1.0);
  AEGfxMeshDraw(pMesh->pMeshLit, AE_GFX_MDM_TRIANGLES);

}