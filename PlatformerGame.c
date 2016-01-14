#include "SoulEngine.h"
#include "PlatformerGame.h"

GAME * InitializePlatformerGame(void)
{
  GAME * pGame;
  pGame = InitializeGame("Platformer");
  CreatePlayerArchetype(pGame);
  
  CreateLevelZero(pGame);
}

ARCHETYPE * CreatePlayerArchetype(GAME * pGame)
{
  ARCHETYPE * paPlayer;
  paPlayer = CreateArchetype(pGame, "Player");
  AddComponent(paPlayer, Sprite);
  AddComponent(paPlayer, SquareMesh);

  return paPlayer;
}

LEVEL * CreateLevelZero(GAME * pGame)
{
	LEVEL * pLevelZero = AddLevel(pGame, "Tutorial", 0);
	
	ARCHETYPE * pPlayerArchetype = FindArchetypeByName(pGame, "Player");
	UNIT * Player = AddUnit(pLevelZero, pPlayerArchetype, "Player");
  AddBehaviorComponent(Player, PlayerUpdate);
}

void PlayerUpdate(BEHAVIOR * Owner, char * Trigger)
{
  if (Trigger == "Update")
  {
    Owner->pArchetype->pUnit->pTransform->Position.x += 1;
  }
}