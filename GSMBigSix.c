#include "SoulEngine.h"
#include "GSMBigSix.h"

extern LEVEL * pRunningLevel;
extern BEHAVIOR ** pBehaviorArray = NULL;
extern int numBehaviors;

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

void UpdateLevel()
{

}

void DrawLevel()
{
  UNIT * tempUnit = pRunningLevel->nextUnit;
  while (tempUnit)
  {
    COMPONENT * tempComp = tempUnit->pArchetype->nextComponent;
    while (tempComp)
    {
      /*
      switch (tempComp->Type)
      {
      case Sprite:
        break;
        break;
      }
      */
    }
  }
}