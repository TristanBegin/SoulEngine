#include "SoulEngine.h"
#define CURRENT_LEVEL pTheGame->pGameStats->currentLevel
#define NEXT_LEVEL pTheGame->pGameStats->nextLevel
#define PREVIOUS_LEVEL pTheGame->pGameStats->previousLevel

extern GAME * pTheGame;
LEVEL * pRunningLevel;
BEHAVIOR ** pBehaviorArray = NULL;
int numBehaviors;


void LoadLevel();
void InitializeLevel();
void UpdateLevel();
void DrawLevel();
void FreeLevel();
void UnloadLevel();

void InitializeGSM(void)
{

}

void UpdateGSM(void)
{
	LEVEL *pRunningLevel = FindLevelByOrder(pTheGame, CURRENT_LEVEL);
}

void MainLoopGSM(void)
{
	while (CURRENT_LEVEL != Quit)
	{
		if (NEXT_LEVEL == Restart)
		{
			CURRENT_LEVEL = PREVIOUS_LEVEL;
			NEXT_LEVEL = PREVIOUS_LEVEL;
		}
		else
		{
			UpdateGSM();
			//Load Level
		}

    InitializeLevel();

		while (CURRENT_LEVEL == NEXT_LEVEL)
		{
			//Input
			//Update Level
			//Draw Level
		}

		//Free Level

		if (NEXT_LEVEL != Restart)
		{
			//Unload level
		}

		PREVIOUS_LEVEL = CURRENT_LEVEL;
		CURRENT_LEVEL = NEXT_LEVEL;
	}
}


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