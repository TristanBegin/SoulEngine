// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	main entry point2 for the test program
// History			:
// - 2012/01/11		:	- initial implementation
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "SoulEngine.h"

// ---------------------------------------------------------------------------

// Libraries
//#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

//int gGameRunning = 1;


//Base functions

GAME * InitializeGame(char * Name)
{
  GAME * pNewGame = malloc(sizeof(GAME));
  pNewGame->Name = Name;
  pNewGame->nextArchetype = NULL;
  pNewGame->nextLevel == NULL;
  pNewGame->pGameStats = SetDefaults(pNewGame);
}

GAMESTATS * SetDefaults(GAME * pGame)
{
  GAMESTATS * pStats = malloc(sizeof(GAMESTATS));

  SPRITE * pSprite = malloc(sizeof(SPRITE));
  VECTOR zeroVector = { 0, 0 };

  pSprite->pTexture = AEGfxTextureLoad("Blank.png");
  pSprite->Offset = zeroVector;
  pSprite->AnimationSpeed = 0;
  pSprite->pComponent = NULL;

  pStats->pDefaultSprite = pSprite;
  
  pStats->Health = 100;
  pStats->pCurrentLevel = NULL;
  pStats->pDefaultBehavior = NULL;
  pStats->pDefaultSquareMesh = NULL;
  pStats->pDefaultTransform = NULL;
  pStats->Points = 0;
  pStats->SpawnPoint = zeroVector;


  return pStats;
}

ARCHETYPE * CreateArchetype(GAME * pGame, char *Name)
{
  ARCHETYPE * pNewArchetype = malloc(sizeof(ARCHETYPE));
  pNewArchetype->Name = Name;
  pNewArchetype->nextComponent = NULL;
  pNewArchetype->pGame = pGame;
  pNewArchetype->nextArchetype = pGame->nextArchetype;
  pGame->nextArchetype = pNewArchetype;
  return pNewArchetype;
}

void FreeGame(GAME * pGame)
{
  ARCHETYPE * temp;
  if (pGame != NULL)
  {
    while (pGame->nextArchetype)
    {
      temp = pGame->nextArchetype->nextArchetype;
      free(pGame->nextArchetype);
      pGame->nextArchetype = temp;
    }
    free(pGame);
  }
}

COMPONENT * AddComponent(ARCHETYPE *pArchetype, COMPONENTTYPE DesiredType)
{
  COMPONENT * pNewComponent = malloc(sizeof(COMPONENT));
  pNewComponent->pArchetype = pArchetype;
  pNewComponent->nextComponent = pArchetype->nextComponent;
  pArchetype->nextComponent = pNewComponent;

  if (DesiredType = Sprite)
  {
    SPRITE * pNewSprite = malloc(sizeof(SPRITE));
    pNewComponent->Type = Sprite;
    pNewComponent->pStruct = pNewSprite;
    *pNewSprite = *(pArchetype->pGame->pGameStats->pDefaultSprite);
    pNewSprite->pComponent = pNewComponent;
  }

  return pNewComponent;
}

COMPONENT * GetComponent(ARCHETYPE * pArchetype, COMPONENTTYPE DesiredType)
{
  COMPONENT * temp = pArchetype->nextComponent;
  while (temp)
  {
    if (temp->Type == DesiredType)
    {
      return temp;
    }
    temp = temp->nextComponent;
  }
  
  return NULL;
}

// ---------------------------------------------------------------------------
// main

/*
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mCreateWindow		= 1;
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 800; 
	sysInitInfo.mWinHeight			= 600;
	sysInitInfo.mCreateConsole		= 1;
	sysInitInfo.mMaxFrameRate		= 60;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;											
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
	sysInitInfo.mWindowHandle		= NULL;
	sysInitInfo.mHandleWindowMessages	= 1;
	AESysInit (&sysInitInfo);

	// reset the system modules
	AESysReset();

	// Game Loop
	while(gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();


		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// free the system
	AESysExit();

	return 1;
}
*/
// ---------------------------------------------------------------------------

