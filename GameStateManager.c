#include "SoulEngine.h"
#include "GSMBigSix.h"


#define CURRENT_LEVEL pTheGame->pGameStats->currentLevel
#define NEXT_LEVEL pTheGame->pGameStats->nextLevel
#define PREVIOUS_LEVEL pTheGame->pGameStats->previousLevel

extern GAME * pTheGame;

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
	pTheGame->pGameStats->pRunningLevel = FindLevelByOrder(pTheGame, CURRENT_LEVEL);

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
			LoadLevel();
		}

    InitializeLevel();

		while (CURRENT_LEVEL == NEXT_LEVEL)
		{

      //Informing the system about the loop's start.
      AESysFrameStart();

      AEInputUpdate();


      //InputFunction?
      UpdateLevel();
      DrawLevel();

      if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
      {
        NEXT_LEVEL = Quit;
      }

      AESysFrameEnd();
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


