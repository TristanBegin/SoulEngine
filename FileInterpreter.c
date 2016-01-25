#include "SoulEngine.h"
#include "Behaviors.h"
#include <stdio.h>
#include "FileInterpreter.h"

#define MAX_LENGTH 80

#pragma warning(disable : 4996)

extern GAME * pTheGame;

void AddNull(char * buffer)
{
  buffer[strlen(buffer) - 1] = '\0';
}

void InterpretArchetypeFiles()
{
  FILE * fpArchList = NULL;
  FILE * fpCurArch = NULL;
  char buffer[MAX_LENGTH];

  fpArchList = fopen("_ArchetypeList.txt", "r");
  if (fpArchList)
  {
    while (!feof(fpArchList))
    {
      if (fgets(buffer, MAX_LENGTH, fpArchList) && buffer[0] != '#' && strlen(buffer) >= 2)
      {
        AddNull(buffer);
        fpCurArch = fopen(buffer, "r");
        if (fpCurArch)
        {
          InterpretArchetype(fpCurArch);
          fclose(fpCurArch);
        }
      }
    }

    fclose(fpArchList);
  }
}

void InterpretArchetype(FILE * fpArch)
{
  char buffer[MAX_LENGTH];
  ARCHETYPE * pNewArchetype = malloc(sizeof(ARCHETYPE));
  COMPONENT * pCurrComp = NULL;
  pNewArchetype->Name = "Untitled";
  pNewArchetype->nextArchetype = NULL;
  pNewArchetype->nextComponent = NULL;
  pNewArchetype->pGame = pTheGame;
  pNewArchetype->pUnit = NULL;
  pNewArchetype->nextArchetype = pTheGame->nextArchetype;
  pTheGame->nextArchetype = pNewArchetype;
  while (!feof(fpArch))
  {
    if (fgets(buffer, MAX_LENGTH, fpArch))
    {
      char question[MAX_LENGTH];
      int inputInt = 0;
      float inputFloat = 0.0f;
      VECTOR inputVector = NewVector(0, 0);
      AddNull(buffer);
      if (buffer[0] != '#' && strlen(buffer) > 2)
      {
        sscanf(buffer, "%s", &question);
        if (myStrCmp(question, "Name") <= 0)
        {
          char nameInput[MAX_LENGTH];
          sscanf(buffer, "Name = %s", nameInput);
          pNewArchetype->Name = myStrCpy(nameInput);
          continue;
        }

        if (myStrCmp(question, "Tag") <= 0)
        {
          char tagInput[MAX_LENGTH];
          sscanf(buffer, "Tag = %s", tagInput);
          pNewArchetype->Tag = GetTagFromString(tagInput);
          continue;
        }

        if (myStrCmp(question, "COMPONENT") <= 0)
        {
          COMPONENTTYPE theType;
          char typeInput[MAX_LENGTH];
          sscanf(buffer, "COMPONENT |%s|", &typeInput);
          if (myStrCmp(typeInput, "Sprite") <= 0) theType = Sprite;
          if (myStrCmp(typeInput, "Mesh") <= 0) theType = Mesh;
          if (myStrCmp(typeInput, "Behavior") <= 0) theType = Behavior;
          if (myStrCmp(typeInput, "Physics") <= 0) theType = Physics;
          if (myStrCmp(typeInput, "Collider") <= 0) theType = Collider;
          if (myStrCmp(typeInput, "KSound") <= 0) theType = KSound;


          pCurrComp = AddComponent(pNewArchetype, theType);
          continue;
        }

        if (pCurrComp)
        {
          if (pCurrComp->Type == Mesh)
          {
            MESH * pMesh = (MESH*)pCurrComp->pStruct;
            if (myStrCmp(question, "Size") <= 0)
            {
              sscanf(buffer, "\tSize = (%f, %f)", &inputVector.x, &inputVector.y);
              pMesh->Size = inputVector;
              continue;
            }

            if (myStrCmp(question, "Color") <= 0)
            {
              COLOR inputColor;
              sscanf(buffer, "\tColor = (%f, %f, %f, %f)", &inputColor.r, &inputColor.g, &inputColor.b, &inputColor.a);
              pMesh->Color = inputColor;
              continue;
            }

            if (myStrCmp(question, "Opacity") <= 0)
            {
              sscanf(buffer, "\tOpacity = %f", &inputFloat);
              pMesh->Opacity = inputFloat;
              continue;
            }
          }

          if (pCurrComp->Type == Behavior)
          {
            BEHAVIOR * pBehavior = (BEHAVIOR*)pCurrComp->pStruct;
            if (myStrCmp(question, "BehaviorScript") <= 0)
            {
              char scriptInput[MAX_LENGTH];
              sscanf(buffer, "\tBehaviorScript = %s", &scriptInput);
              pBehavior->BehaviorScript = GetBehaviorFromString(scriptInput);
              continue;
            }
          }

          if (pCurrComp->Type == Physics)
          {
            PHYSICS * pPhysics = (PHYSICS*)pCurrComp->pStruct;
            if (myStrCmp(question, "Gravity") <= 0)
            {
              sscanf(buffer, "\tGravity = %f", &inputFloat);
              pPhysics->Gravity = inputFloat;
              continue;
            }

            if (myStrCmp(question, "MaxSpeed") <= 0)
            {
              sscanf(buffer, "\tMaxSpeed = %f", &inputFloat);
              pPhysics->MaxSpeed = inputFloat;
              continue;
            }

            if (myStrCmp(question, "Friction") <= 0)
            {
              sscanf(buffer, "\tFriction = %f", &inputFloat);
              pPhysics->Friction = inputFloat;
              continue;
            }

            if (myStrCmp(question, "Velocity") <= 0)
            {
              sscanf(buffer, "\tVelocity = (%f, %f)", &inputVector.x, &inputVector.y);
              pPhysics->Velocity = inputVector;
              continue;
            }

            if (myStrCmp(question, "Acceleration") <= 0)
            {
              sscanf(buffer, "\tAcceleration = (%f, %f)", &inputVector.x, &inputVector.y);
              pPhysics->Acceleration = inputVector;
              continue;
            }
          }

          if (pCurrComp->Type == Collider)
          {
            COLLIDER * pCollider = (COLLIDER*)pCurrComp->pStruct;

            if (myStrCmp(question, "Offset") <= 0)
            {
              sscanf(buffer, "\tOffset = (%f, %f)", &inputVector.x, &inputVector.y);
              pCollider->Offset = inputVector;
              continue;
            }

            if (myStrCmp(question, "Height") <= 0)
            {
              sscanf(buffer, "\tHeight = %f", &inputFloat);
              pCollider->Height = inputFloat;
              continue;
            }

            if (myStrCmp(question, "Width") <= 0)
            {
              sscanf(buffer, "\tWidth = %f", &inputFloat);
              pCollider->Width = inputFloat;
              continue;
            }

            if (myStrCmp(question, "IsGhosted") <= 0)
            {
              sscanf(buffer, "\tIsGhosted = %i", &inputInt);
              pCollider->IsGhosted = inputInt;
              continue;
            }
          }

          if (pCurrComp->Type == KSound)
          {
            KSOUND * pSound = (KSOUND*)pCurrComp->pStruct;

            if (myStrCmp(question, "Volume") <= 0)
            {
              sscanf(buffer, "\tVolume = %f", &inputFloat);
              pSound->Volume = inputFloat;
              continue;
            }

            if (myStrCmp(question, "Positional") <= 0)
            {
              sscanf(buffer, "\tPositional = %i", &inputInt);
              pSound->Positional = inputInt;
              continue;
            }

            if (myStrCmp(question, "MaxReach") <= 0)
            {
              sscanf(buffer, "\tMaxReach = %f", &inputFloat);
              pSound->MaxReach = inputFloat;
              continue;
            }

            if (myStrCmp(question, "SoundFile") <= 0)
            {
              char scriptInput[MAX_LENGTH];
              sscanf(buffer, "\tSoundFile = %s", &scriptInput);
              pSound->SoundFile = scriptInput;
              continue;
            }

            if (myStrCmp(question, "PlayOnStart") <= 0)
            {
              sscanf(buffer, "\tPlayOnStart = %i", &inputInt);
              pSound->PlayOnStart = inputInt;
              continue;
            }

			// TODO : Add the rest of the params.
          }

          if (pCurrComp->Type == Sprite)
          {
            SPRITE * pSprite = (SPRITE*)pCurrComp->pStruct;
            if (myStrCmp(question, "TextureFile") <= 0)
            {
			        int temp;
              char textureInput[MAX_LENGTH];
              sscanf(buffer, "\tTextureFile = %i , ", &temp);
			        MultipleAnimations(buffer, temp, pSprite);
              continue;
            }

            if (myStrCmp(question, "Animated") <= 0)
            {
              sscanf(buffer, "\tAnimated = %i", &inputInt);
              pSprite->Animated = inputInt;
              continue;
            }

            if (myStrCmp(question, "RowCol") <= 0)
            {
              sscanf(buffer, "\tRowCol = (%f, %f)", &inputVector.x, &inputVector.y);
              pSprite->RowCol = inputVector;
              continue;
            }

            if (myStrCmp(question, "Offset") <= 0)
            {
              sscanf(buffer, "\tOffset = (%f, %f)", &inputVector.x, &inputVector.y);
              pSprite->Offset = inputVector;
              continue;
            }

            if (myStrCmp(question, "AnimationSpeed") <= 0)
            {
              sscanf(buffer, "\tAnimationSpeed = %f", &inputFloat);
              pSprite->AnimationSpeed = inputFloat;
              continue;
            }
          }

          if (myStrCmp(question, "EndComponent") <= 0)
          {
            pCurrComp = NULL;
            continue;
          }
        }
      }
    }
  }
}

