#include "SoulEngine.h"
#include "GSMBigSix.h"

extern LEVEL * pRunningLevel;
extern BEHAVIOR ** pBehaviorArray = NULL;
extern int numBehaviors;


/********************************************************
  INITIALIZE LEVEL
*********************************************************/

void InitializeLevel()
{
  UNIT * temp = pRunningLevel->nextUnit;
  numBehaviors = 0;
  while (temp)
  {
    InitializeUnit(temp);

    if (FindComponentStruct(temp->pArchetype, Behavior))
    {
      numBehaviors++;
    }
    temp = temp->nextUnit;
  }

  if (numBehaviors > 0)
  {
    int i = 0;
    pBehaviorArray = (BEHAVIOR**)calloc(numBehaviors, sizeof(BEHAVIOR*));
    temp = pRunningLevel->nextUnit;

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
}




/********************************************************
  UPDATE LEVEL
*********************************************************/

void UpdateLevel()
{

}


/********************************************************
  DRAW LEVEL
*********************************************************/
void DrawLevel()
{
  UNIT * tempUnit = pRunningLevel->nextUnit;
  while (tempUnit)
  {
    COMPONENT * tempComp = tempUnit->pArchetype->nextComponent;
    SPRITE * pSprite;
    SQUAREMESH * pMesh;
    while (tempComp)
    {
      switch (tempComp->Type)
      {
        case Sprite:
          pSprite = (SPRITE *)tempComp->pStruct;
          break;
        case SquareMesh:
          pMesh = (SQUAREMESH *) tempComp->pStruct;
          break;
        default:
          break;
      }
      
      if (pMesh && pSprite)
      {
        DrawObject(pMesh, pSprite);
      }
      
    }
  }
}

void DrawObject(SQUAREMESH * pMesh, SPRITE * pSprite)
{
  TRANSFORM * pMyTransform = pSprite->pArchetype->pUnit->pTransform;

  //Drawing object

  AEGfxSetRenderMode(AE_GFX_RM_COLOR);

  //Set Position of object
  AEGfxSetPosition(pMyTransform->Position.x, pMyTransform->Position.y);

  //Set texture for object
  AEGfxTextureSet(pSprite->pTexture, 0, 0);

  //Drawing the mesh (list of triangles)
  AEGfxSetTransparency(0.0);
  AEGfxMeshDraw(pMesh->pMesh, AE_GFX_MDM_TRIANGLES);
}