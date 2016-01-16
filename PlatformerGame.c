#include "SoulEngine.h"
#include "PlatformerGame.h"
#include "Behaviors.h"

extern GAME * pTheGame;

void InitializePlatformerGame(void)
{

  pTheGame = InitializeGame("Platformer");
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

  ARCHETYPE * pAnimArchetype = FindArchetypeByName(pGame, "Anim");

	UNIT * Player = AddUnit(pLevelZero, pPlayerArchetype, "Player");

  UNIT * Anim = AddUnit(pLevelZero, pAnimArchetype, "Anim");

  Anim->pInitTransform->Position.x = -300;

  return pLevelZero;
}