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
#include <string.h>
#include "FileInterpreter.h"
#include "Behaviors.h"
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
  pNewGame->nextLevel = NULL;
  pNewGame->pGameStats = SetDefaults(pNewGame);

  return pNewGame;
}

GAMESTATS * SetDefaults(GAME * pGame)
{
  GAMESTATS * pStats = malloc(sizeof(GAMESTATS));

  SPRITE * pSprite = malloc(sizeof(SPRITE));
  MESH * pMesh = malloc(sizeof(MESH));
  VECTOR zeroVector = { 0, 0 };
  VECTOR meshSizeVector = { 1, 1 };
  VECTOR oneVector = { 1, 1 };
  AEGfxVertexList * pLMesh;
  TRANSFORM * pTransform = malloc(sizeof(TRANSFORM));

  pTransform->Position = zeroVector;
  pTransform->Rotation = 0;
  pTransform->Scale = oneVector;
  pStats->pDefaultTransform = pTransform;

  pSprite->pTexture = AEGfxTextureLoad("Blank.png");
  pSprite->Animated = TRUE;
  pSprite->RowCol.x = 2;
  pSprite->RowCol.y = 5;
  pSprite->Offset = zeroVector;

  pSprite->AnimationSpeed = 0;

  pSprite->pComponent = NULL;

  pStats->pDefaultSprite = pSprite;
  
  ////Informing the library that we're about to start adding triangles.
  //AEGfxMeshStart();
  //
  ////This shape has 2 triangles.
  //AEGfxTriAdd(
	//  -16.0f, -16.0f, 0xFFFFFFFF, 0.0f, 0.5f,
	//  16.0f, -16.0f, 0xFFFFFFFF, 0.2f, 0.5f,
	//  -16.0f, 16.0f, 0xFFFFFFFF, 0.0f, 0.0f);
  //AEGfxTriAdd(
	//  16.0f, -16.0f, 0xFFFFFFFF, 0.2f, 0.5f,
	//  16.0f, 16.0f, 0xFFFFFFFF, 0.2f, 0.0f,
	//  -16.0f, 16.0f, 0xFFFFFFFF, 0.0f, 0.0f);
  //
  //
  //pLMesh = AEGfxMeshEnd();
  //AE_ASSERT_MESG(pMesh, "Failed to create default mesh");

  pMesh->pMeshLit = NULL;
  pMesh->Size = meshSizeVector;
  
  pStats->pDefaultMesh = pMesh;

  pStats->pRunningLevel = NULL;
  pStats->Health = 100;
  pStats->currentLevel = Level0;
  pStats->nextLevel = Level0;
  pStats->previousLevel = Level0;
  pStats->pDefaultBehavior = NULL;
  pStats->Points = 0;
  pStats->SpawnPoint = zeroVector;
  pStats->GridSize = 32;

  return pStats;
}

ARCHETYPE * CreateArchetype(GAME * pGame, char *Name)
{
  ARCHETYPE * pNewArchetype = malloc(sizeof(ARCHETYPE));
  pNewArchetype->Name = Name;
  pNewArchetype->nextComponent = NULL;
  pNewArchetype->pUnit = NULL;
  pNewArchetype->pGame = pGame;
  pNewArchetype->nextArchetype = pGame->nextArchetype;
  pGame->nextArchetype = pNewArchetype;
  return pNewArchetype;
}

void FreeGame(GAME * pGame)
{
  ARCHETYPE * temp = pGame->nextArchetype;
  LEVEL * tempLevel = pGame->nextLevel;
  if (pGame != NULL)
  {
    while (pGame->nextArchetype)
    {
      COMPONENT * tempComp = temp->nextComponent;
      while (temp->nextComponent)
      {
        free(tempComp->pStruct);

        tempComp = temp->nextComponent->nextComponent;
        free(temp->nextComponent);
        temp->nextComponent = tempComp;
      }
      temp = pGame->nextArchetype->nextArchetype;
      free(pGame->nextArchetype);
      pGame->nextArchetype = temp;
    }

    while (pGame->nextLevel)
    {
      UNIT * tempUnit = tempLevel->nextUnit;
      while (tempLevel->nextUnit)
      {
        //free(tempUnit->pInitArchetype);
        free(tempUnit->pInitTransform);
        
        tempUnit = tempLevel->nextUnit->nextUnit;
        free(tempLevel->nextUnit);
        tempLevel->nextUnit = tempUnit;
      }
      tempLevel = pGame->nextLevel->nextLevel;
      free(pGame->nextLevel);
      pGame->nextLevel = tempLevel;
    }
    // Freeing the objects and textures
    AEGfxMeshFree(pGame->pGameStats->pDefaultMesh->pMeshLit);
    
    // Freeing the texture
    AEGfxTextureUnload(pGame->pGameStats->pDefaultSprite->pTexture);

    free(pGame->pGameStats->pDefaultTransform);
    free(pGame->pGameStats);
    free(pGame);
  }
}

