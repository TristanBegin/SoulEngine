void(*GetBehaviorFromString(char * String))(BEHAVIOR * Owner, char * Trigger, void * Data);
void PlayerBehavior(BEHAVIOR * Owner, char * Trigger, void * Data);
void GoRightBehavior(BEHAVIOR * Owner, char * Trigger, void * Data);
void DefaultBehavior(BEHAVIOR * Owner, char * Trigger, void * Data);
void BulletBehavior(BEHAVIOR * Owner, char * Trigger, void * Data);
void EnemyBehavior(BEHAVIOR * Owner, char * Trigger, void * Data);

#pragma once
