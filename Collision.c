#include "Collision.h"

// ---------------------------------------------------------------------------

void UpdateCollision(COLLIDER *pCollider)
{
	UNIT *tempUnit = pCollider->pArchetype->pUnit->pLevel->nextUnit;
	VECTOR pRect0 = pCollider->pArchetype->pUnit->pTransform->Position;
	float height0 = pCollider->Height;
	float width0 = pCollider->Width;
	//Adding the offset to get the world pos of pRect0
	pRect0.x += pCollider->Offset.x;
	pRect0.y += pCollider->Offset.y;

  pCollider->Grounded = False;
  pCollider->LeftBlocked = False;
  pCollider->RightBlocked = False;
  pCollider->TopBlocked = False;

	//Walk through the list of Units in the Level, checking for collisions with current collider
	while (tempUnit)
	{
    if (tempUnit->pArchetype)
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
			VECTOR pRect1 = tempCollider->pArchetype->pUnit->pTransform->Position;
			float height1 = tempCollider->Height;
			float width1 = tempCollider->Width;
			BOOL colResult;
			DIRECTION colDir;

			//Adding the offset to get the world pos of pRect1
			pRect1.x += tempCollider->Offset.x;
			pRect1.y += tempCollider->Offset.y;

			//Checking for collision between pRect0 and pRect1
			colResult = StaticRectToStaticRect(&pRect0, width0, height0, &pRect1, width1, height1);
            
			if (colResult)
			{
				if (!tempCollider->IsGhosted)
				{
					colDir = CollisionDirection(&pRect0, width0, height0, &pRect1, width1, height1);
					
					switch (colDir)
					{
						case Bottom:
							pCollider->Grounded = True;
							tempCollider->TopBlocked = True;
							break;
						case Top:
							pCollider->TopBlocked = True;
							tempCollider->Grounded = True;
							break;
						case Left:
							pCollider->LeftBlocked = True;
							tempCollider->RightBlocked = True;
							break;
						case Right:
							pCollider->RightBlocked = True;
							tempCollider->LeftBlocked = True;
							break;
					}
				}
			}
		}
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

	if (right0 < left1 || left0 > right1 && bottom0 > top1 || top0 < bottom1)
		return 0;
	else
		return 1;
}

// ---------------------------------------------------------------------------

int CollisionDirection(VECTOR *pRect0, float Width0, float Height0, VECTOR *pRect1, float Width1, float Height1)
{
	float left0 = pRect0->x - Width0 / 2;
	float right0 = pRect0->x + Width0 / 2;
	float bottom0 = pRect0->y - Height0 / 2;
	float top0 = pRect0->y + Height0 / 2;

	float left1 = pRect1->x - Width1 / 2;
	float right1 = pRect1->x + Width1 / 2;
	float bottom1 = pRect1->y - Height1 / 2;
	float top1 = pRect1->y + Height1 / 2;

	float bottomCol = bottom0 - top1;
	float topCol = top0 - bottom1;
	float leftCol = left0 - right1;
	float rightCol = right0 - right1;

	bottomCol = abs(10* bottomCol);
	topCol = abs(10 * topCol);
	leftCol = abs(10 * leftCol);
	rightCol = abs(10 * rightCol);

	if (bottomCol < topCol && bottomCol < leftCol && bottomCol < rightCol)
		return Bottom;
	else if (topCol < bottomCol && topCol < leftCol && topCol < rightCol)
		return Top;
	else if (leftCol < topCol && leftCol < bottomCol && leftCol < rightCol)
		return Left;
	else if (rightCol < topCol && rightCol < bottomCol && rightCol < leftCol)
		return Right;
}