COMPONENT * AddComponent(ARCHETYPE *pArchetype, COMPONENTTYPE DesiredType)
{
  COMPONENT * pNewComponent = malloc(sizeof(COMPONENT));
  pNewComponent->pArchetype = pArchetype;
  pNewComponent->nextComponent = pArchetype->nextComponent;
  pArchetype->nextComponent = pNewComponent;

  if (DesiredType == Sprite)
  {
    SPRITE * pNewSprite = malloc(sizeof(SPRITE));
    pNewComponent->Type = Sprite;
    *pNewSprite = *(pArchetype->pGame->pGameStats->pDefaultSprite);
    pNewComponent->pStruct = pNewSprite;
    pNewSprite->TextureFile = NULL;
    pNewSprite->pComponent = pNewComponent;
    pNewSprite->pArchetype = pArchetype;
  }

  if (DesiredType == Mesh)
  {
    MESH * pNewMesh = malloc(sizeof(MESH));
    pNewComponent->Type = Mesh;
    *pNewMesh = *(pArchetype->pGame->pGameStats->pDefaultMesh);
    pNewComponent->pStruct = pNewMesh;
    pNewMesh->pComponent = pNewComponent;
    pNewMesh->pArchetype = pArchetype;
  }

  if (DesiredType == Behavior)
  {
    BEHAVIOR * pNewBehavior = malloc(sizeof(BEHAVIOR));
    pNewComponent->Type = Behavior;
    pNewComponent->pStruct = pNewBehavior;
    pNewBehavior->BehaviorScript = DefaultBehavior;
    pNewBehavior->pComponent = pNewComponent;
    pNewBehavior->pArchetype = pArchetype;
  }

  if (DesiredType == RigidBody)
  {
    RIGIDBODY * pNewRigidBody = malloc(sizeof(RIGIDBODY));
    
    pNewComponent->Type = RigidBody;
    pNewComponent->pStruct = pNewRigidBody;
    
    pNewRigidBody->Velocity = NewVector(0, 0);
    pNewRigidBody->Gravity = 0.5;
    pNewRigidBody->Friction = 0.1;
    pNewRigidBody->MaxSpeed = 0.5;

    pNewRigidBody->pComponent = pNewComponent;
    pNewRigidBody->pArchetype = pArchetype;
  }

  return pNewComponent;
}


COMPONENT * AddBehaviorComponent(ARCHETYPE *pArchetype, void(*BehaviorScript)(BEHAVIOR * Owner, char * Trigger))
{
  COMPONENT * pNewComponent = malloc(sizeof(COMPONENT));
  pNewComponent->pArchetype = pArchetype;
  pNewComponent->nextComponent = pArchetype->nextComponent;
  pArchetype->nextComponent = pNewComponent;
  
  BEHAVIOR * pNewBehavior = malloc(sizeof(BEHAVIOR));
  pNewComponent->Type = Behavior;
  pNewComponent->pStruct = pNewBehavior;
  pNewBehavior->BehaviorScript = BehaviorScript;
  pNewBehavior->pComponent = pNewComponent;
  pNewBehavior->pArchetype = pArchetype;

  return pNewComponent;
}

COMPONENT * FindComponent(ARCHETYPE * pArchetype, COMPONENTTYPE DesiredType)
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

void * FindComponentStruct(ARCHETYPE * pArchetype, COMPONENTTYPE DesiredType)
{
  COMPONENT * pComponent;
  pComponent = FindComponent(pArchetype, DesiredType);
  return pComponent->pStruct;
}

ARCHETYPE * FindArchetypeByName(GAME *pGame, char *Name)
{
	ARCHETYPE * temp = pGame->nextArchetype;
	while (temp)
	{
		if (myStrCmp(temp->Name, Name) <= 0)
		{
			return temp;
		}
		temp = temp->nextArchetype;
	}
	return NULL;
}

//Creates an empty Level.
LEVEL * AddLevel(GAME * pGame, char *Name, int Order)
{

	LEVEL * pNewLevel = malloc(sizeof(LEVEL));
	pNewLevel->Name = Name;
	pNewLevel->Order = Order;
	pNewLevel->pCamera = NULL;
	pNewLevel->nextUnit = NULL;
  pNewLevel->pGame = pGame;
	pNewLevel->nextLevel = pGame->nextLevel;
	pGame->nextLevel = pNewLevel;

  return pNewLevel;
}