int myStrCmp(char * Input, char * Answer)
{
  int i = 0;
  
  int closeness = strlen(Answer);
  for (i = 0; i < strlen(Input); i++)
  {
    if (toupper(Input[i]) == toupper(Answer[i]))
    {
      closeness--;
    }
  }
  if (strlen(Input) != strlen(Answer) && closeness <= 0)
  {
    return 1;
  }
  return closeness;
}

char * myStrCpy(char * theString)
{
  char * newString = malloc(strlen(theString));
  int i = 0;
  for (i = 0; i < strlen(theString); i++)
  {
    newString[i] = theString[i];
  }
  newString[strlen(theString)] = '\0';
  return newString;
}

void InterpretLevelFiles()
{
  FILE * fpLevelList = NULL;
  FILE * fpCurLevel = NULL;
  char buffer[MAX_LENGTH];

  fpLevelList = fopen("_LevelList.txt", "r");
  if (fpLevelList)
  {
    while (!feof(fpLevelList))
    {
      if (fgets(buffer, MAX_LENGTH, fpLevelList) && buffer[0] != '#' && strlen(buffer) >= 2)
      {
        AddNull(buffer);
        fpCurLevel = fopen(buffer, "r");
        if (fpCurLevel)
        {
          InterpretLevel(fpCurLevel);
          fclose(fpCurLevel);
        }
      }
    }

    fclose(fpLevelList);
  }
  
}

