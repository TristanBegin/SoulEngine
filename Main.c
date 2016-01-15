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
	//Variable declaration
  unsigned char colors[16]; // 2x2 image.
	float obj1X = 0.0f, obj1Y = 0.0f; //Position variables for object 1.
  float obj2texX = 0, obj2texY = 0; //Texture variables for object 2's texture.
	AEGfxVertexList *pMesh1; //Pointer to Mest (Model).
  AEGfxVertexList *pMesh2; //Pointer to Mest (Model).
  AEGfxVertexList *pMeshLine; //Pointer to Mest (Model).
  AEGfxTexture *pTex1, *pTex2; //Pointer to Texture (Image).
  float camX, camY; //Used to temporarily store camera position.
	float alpha = 1.0f;

  ARCHETYPE * pPlayerArch;
  COMPONENT * pComponent;
  SPRITE * pSprite;

  // Initialize the system
  SystemInitialize(instanceH, show);

  // Initialize Game
  pTheGame = InitializePlatformerGame();

  //-------------------------------------------------------------------------------//

  //Informing the library that we're about to start adding triangles.
  AEGfxMeshStart();

	//1 triangle at a time.
	//X, Y, Color, texU, texV
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f,
		25.5f, 0.0f, 0xFFFF0000, 0.0f, 0.0f,
		-25.5f, 25.5f, 0xFFFF0000, 0.0f, 0.0f);

	//Saving the mest (list of triangles) in pMesh1
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1");

  //Change background color.
  AEGfxSetBackgroundColor(0.0f, 0.15f, 0.25f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);

  //Informing the library that we're about to start adding triangles.
  AEGfxMeshStart();

  //This shape has 2 triangles.
  AEGfxTriAdd(
    -16.0f, -16.0f, 0x00FF00FF, 0.0f, 1.0f,
    16.0f, -16.0f, 0x00FFFF00, 1.0f, 1.0f,
    -16.0f, 16.0f, 0x00F00FFF, 0.0f, 0.0f);
  AEGfxTriAdd(
    16.0f, -16.0f, 0x00FF00FF, 1.0f, 1.0f,
    16.0f, 16.0f, 0x00FFFF00, 1.0f, 0.0f,
    -16.0f, 16.0f, 0x00F00FFF, 0.0f, 0.0f);

  pMesh2 = AEGfxMeshEnd();
  AE_ASSERT_MESG(pMesh2, "Failed to create mesh 2");

  // Informing the library that we're about to start adding vertices
  AEGfxMeshStart();

  // This shape has 5 vertices

  AEGfxVertexAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
  AEGfxVertexAdd(100.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
  AEGfxVertexAdd(200.0f, 150.0f, 0xFFFFFFFF, 0.0f, 0.0f);
  AEGfxVertexAdd(300.0f, -100.0f, 0xFFFFFFFF, 0.0f, 0.0f);
  AEGfxVertexAdd(100.0f, -250.0f, 0xFFFFFFFF, 0.0f, 0.0f);

  pMeshLine = AEGfxMeshEnd();
  AE_ASSERT_MESG(pMeshLine, "Failed to create mesh line.");

  //Texture 1: From file.
  pTex1 = AEGfxTextureLoad("Blank.png");
  AE_ASSERT_MESG(pTex1, "Failed to create texture 1.");

  //Texture 2: from memory.
  //RGBA format
  colors[0] = 255;  colors[1] = 0;    colors[2] = 0;    colors[3] = 255;
  colors[4] = 0;    colors[5] = 255;  colors[6] = 0;    colors[7] = 255;
  colors[8] = 0;    colors[9] = 0;    colors[10] = 255; colors[11] = 255;
  colors[12] = 255; colors[13] = 255; colors[14] = 255; colors[15] = 255;
  pTex2 = AEGfxTextureLoadFromMemory(colors, 2, 2);
  AE_ASSERT_MESG(pTex2, "Failed to create texture 2.");

	//Game Loop
	while (gGameRunning)
	{

		//Informing the system about the loop's start.
		AESysFrameStart();

		// Handling Input.
		AEInputUpdate();

		if (AEInputCheckCurr(VK_UP))
		{
			obj1Y += 3.0;
		}
		else if (AEInputCheckCurr(VK_DOWN))
		{
			obj1Y -= 3.0f;
		}

		if (AEInputCheckCurr(VK_LEFT))
		{
			obj1X -= 3.0f;
		}
		else if (AEInputCheckCurr(VK_RIGHT))
		{
			obj1X += 3.0f;
		}

		//Alpha value
		if (AEInputCheckCurr('Z'))
		{
			alpha -= 0.01f;
		}
		else if (AEInputCheckCurr('X'))
		{
			alpha += 0.01f;
		}

		alpha = AEClamp(alpha, 0.0f, 1.0f);

    // Move the camera.
    AEGfxGetCamPosition(&camX, &camY);
    if (AEInputCheckCurr('W'))
    {
      AEGfxSetCamPosition(camX, camY + 2);
    }
    else if (AEInputCheckCurr('S'))
    {
      AEGfxSetCamPosition(camX, camY - 2);
    }

    AEGfxGetCamPosition(&camX, &camY);
    if (AEInputCheckCurr('A'))
    {
      AEGfxSetCamPosition(camX - 2, camY);
    }
    else if (AEInputCheckCurr('D'))
    {
      AEGfxSetCamPosition(camX + 2, camY);
    }

    // Texture offset
    if (AEInputCheckCurr('L'))
        obj2texX += 0.01f;
    else if (AEInputCheckCurr('J'))
        obj2texX -= 0.01f;

    if (AEInputCheckCurr('I'))
        obj2texY += 0.01f;
    else if (AEInputCheckCurr('K'))
        obj2texY -= 0.01f;

		//Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//Set Position of object 1
		AEGfxSetPosition(obj1X, obj1Y);
		//No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		//Drawing the mesh (list of triangles)
		AEGfxSetTransparency(alpha);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

    //Drawing object 2.
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    // Set Position of object 2
    AEGfxSetPosition(100.0f, -60.0f);
    // Set texture for object 2
    AEGfxTextureSet(pTex1, obj2texX, obj2texY);
    AEGfxSetTransparency(1.0f);
    // Drawing the mesh (list of triangles).
    AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);

    //Drawing object 2 again.
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    // Set Position of object 2
    AEGfxSetPosition(-100.0f, -60.0f);
    // Set texture for object 2
    AEGfxTextureSet(pTex2, 0.0f, 0.0f);
    AEGfxSetTransparency(1.0f);
    // Drawing the mesh (list of triangles).
    AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
		
    // Drawing object 2 again and again
    // Set poisition for object 2
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetPosition(100.0f, 100.0f);
    // Set texture for object 2
    AEGfxTextureSet(pTex1, obj2texX, obj2texY);
    AEGfxSetTransparency(1.0f);
    AEGfxSetBlendColor(0.5f, 0.0f, 0.0, 0.5f);
    // Drawing the mesh (list of triangles)
    AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);

		//Informing the system about the loop's end.
		AESysFrameEnd();

		//Check if forcing the application to quit.
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
		{
			gGameRunning = 0;
		}
	}

	//Free the objects and textures
	AEGfxMeshFree(pMesh1);
  AEGfxMeshFree(pMesh2);
  AEGfxMeshFree(pMeshLine);

  AEGfxTextureUnload(pTex1);
  AEGfxTextureUnload(pTex2);

	//Free the system.
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------

