#include "SoulEngine.h"

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;

void Start();
void Update();

void PlayerBehavior(BEHAVIOR * Owner, char * Trigger)
{
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;

  if (Trigger == "Start")
  {
    Start();
  }

  if (Trigger == "Update")
  {
    Update();
  }
}

static void Start()
{
  //pMyTransform->Position = NewVector(2, 2);

}

static void Update()
{
  if (AEInputCheckCurr(VK_SPACE))
  {
    pMyTransform->Position.x -= 0.1;
  }

}