void InterpretLevel(FILE * fpLevel)
{
  char buffer[MAX_LENGTH];
  LEVEL * pNewLevel = malloc(sizeof(LEVEL));
  UNIT * pCurrUnit = NULL;
  pNewLevel->Name = "Untitled";
  pNewLevel->Order = 0;
  pNewLevel->pCamera = NULL;
  pNewLevel->nextUnit = NULL;
  pNewLevel->pGame = pTheGame;
  pNewLevel->nextLevel = pTheGame->nextLevel;
  pTheGame->nextLevel = pNewLevel;
  while (!feof(fpLevel))
  {
    if (fgets(buffer, MAX_LENGTH, fpLevel))
    {
      char question[MAX_LENGTH];
      int inputInt = 0;
      float inputFloat = 0.0f;
      VECTOR inputVector = NewVector(0, 0);
     
      AddNull(buffer);
      if (buffer[0] != '#' && strlen(buffer) > 2)
      {
        sscanf(buffer, "%s", &question);
        if ((myStrCmp(question, "Name") <= 0) && pCurrUnit == NULL)
        {
          char nameInput[MAX_LENGTH];
          sscanf(buffer, "Name = %s", nameInput);
          pNewLevel->Name = myStrCpy(nameInput);
          continue;
        }
        
        if (myStrCmp(question, "Order") <= 0)
        {
          char nameInput[MAX_LENGTH];
          sscanf(buffer, "Order = %i", &inputInt);
          pNewLevel->Order = inputInt;
          continue;
        }
        
        if (myStrCmp(question, "UNIT") <= 0)
        {
          char archInput[MAX_LENGTH];
          char nameInput[MAX_LENGTH];
          ARCHETYPE * pArchetype = NULL;
          sscanf(buffer, "UNIT < %s > %s", &archInput, &nameInput);
          pArchetype = FindArchetypeByName(pTheGame, archInput);
          pCurrUnit = AddUnit(pNewLevel, pArchetype, myStrCpy(nameInput));
          continue;
        }
        
        if (pCurrUnit)
        {
          if (myStrCmp(question, "Tag") <= 0)
          {
            char tagInput[MAX_LENGTH];
            sscanf(buffer, "\tTag = %s", tagInput);
            pCurrUnit->Tag = GetTagFromString(tagInput);
            continue;
          }

          if (myStrCmp(question, "InitialPosition") <= 0)
          {
            sscanf(buffer, "\tInitialPosition = (%f, %f)", &inputVector.x, &inputVector.y);
            pCurrUnit->pInitTransform->Position = inputVector;
            continue;
          }

          if (myStrCmp(question, "InitialRotation") <= 0)
          {
            sscanf(buffer, "\tInitialRotation = %f", &inputFloat);
            pCurrUnit->pInitTransform->Rotation = inputFloat;
            continue;
          }

          if (myStrCmp(question, "InitialScale") <= 0)
          {
            sscanf(buffer, "\tInitialScale = (%f, %f)", &inputVector.x, &inputVector.y);
            pCurrUnit->pInitTransform->Scale = inputVector;
            continue;
          }

          if (myStrCmp(question, "VAR") <= 0)
          {
            VTYPE theType;
            VAR * newVar;
            char varInput[MAX_LENGTH];
            char typeInput[MAX_LENGTH];
            char dataInput[MAX_LENGTH];
            void * data;
            sscanf(buffer, "\tVAR %s : %s = %s", &varInput, &typeInput, &dataInput);
            theType = GetVTypeFromString(typeInput);

            newVar = AddUnitVar(theType, varInput, pCurrUnit);

            if (theType == Float)
            {
              float x;
              sscanf(dataInput, "%f", &x);
              data = malloc(sizeof(x));
              *((float*)newVar->Data) = x;
            }
            else if (theType == Int)
            {
              int x;
              sscanf(dataInput, "%i", &x);
              data = malloc(sizeof(x));
              *(int*)newVar->Data = x;
            }
            else if (theType == Vector)
            {
              VECTOR x;
              sscanf(dataInput, "(%f, %f)", &x.x, &x.y);
              data = malloc(sizeof(x));
              *(VECTOR*)newVar->Data = x;
            }
            else if (theType == String)
            {
              char * x;
              x = myStrCpy(dataInput);
              data = malloc(sizeof(x));
              *(char**)newVar->Data = x;
            }
            else if (theType == Bool)
            {
              BOOL x;
              sscanf(dataInput, "%i", &x);
              data = malloc(sizeof(x));
              *(BOOL*)newVar->Data = x;
            }
            else if (theType == Color)
            {
              COLOR x;
              sscanf(dataInput, "(%f, %f, %f, %f)", &x.r, &x.g, &x.b, &x.a);
              data = malloc(sizeof(x));
              *(COLOR*)newVar->Data = x;
            }
            else if (theType == Char)
            {
              char x;
              sscanf(dataInput, "%c", &x);
              data = malloc(sizeof(x));
              *(char*)newVar->Data = x;
            }
            else if (theType == Matrix)
            {
              MATRIX x;
              sscanf(dataInput, "{ {%f, %f, %f} {%f, %f, %f} {%f, %f, %f} }", 
                            &x.m[0][0], &x.m[0][1], &x.m[0][2], 
                            &x.m[1][0], &x.m[1][1], &x.m[1][2],
                            &x.m[2][0], &x.m[2][1], &x.m[2][2]);
              data = malloc(sizeof(x));
              *(MATRIX*)newVar->Data = x;
            }
            
            
          }
        }
        

        if (myStrCmp(question, "EndUnit") <= 0)
        {

            pCurrUnit = NULL;
            continue;
        }
        
      }
    }
  }
}

