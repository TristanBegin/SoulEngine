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
#include "System.h"
#include "PlatformerGame.h"
// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

void MainLoopGSM(void);
// ---------------------------------------------------------------------------
// globals
int gGameRunning = 1;
GAME * pTheGame;
// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main


int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{

  // Initialize the system
  SystemInitialize(instanceH, show);

  // Initialize Game
  InitializePlatformerGame();

  //Change background color.
  AEGfxSetBackgroundColor(0.0f, 0.15f, 0.25f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);

  //Start Game Loop
  MainLoopGSM();

  
  // Game Loop
  while (gGameRunning)
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
  FreeGame(pTheGame);
	//Free the system.
	AESysExit();

	return 1;
}