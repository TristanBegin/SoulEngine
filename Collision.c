#include "Collision.h"

// ---------------------------------------------------------------------------

void UpdateCollision(COLLIDER *pCollider)
{
	UNIT *tempUnit = pCollider->pArchetype->pUnit->nextUnit;
	VECTOR pRect0 = pCollider->Offset;
	float height0 = pCollider->Height;
	float width0 = pCollider->Width;

	//Walk through the list of Units in the Level, checking for collisions with current collider
	while (tempUnit)
	{
		COMPONENT *tempComp = tempUnit->pArchetype->nextComponent;
		COLLIDER *tempCollider = NULL;

		//Search for a Collider on the current Unit
		while (tempComp)
		{
			//If we find a Collider, store it and break out
			if (tempComp->Type == Collider)
			{
				tempCollider = (COLLIDER *)tempComp->pStruct;
				break;
			}
			tempComp = tempComp->nextComponent;
		}

		//If the Unit has a Collider, check for collision with current Collider
		if (tempCollider)
		{
			VECTOR pRect1 = tempCollider->Offset;
			float height1 = tempCollider->Height;
			float width1 = tempCollider->Width;

			StaticRectToStaticRect(&pRect0, width0, height0, &pRect1, height1, width1);
		}

		tempUnit = tempUnit->nextUnit;
	}
}

// ---------------------------------------------------------------------------

int StaticPointToStaticCircle(VECTOR *pP, VECTOR *pCenter, float Radius)
{
	float sqrDistance = Vector2DSquareDistance(pP, pCenter);

	if (sqrDistance > Radius * Radius)
		return 0;
	else
		return 1;
}

// ---------------------------------------------------------------------------

int StaticPointToStaticRect(VECTOR *pPos, VECTOR *pRect, float Width, float Height)
{
	float x = pPos->x;
	float y = pPos->y;

	float l = pRect->x - Width / 2;
	float r = pRect->x + Width / 2;
	float b = pRect->y - Height / 2;
	float t = pRect->y + Height / 2;

	if (x < l || x > r || x > t || x < b)
		return 0;
	else
		return 1;
}

// ---------------------------------------------------------------------------

int StaticCircleToStaticCircle(VECTOR *pCenter0, float Radius0, VECTOR *pCenter1, float Radius1)
{
	float sqrDistance = Vector2DSquareDistance(pCenter0, pCenter1);

	if (sqrDistance > (Radius0 + Radius1) * (Radius0 + Radius1))
		return 0;
	else
		return 1;
}

// ---------------------------------------------------------------------------

int StaticRectToStaticRect(VECTOR *pRect0, float Width0, float Height0, VECTOR *pRect1, float Width1, float Height1)
{
	float left0 = pRect0->x - Width0 / 2;
	float right0 = pRect0->x + Width0 / 2;
	float bottom0 = pRect0->y - Height0 / 2;
	float top0 = pRect0->y + Height0 / 2;

	float left1 = pRect1->x - Width1 / 2;
	float right1 = pRect1->x + Width1 / 2;
	float bottom1 = pRect1->y - Height1 / 2;
	float top1 = pRect1->y + Height1 / 2;

	if (right0 < left1 || left0 > right1 || bottom0 > top1 || top0 < bottom1)
		return 0;
	else
		return 1;
}

// ---------------------------------------------------------------------------