void MultipleAnimations(char * Buffer, int NumberOfAnimations, SPRITE * pSprite)
{
	int useless;
	char textureInput1[MAX_LENGTH];
	char textureInput2[MAX_LENGTH];
	char textureInput3[MAX_LENGTH];
	char textureInput4[MAX_LENGTH];
	char textureInput5[MAX_LENGTH];

	switch (NumberOfAnimations) {
	case 1:
		sscanf(Buffer, "\tTextureFile = %i , %s", &useless, &textureInput1);
		pSprite->pImage->TextureFile = myStrCpy(textureInput1);
		break;
	case 2:
		sscanf(Buffer, "\tTextureFile = %i , %s , %s", &useless,  &textureInput1, &textureInput2);
		pSprite->pImage->TextureFile = myStrCpy(textureInput1);
		pSprite->pImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pTexture = myStrCpy(textureInput2);
		pSprite->pImage->pNextImage->pNextImage = NULL;
		break;
	case 3:
		sscanf(Buffer, "\tTextureFile = %i , %s , %s , %s", &useless, &textureInput1, &textureInput2, &textureInput3);
		pSprite->pImage->TextureFile = myStrCpy(textureInput1);
		pSprite->pImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pTexture = myStrCpy(textureInput2);
		pSprite->pImage->pNextImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pNextImage->pTexture = myStrCpy(textureInput3);
		pSprite->pImage->pNextImage->pNextImage->pNextImage = NULL;
		break;
	case 4:
		sscanf(Buffer, "\tTextureFile = %i , %s , %s , %s , %s", &useless, &textureInput1, &textureInput2, &textureInput3, &textureInput4);
		pSprite->pImage->TextureFile = myStrCpy(textureInput1);
		pSprite->pImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pTexture = myStrCpy(textureInput2);
		pSprite->pImage->pNextImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pNextImage->pTexture = myStrCpy(textureInput3);
		pSprite->pImage->pNextImage->pNextImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pNextImage->pNextImage->pTexture = myStrCpy(textureInput4);
		pSprite->pImage->pNextImage->pNextImage->pNextImage->pNextImage = NULL;
		break;
	case 5:
		sscanf(Buffer, "\tTextureFile = %i , %s , %s , %s , %s , %s", &useless, &textureInput1, &textureInput2, &textureInput3, &textureInput4, &textureInput5);
		pSprite->pImage->TextureFile = myStrCpy(textureInput1);
		pSprite->pImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pTexture = myStrCpy(textureInput2);
		pSprite->pImage->pNextImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pNextImage->pTexture = myStrCpy(textureInput3);
		pSprite->pImage->pNextImage->pNextImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pNextImage->pNextImage->pTexture = myStrCpy(textureInput4);
		pSprite->pImage->pNextImage->pNextImage->pNextImage->pNextImage = malloc(sizeof(IMAGE));
		pSprite->pImage->pNextImage->pNextImage->pNextImage->pNextImage->pTexture = myStrCpy(textureInput5);
		pSprite->pImage->pNextImage->pNextImage->pNextImage->pNextImage->pNextImage = NULL;
		break;
	default:
		sscanf(Buffer, " %s", &textureInput1);
		pSprite->pImage->TextureFile = myStrCpy(textureInput1);
	}

}

