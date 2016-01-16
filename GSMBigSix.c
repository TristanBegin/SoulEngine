#include "SoulEngine.h"
#include "GSMBigSix.h"

extern GAME * pTheGame;
//extern BEHAVIOR ** pBehaviorArray;
//extern int numBehaviors;


/********************************************************
  INITIALIZE LEVEL
*********************************************************/

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
        pMesh = (BEHAVIOR *)tempComp->pStruct;
        break;

      default:
        break;
      }

      tempComp = tempComp->nextComponent;
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

  //Drawing object
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);

  //Set Position of object
  AEGfxSetPosition(pMyTransform->Position.x, pMyTransform->Position.y);

  //Set texture for object
  AEGfxTextureSet(pSprite->pTexture, 0, 0);

  //Drawing the mesh (list of triangles)
  AEGfxSetTransparency(1.0);
  AEGfxMeshDraw(pMesh->pMeshLit, AE_GFX_MDM_TRIANGLES);

}