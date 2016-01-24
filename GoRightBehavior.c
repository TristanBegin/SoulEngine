#include "SoulEngine.h"

#define LEVELSWITCH 18

static UNIT * pMyUnit;
static TRANSFORM * pMyTransform;
static ARCHETYPE * pMyArchetype;
static LEVEL * pMyLevel;
static GAME * pMyGame;
static GAMESTATS * pMyGameStats;
static BEHAVIOR * pMyBehavior;

void Start();
void Update();

void GoRightBehavior(BEHAVIOR * Owner, char * Trigger)
{
  pMyUnit = Owner->pArchetype->pUnit;
  pMyTransform = pMyUnit->pTransform;
  pMyArchetype = pMyUnit->pArchetype;
  pMyLevel = pMyUnit->pLevel;
  pMyGame = pMyLevel->pGame;
  pMyGameStats = pMyGame->pGameStats;
  pMyBehavior = Owner;

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
   (float*)AddVar(Float, "exampleFloat", pMyBehavior);


  //AddVar(Vector, "exampleVector", pMyBehavior);

  AddVar(String, "exampleString", pMyBehavior);

}

static void Update()
{
  float * exampleFloat = GetVar("exampleFloat", pMyBehavior);

  //VECTOR * exampleVector = GetVar("exampleVector", pMyBehavior);

  //char ** exampleString = GetVar("exampleString", pMyBehavior);
  //*exampleString = "Hello World";

  pMyTransform->Position.x += *exampleFloat;

  //if (pMyTransform->Position.x > LEVELSWITCH)
	  //pMyGame->pGameStats->nextLevel = 1;


}