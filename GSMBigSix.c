#include "SoulEngine.h"
#include "GSMBigSix.h"
#include "Animation.h"

extern GAME * pTheGame;
//extern BEHAVIOR ** pBehaviorArray;
//extern int numBehaviors;


/********************************************************
  INITIALIZE LEVEL
*********************************************************/

void LoadLevel()
{
	int gridSize = pTheGame->pGameStats->GridSize/2;
	AEGfxVertexList * pLMesh;
	LEVEL * pTheLevel = pTheGame->pGameStats->pRunningLevel;
	UNIT * tempUnit = pTheLevel->nextUnit;
	while (tempUnit)
	{
		MESH * pMesh = ((MESH*)FindComponentStruct(tempUnit->pInitArchetype, Mesh));
		SPRITE * pSprite = ((SPRITE*)FindComponentStruct(tempUnit->pInitArchetype, Sprite));

		if (pMesh->pMeshLit != NULL)
		{
			break;
		}
		// Informing the library that we're about to start adding triangles.
		AEGfxMeshStart();
		//This shape has 2 triangles.
		AEGfxTriAdd(
			-(pMesh->Size.x) * gridSize, -(pMesh->Size.y) * gridSize, 0xFFFFFFFF, 0.0f                 , 1 / pSprite->RowCol.y,
			(pMesh->Size.x) * gridSize, -(pMesh->Size.y) * gridSize, 0xFFFFFFFF,  1 / pSprite->RowCol.x, 1 / pSprite->RowCol.y,
			-(pMesh->Size.x) * gridSize, (pMesh->Size.y) * gridSize, 0xFFFFFFFF,  0.0f, 0.0f);
		AEGfxTriAdd(
			(pMesh->Size.x) * gridSize, -(pMesh->Size.y) * gridSize, 0xFFFFFFFF,  1 / pSprite->RowCol.x, 1 / pSprite->RowCol.y,
			(pMesh->Size.x) * gridSize, (pMesh->Size.y) * gridSize, 0xFFFFFFFF,   1 / pSprite->RowCol.x, 0.0f,
			-(pMesh->Size.x) * gridSize, (pMesh->Size.y) * gridSize, 0xFFFFFFFF,  0.0f, 0.0f);


		pLMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(pLMesh, "Failed to create default mesh");
		pMesh->pMeshLit = pLMesh;


		tempUnit = tempUnit->nextUnit;
	}

}

void InitializeLevel()
{
  LEVEL * pTheLevel = pTheGame->pGameStats->pRunningLevel;
  UNIT * temp = pTheLevel->nextUnit;
  //numBehaviors = 0;
  while (temp)
  {
    InitializeUnit(temp);
    /*
    if (FindComponentStruct(temp->pArchetype, Behavior))
    {
      numBehaviors++;
    }
    */
    temp = temp->nextUnit;
  }
  /*
  if (numBehaviors > 0)
  {
    int i = 0;
    pBehaviorArray = (BEHAVIOR**)calloc(numBehaviors, sizeof(BEHAVIOR*));
    temp = pTheLevel->nextUnit;

    while (temp)
    {
      BEHAVIOR * pBehavior = FindComponentStruct(temp->pArchetype, Behavior);
      if (pBehavior)
      {
        pBehaviorArray[i] = pBehavior;
        i++;
      }
      temp = temp->nextUnit;
    }
  }
  */
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