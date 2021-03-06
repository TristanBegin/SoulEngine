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

#include "KSOUND.h"
// ---------------------------------------------------------------------------

// Libraries
//#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
extern GAME * pTheGame;
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
  IMAGE * pImage = malloc(sizeof(IMAGE));
  MESH * pMesh = malloc(sizeof(MESH));
  VECTOR zeroVector = { 0, 0 };
  VECTOR meshSizeVector = { 1, 1 };
  VECTOR oneVector = { 1, 1 };
  //AEGfxVertexList * pLMesh;
  TRANSFORM * pTransform = malloc(sizeof(TRANSFORM));

  pTransform->Position = zeroVector;
  pTransform->Rotation = 0;
  pTransform->Scale = oneVector;
  pStats->pDefaultTransform = pTransform;

  pImage->pNextImage = NULL;
  pSprite->pImage = pImage;
  pSprite->pImage->pTexture = AEGfxTextureLoad("WitchWalking01.png");
  pSprite->Animated = TRUE;
  pSprite->RowCol.x = 2;
  pSprite->RowCol.y = 5;
  pSprite->Offset = zeroVector;

  pSprite->AnimationSpeed = 0;
  pSprite->TimeSinceLastFrame = 0;

  pSprite->pComponent = NULL;

  pStats->pDefaultSprite = pSprite;
  

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
  pStats->GridSize = 48;

  return pStats;
}

ARCHETYPE * CreateArchetype(GAME * pGame, char *Name)
{
  ARCHETYPE * pNewArchetype = malloc(sizeof(ARCHETYPE));
  pNewArchetype->Name = Name;
  pNewArchetype->Tag = DEFAULT;
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
    AEGfxTextureUnload(pGame->pGameStats->pDefaultSprite->pImage->pTexture);

    free(pGame->pGameStats->pDefaultTransform);
    free(pGame->pGameStats);
    free(pGame);
  }
}

COMPONENT * AddComponent(ARCHETYPE *pArchetype, COMPONENTTYPE DesiredType)
{
  COMPONENT * pNewComponent = calloc(1, sizeof(COMPONENT));
  pNewComponent->pArchetype = pArchetype;
  pNewComponent->nextComponent = pArchetype->nextComponent;
  pArchetype->nextComponent = pNewComponent;

  if (DesiredType == Sprite) {
	  SPRITE * pNewSprite = calloc(1, sizeof(SPRITE));
	  IMAGE * pImage = calloc(1, sizeof(IMAGE));
    pNewSprite->Visible = True;
	  pImage->pNextImage = NULL;
	  pNewComponent->Type = Sprite;
	  *pNewSprite = *(pArchetype->pGame->pGameStats->pDefaultSprite);
		pNewSprite->pImage = pImage;
		pNewSprite->CurrentAnimation = "Blank.png";
	  pNewComponent->pStruct = pNewSprite;
	  pNewSprite->pImage->TextureFile = NULL;
	  pNewSprite->pComponent = pNewComponent;
	  pNewSprite->pArchetype = pArchetype;
  }

  if (DesiredType == Mesh)
  {
    MESH * pNewMesh = calloc(1, sizeof(MESH));
    pNewComponent->Type = Mesh;
    *pNewMesh = *(pArchetype->pGame->pGameStats->pDefaultMesh);
    pNewMesh->Color = NewColor(1, 1, 1, 1);
    pNewMesh->Opacity = 1.0;
    pNewComponent->pStruct = pNewMesh;
    pNewMesh->pComponent = pNewComponent;
    pNewMesh->pArchetype = pArchetype;
  }

  if (DesiredType == Behavior)
  {
    BEHAVIOR * pNewBehavior = calloc(1, sizeof(BEHAVIOR));
    pNewComponent->Type = Behavior;
    pNewComponent->pStruct = pNewBehavior;
    pNewBehavior->BehaviorScript = DefaultBehavior;
    pNewBehavior->pComponent = pNewComponent;
    pNewBehavior->pArchetype = pArchetype;
  }

  if (DesiredType == Physics)
  {
    PHYSICS * pNewPhysics = calloc(1, sizeof(PHYSICS));

    pNewComponent->Type = Physics;
    pNewComponent->pStruct = pNewPhysics;

    pNewPhysics->Velocity = NewVector(0, 0);
    pNewPhysics->Acceleration = NewVector(0, 0);
    pNewPhysics->Gravity = 0.5;
    pNewPhysics->Friction = 0.1f;
    pNewPhysics->MaxSpeed = 0.5;

    pNewPhysics->pComponent = pNewComponent;
    pNewPhysics->pArchetype = pArchetype;
  }

  if (DesiredType == Collider)
  {
    COLLIDER * pNewCollider = calloc(1, sizeof(COLLIDER));
    
    pNewComponent->Type = Collider;
    pNewComponent->pStruct = pNewCollider;
    
    pNewCollider->Enabled = True;
    pNewCollider->Offset = NewVector(0, 0);
    pNewCollider->Height = 1;
    pNewCollider->Width = 1;
        
    pNewCollider->pComponent = pNewComponent;
    pNewCollider->pArchetype = pArchetype;
  }

  if (DesiredType == KSound)
  {
    KSOUND * pNewSound = calloc(1, sizeof(KSOUND));
	KSOUND_Init(pNewSound);
    
	pNewComponent->Type = KSound;
    pNewComponent->pStruct = pNewSound;

	// Generic component stuff.
    pNewSound->pComponent = pNewComponent;
    pNewSound->pArchetype = pArchetype;
  }

  return pNewComponent;
}


