#include "SoulEngine.h"
#include "Behaviors.h"
#include <stdio.h>
#include "FileInterpreter.h"

#define MAX_LENGTH 50


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

        if (myStrCmp(question, "COMPONENT") <= 0)
        {
          COMPONENTTYPE theType;
          char typeInput[MAX_LENGTH];
          sscanf(buffer, "COMPONENT |%s|", &typeInput);
          if (myStrCmp(typeInput, "Sprite") <= 0) theType = Sprite;
          if (myStrCmp(typeInput, "Mesh") <= 0) theType = Mesh;
          if (myStrCmp(typeInput, "Behavior") <= 0) theType = Behavior;
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

          if (pCurrComp->Type == Sprite)
          {
            SPRITE * pSprite = (SPRITE*)pCurrComp->pStruct;
            if (myStrCmp(question, "TextureFile") <= 0)
            {
              char textureInput[MAX_LENGTH];
              sscanf(buffer, "\tTextureFile = %s", &textureInput);
              pSprite->TextureFile = myStrCpy(textureInput);
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
          sscanf(buffer, "UNIT <%s> %s", &archInput, &nameInput);
          pArchetype = FindArchetypeByName(pTheGame, archInput);
          OutputDebugString("Unit");
          pCurrUnit = AddUnit(pNewLevel, pArchetype, nameInput);
          continue;
        }
        
        if (pCurrUnit)
        {
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