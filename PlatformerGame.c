#include "SoulEngine.h"
#include "PlatformerGame.h"

extern GAME * pTheGame;

void InitializePlatformerGame(void)
{
  pTheGame = InitializeGame("Platformer");
  CreatePlayerArchetype(pTheGame);
  
  CreateLevelZero(pTheGame);
}

ARCHETYPE * CreatePlayerArchetype(GAME * pGame)
{
  ARCHETYPE * paPlayer;
  paPlayer = CreateArchetype(pGame, "Player");
  AddComponent(paPlayer, Sprite);
  AddComponent(paPlayer, Mesh);
  AddBehaviorComponent(paPlayer, PlayerUpdate);

  return paPlayer;
}

LEVEL * CreateLevelZero(GAME * pGame)
{
	LEVEL * pLevelZero = AddLevel(pGame, "Tutorial", 0);
	
	ARCHETYPE * pPlayerArchetype = FindArchetypeByName(pGame, "Player");

	UNIT * Player = AddUnit(pLevelZero, pPlayerArchetype, "Player");
	
	SPRITE * pSprite = (SPRITE *)FindComponentStruct(Player->pInitArchetype, Sprite);

  
}

void PlayerUpdate(BEHAVIOR * Owner, char * Trigger)
{
  if (Trigger == "Update")
  {
    Owner->pArchetype->pUnit->pTransform->Position.x += 1;
  }
}