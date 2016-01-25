void(*GetBehaviorFromString(char * String))(BEHAVIOR * Owner, char * Trigger);
void PlayerBehavior(BEHAVIOR * Owner, char * Trigger);
void GoRightBehavior(BEHAVIOR * Owner, char * Trigger);
void DefaultBehavior(BEHAVIOR * Owner, char * Trigger);
void BulletBehavior(BEHAVIOR * Owner, char * Trigger);
//void EnemyBehavior(BEHAVIOR * Owner, char * Trigger);

#pragma once
