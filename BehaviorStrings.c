#include "SoulEngine.h"
#include "Behaviors.h"

void(*GetBehaviorFromString(char * String))(BEHAVIOR * Owner, char * Trigger)
{
  if (String == "PlayerBehavior") return PlayerBehavior;
}