VTYPE GetVTypeFromString(char * theString)
{
  if (myStrCmp(theString, "Float") <= 0)
  {
    return Float;
  }
  if (myStrCmp(theString, "Int") <= 0)
  {
    return Int;
  }
  if (myStrCmp(theString, "Vector") <= 0)
  {
    return Vector;
  }
  if (myStrCmp(theString, "String") <= 0)
  {
    return String;
  }
  if (myStrCmp(theString, "Bool") <= 0)
  {
    return Bool;
  }
  if (myStrCmp(theString, "Color") <= 0)
  {
    return Color;
  }
  if (myStrCmp(theString, "Char") <= 0)
  {
    return Char;
  }
  if (myStrCmp(theString, "Matrix") <= 0)
  {
    return Matrix;
  }
}

TAG GetTagFromString(char * String)
{
  if (myStrCmp(String, "DEFUALT") <= 0)
  {
    return DEFAULT;
  }
  else if (myStrCmp(String, "PLAYER") <= 0)
  {
    return PLAYER;
  }
  else if (myStrCmp(String, "BAD") <= 0)
  {
    return BAD;
  }
  else if (myStrCmp(String, "ENEMY") <= 0)
  {
    return ENEMY;
  }
  else if (myStrCmp(String, "WALL") <= 0)
  {
    return WALL;
  }
}
