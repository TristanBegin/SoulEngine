#include "SoulEngine.h"
#include "Behaviors.h"
#include "FileInterpreter.h"

void(*GetBehaviorFromString(char * String))(BEHAVIOR * Owner, char * Trigger)
{
  if (myStrCmp(String, "PlayerBehavior") <= 0) return PlayerBehavior;
  if (myStrCmp(String, "GoRightBehavior") <= 0) return GoRightBehavior;
  if (myStrCmp(String, "DefaultBehavior") <= 0) return GoRightBehavior;
  if (myStrCmp(String, "BulletBehavior") <= 0) return BulletBehavior;
}