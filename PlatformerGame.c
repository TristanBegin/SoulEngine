#include "SoulEngine.h"
#include "PlatformerGame.h"
#include "Behaviors.h"
#include "FileInterpreter.h"

extern GAME * pTheGame;

void InitializePlatformerGame(void)
{

  pTheGame = InitializeGame("Platformer");
  InterpretArchetypeFiles();
  CreatePlayerArchetype(pTheGame);
  CreateAnimArchetype(pTheGame);
  CreateLevelZero(pTheGame);
}

ARCHETYPE * CreatePlayerArchetype(GAME * pGame)
{
  ARCHETYPE * paPlayer;
  paPlayer = CreateArchetype(pGame, "Player");
  AddComponent(paPlayer, Sprite);
  AddComponent(paPlayer, Mesh);
  AddBehaviorComponent(paPlayer, PlayerBehavior);

  return paPlayer;
}

ARCHETYPE * CreateAnimArchetype(GAME * pGame)
{
  ARCHETYPE * paAnim;
  COMPONENT * pComp;
  paAnim = CreateArchetype(pGame, "Anim");
  pComp = AddComponent(paAnim, Sprite);
  AddComponent(paAnim, Mesh);

  ((SPRITE*)pComp->pStruct)->pTexture = AEGfxTextureLoad("TestAnimation.png");

  return paAnim;
}

LEVEL * CreateLevelZero(GAME * pGame)
{
	LEVEL * pLevelZero = AddLevel(pGame, "Tutorial", 0);
	
	ARCHETYPE * pPlayerArchetype = FindArchetypeByName(pGame, "Player");
  ARCHETYPE * pPlayer2Archetype = FindArchetypeByName(pGame, "GoRight");

  ARCHETYPE * pAnimArchetype = FindArchetypeByName(pGame, "Anim");

	UNIT * Player = AddUnit(pLevelZero, pPlayerArchetype, "Player");

  UNIT * Player2 = AddUnit(pLevelZero, pPlayer2Archetype, "Player2");

  UNIT * Anim = AddUnit(pLevelZero, pAnimArchetype, "Anim");
  SPRITE * pPlayer2Sprite = (SPRITE*)FindComponentStruct(pPlayer2Archetype, Sprite);
//  BEHAVIOR * pPlayer2Behavior = (BEHAVIOR*)FindComponentStruct(pPlayer2Archetype, Behavior);
  pPlayer2Sprite->pTexture = AEGfxTextureLoad(pPlayer2Sprite->TextureFile);
  Player2->pInitTransform->Position.y = -2;
  Anim->pInitTransform->Position.x = -1;

  return pLevelZero;
}