#include "Collision.h"

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