COMPONENT * AddBehaviorComponent(ARCHETYPE *pArchetype, void(*BehaviorScript)(BEHAVIOR * Owner, char * Trigger, void * Data))
{
  COMPONENT * pNewComponent = calloc(1, sizeof(COMPONENT));
  pNewComponent->pArchetype = pArchetype;
  pNewComponent->nextComponent = pArchetype->nextComponent;
  pArchetype->nextComponent = pNewComponent;
  
  BEHAVIOR * pNewBehavior = calloc(1, sizeof(BEHAVIOR));
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
  if (pComponent) return pComponent->pStruct;
  else return NULL;
}

void * AddVar(VTYPE Type, char * Name, BEHAVIOR * Owner)
{
  VAR * pNewVar = malloc(sizeof(VAR));
  pNewVar->Name = myStrCpy(Name);
  pNewVar->Type = Float;
  pNewVar->nextVar = NULL;

  // NOTE: SHOULD MAKE A SWITCH CASE. MUCH CLEANER (AND SOMETIMES FASTER!)
  if (Type == Int)
  {
    int *x = malloc(sizeof(int));
    *x = 0;
    pNewVar->Data = x;
  }
  else if (Type == Float)
  {
    float *x = malloc(sizeof(float));
    *x = 0.0;
    pNewVar->Data = x;
  }
  else if (Type == Vector)
  {
    VECTOR *x = malloc(sizeof(VECTOR));
    *x = NewVector(0,0);
    pNewVar->Data = x;
  }
  else if (Type == String)
  {
    char **x = malloc(sizeof(char*));
    *x = NULL;
    pNewVar->Data = x;
  }
  else if (Type == Color)
  {
    COLOR newCol = { 1, 1, 1, 1 };
    COLOR *x = malloc(sizeof(COLOR));
    *x = newCol;
    pNewVar->Data = x;
  }
  else if (Type == Bool)
  {
    BOOL *x = malloc(sizeof(BOOL));
    *x = False;
    pNewVar->Data = x;
  }
  else if (Type == Char)
  {
    char *x = malloc(sizeof(char));
    *x = '\0';
    pNewVar->Data = x;
  }
  else if (Type == Matrix)
  {
    MATRIX *temp = (MATRIX*)malloc(sizeof(MATRIX));
    SecureZeroMemory(temp->m, sizeof(float)* 9);
    pNewVar->Data = temp;
  }
  pNewVar->nextVar = Owner->nextVar;
  Owner->nextVar = pNewVar;
  return pNewVar->Data;
}

VAR * AddUnitVar(VTYPE Type, char * Name, UNIT * Owner)
{
  VAR * pNewVar = malloc(sizeof(VAR));
  pNewVar->Name = myStrCpy(Name);
  pNewVar->Type = Float;

  if (Type == Int)
  {
    int *x = malloc(sizeof(int));
    *x = 0;
    pNewVar->Data = x;
  }
  else if (Type == Float)
  {
    float *x = malloc(sizeof(float));
    *x = 0.0;
    pNewVar->Data = x;
  }
  else if (Type == Vector)
  {
    VECTOR *x = malloc(sizeof(VECTOR));
    *x = NewVector(0, 0);
    pNewVar->Data = x;
  }
  else if (Type == String)
  {
    char **x = malloc(sizeof(char*));
    *x = NULL;
    pNewVar->Data = x;
  }
  else if (Type == Color)
  {
    COLOR newCol = { 1, 1, 1, 1 };
    COLOR *x = malloc(sizeof(COLOR));
    *x = newCol;
    pNewVar->Data = x;
  }
  else if (Type == Bool)
  {
    BOOL *x = malloc(sizeof(BOOL));
    *x = False;
    pNewVar->Data = x;
  }
  else if (Type == Char)
  {
    char *x = malloc(sizeof(char));
    *x = '\0';
    pNewVar->Data = x;
  }
  else if (Type == Matrix)
  {
    MATRIX *temp = (MATRIX*)malloc(sizeof(MATRIX));
    SecureZeroMemory(temp->m, sizeof(float) * 9);
    pNewVar->Data = temp;
  }
  pNewVar->nextVar = Owner->nextVar;
  Owner->nextVar = pNewVar;
  return pNewVar;
  return NULL;
}