//Adds a Unit of the given Archetype to the given level with the default Transform.
UNIT * AddUnit(LEVEL *pLevel, ARCHETYPE *pArchetype, char *Name)
{

	UNIT * pNewUnit = malloc(sizeof(UNIT));
  pNewUnit->pInitTransform = malloc(sizeof(TRANSFORM));
  pNewUnit->pTransform = malloc(sizeof(TRANSFORM));
	pNewUnit->Name = Name;
	pNewUnit->pInitArchetype = pArchetype;
  *(pNewUnit->pInitTransform) = *(pArchetype->pGame->pGameStats->pDefaultTransform);
	*(pNewUnit->pTransform) = *(pArchetype->pGame->pGameStats->pDefaultTransform);
	pNewUnit->pLevel = pLevel;
	pNewUnit->nextUnit = pLevel->nextUnit;
	pLevel->nextUnit = pNewUnit;

  return pNewUnit;
}

LEVEL * FindLevelByOrder(GAME *pGame, int Order)
{

	LEVEL * temp = pGame->nextLevel;
	while (temp)
	{
		if (temp->Order == Order)
		{
			return temp;
		}
		temp = temp->nextLevel;
	}

  printf("Couldn't find level");
	return NULL;
}

LEVEL * FindLevelByName(GAME *pGame, char * Name)
{
	LEVEL * temp = pGame->nextLevel;
	while (temp)
	{
		if (myStrCmp(temp->Name, Name) <= 0)
		{
			return temp;
		}
		temp = temp->nextLevel;
	}
	return NULL;
}

void InitializeUnit(UNIT * pUnit)
{
  COMPONENT * temp;
  pUnit->pTransform = malloc(sizeof(TRANSFORM));
  *(pUnit->pTransform) = *(pUnit->pInitTransform);

  pUnit->pArchetype = CreateInstanceOfArchetype(pUnit->pInitArchetype, pUnit);

  temp = pUnit->pArchetype->nextComponent;
  while (temp)
  {
    if (temp->Type == Behavior)
    {
      ((BEHAVIOR*)temp->pStruct)->BehaviorScript(((BEHAVIOR*)temp->pStruct), "Start");
      break;
    }
    temp = temp->nextComponent;
  }

}

ARCHETYPE * CreateInstanceOfArchetype(ARCHETYPE * pArchetype, UNIT * pUnit)
{
  ARCHETYPE * pNewArchetype = malloc(sizeof(ARCHETYPE));
  COMPONENT * temp = pArchetype->nextComponent;
  //pNewArchetype->Name = strcat(pArchetype->Name, "(Instance)");
  pNewArchetype->nextComponent = NULL;
  pNewArchetype->nextArchetype = NULL;
  pNewArchetype->pGame = pArchetype->pGame;
  pNewArchetype->pUnit = pUnit;
  while (temp)
  {
	  COMPONENT * compCopy = malloc(sizeof(COMPONENT));
    compCopy->pArchetype = pNewArchetype;
    compCopy->Type = temp->Type;

    if (temp->Type == Sprite)
    {
      SPRITE * spriteCopy = malloc(sizeof(SPRITE));
      *spriteCopy = *((SPRITE *) temp->pStruct);
      spriteCopy->pArchetype = pNewArchetype;
      spriteCopy->pComponent = compCopy;
      compCopy->pStruct = spriteCopy;
    }
    else if (temp->Type == Mesh)
    {
      MESH * sqmeshCopy = malloc(sizeof(MESH));
      *sqmeshCopy = *((MESH *)temp->pStruct);
      sqmeshCopy->pArchetype = pNewArchetype;
      sqmeshCopy->pComponent = compCopy;
      compCopy->pStruct = sqmeshCopy;
    }
    else if (temp->Type == Behavior)
    {
      BEHAVIOR * behCopy = malloc(sizeof(BEHAVIOR));
      *behCopy = *((BEHAVIOR *)temp->pStruct);
      behCopy->pArchetype = pNewArchetype;
      behCopy->pComponent = compCopy;
      compCopy->pStruct = behCopy;
    }
    else if (temp->Type == RigidBody)
    {
      RIGIDBODY * rbCopy = malloc(sizeof(RIGIDBODY));
      *rbCopy = *((RIGIDBODY *)temp->pStruct);
      rbCopy->pArchetype = pNewArchetype;
      rbCopy->pComponent = compCopy;
      compCopy->pStruct = rbCopy;
    }

		compCopy->nextComponent = pNewArchetype->nextComponent;
    pNewArchetype->nextComponent = compCopy;

    temp = temp->nextComponent;
  }

  return pNewArchetype;
}

VECTOR NewVector(float x, float y)
{
  VECTOR newVec = { x, y };
  return newVec;
}