void * GetVar(char * Name, BEHAVIOR * Owner)
{
  return GetActualVar(Name, Owner)->Data;
}

VAR * GetActualVar(char * Name, BEHAVIOR * Owner)
{
  VAR * temp = Owner->nextVar;
  while (temp)
  {
    if (myStrCmp(temp->Name, Name) <= 0)
    {
      return temp;
    }
    temp = temp->nextVar;
  }
  return NULL;
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
  CAMERA * pCam = malloc(sizeof(CAMERA));
  pCam->Position = NewVector(0, 0);
  pCam->Zoom = 48;
  
	pNewLevel->Name = myStrCpy(Name);
	pNewLevel->Order = Order;
	pNewLevel->pCamera = pCam;
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
	pNewUnit->Name = myStrCpy(Name);
  pNewUnit->Tag = pArchetype->Tag;
	pNewUnit->pInitArchetype = pArchetype;
  *(pNewUnit->pInitTransform) = *(pArchetype->pGame->pGameStats->pDefaultTransform);
	*(pNewUnit->pTransform) = *(pArchetype->pGame->pGameStats->pDefaultTransform);
	pNewUnit->pLevel = pLevel;
  pNewUnit->nextVar = NULL;
	pNewUnit->nextUnit = pLevel->nextUnit;
	pLevel->nextUnit = pNewUnit;

  return pNewUnit;
}

UNIT * InstantiateUnit(LEVEL *pLevel, char * ArchetypeName, VECTOR position)
{
  ARCHETYPE * pArchetype = FindArchetypeByName(pTheGame, ArchetypeName);
  
  UNIT * pUnit = AddUnit(pLevel, pArchetype, ArchetypeName);
  pUnit->pInitTransform->Position = position;
  pUnit->Tag = pUnit->pInitArchetype->Tag;
  InitializeUnit(pUnit);
  pUnit->pInitTransform = NULL;
  pUnit->pInitArchetype = NULL;
  return pUnit;
}

void DestroyUnit(UNIT * pUnit)
{
  //if (pUnit->pInitTransform)
  //{
    //Remember to free these later and add functionality to remove from list.
    pUnit->pTransform = NULL;
    pUnit->pArchetype = NULL;
  //}
  //else
  //{

  //}
}

UNIT * FindUnitByName(LEVEL *pLevel, char * Name)
{
  UNIT * temp = pLevel->nextUnit;
  while (temp)
  {
    if (myStrCmp(temp->Name, Name) <= 0)
    {
      return temp;
    }
    temp = temp->nextUnit;
  }
  return NULL;
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
      VAR *tempVar = pUnit->nextVar;
      BEHAVIOR * pBehavior = ((BEHAVIOR*)temp->pStruct);
      pBehavior->BehaviorScript(((BEHAVIOR*)temp->pStruct), "Start", NULL);
      while (tempVar)
      {
        VAR * pBVar = GetActualVar(tempVar->Name, pBehavior);
        if (pBVar)
        {
          float f = *(float*)tempVar->Data;
          pBVar->Data = tempVar->Data;
          f = *(float*)pBVar->Data;
          f = *(float*)pBVar->Data;
        }
        else 
        {
          //AddVar(tempVar->Type, tempVar->Name, pBehavior);
        }
        tempVar = tempVar->nextVar;
      }
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
  pNewArchetype->Tag = pUnit->Tag;
  pNewArchetype->Name = myStrCpy(pArchetype->Name);
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
    else if (temp->Type == Physics)
    {
      PHYSICS * physCopy = malloc(sizeof(PHYSICS));
      *physCopy = *((PHYSICS *)temp->pStruct);
      physCopy->pArchetype = pNewArchetype;
      physCopy->pComponent = compCopy;
      compCopy->pStruct = physCopy;
    }
    else if (temp->Type == Collider)
    {
      COLLIDER * collCopy = malloc(sizeof(COLLIDER));
      *collCopy = *((COLLIDER *)temp->pStruct);
      collCopy->pArchetype = pNewArchetype;
      collCopy->pComponent = compCopy;
      compCopy->pStruct = collCopy;
    }
    else if (temp->Type == KSound)
    {
      KSOUND * soundCopy = malloc(sizeof(KSOUND));
      *soundCopy = *((KSOUND *)temp->pStruct);
      soundCopy->pArchetype = pNewArchetype;
      soundCopy->pComponent = compCopy;
      compCopy->pStruct = soundCopy;
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

COLOR NewColor(float r, float g, float b, float a)
{
  COLOR newColor = { r, g, b, a };
  return newColor;
}