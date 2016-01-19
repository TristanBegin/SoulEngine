#include "VectorMath.h"

#define PI      3.1415926535897932384626433832795

// ---------------------------------------------------------------------------

void Vector2DZero(VECTOR *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(VECTOR *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(VECTOR *pResult, VECTOR *pVec0)
{
	Vector2DSet(pResult, -pVec0->x, -pVec0->y);
}

// ---------------------------------------------------------------------------

void Vector2DAdd(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1)
{
	float x = pVec0->x + pVec1->x;
	float y = pVec0->y + pVec1->y;

	Vector2DSet(pResult, x, y);
}

// ---------------------------------------------------------------------------

void Vector2DSub(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1)
{
	float x = pVec0->x - pVec1->x;
	float y = pVec0->y - pVec1->y;

	Vector2DSet(pResult, x, y);
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(VECTOR *pResult, VECTOR *pVec0)
{
	float length = Vector2DLength(pVec0);
	float x = pVec0->x / length;
	float y = pVec0->y / length;

	Vector2DSet(pResult, x, y);
}

// ---------------------------------------------------------------------------

void Vector2DScale(VECTOR *pResult, VECTOR *pVec0, float c)
{
	float x = pVec0->x * c;
	float y = pVec0->y * c;

	Vector2DSet(pResult, x, y);
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1, float c)
{
	Vector2DScale(pResult, pVec0, c);
	Vector2DAdd(pResult, pResult, pVec1);
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1, float c)
{
	Vector2DScale(pResult, pVec0, c);
	Vector2DSub(pResult, pResult, pVec1);
}

// ---------------------------------------------------------------------------

float Vector2DLength(VECTOR *pVec0)
{
	float x = pVec0->x;
	float y = pVec0->y;
	float h = sqrt(x*x + y*y);

	return h;
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(VECTOR *pVec0)
{
	float x = pVec0->x;
	float y = pVec0->y;
	float h = x*x + y*y;

	return h;
}

// ---------------------------------------------------------------------------

float Vector2DDistance(VECTOR *pVec0, VECTOR *pVec1)
{
	float t = pVec0->x;
	float u = pVec0->y;
	float v = pVec1->x;
	float w = pVec1->y;

	float x = t - v;
	float y = u - w;

	float h = sqrt(x*x + y*y);

	return h;
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(VECTOR *pVec0, VECTOR *pVec1)
{
	float t = pVec0->x;
	float u = pVec0->y;
	float v = pVec1->x;
	float w = pVec1->y;

	float x = t - v;
	float y = u - w;

	float h = x*x + y*y;

	return h;
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(VECTOR *pVec0, VECTOR *pVec1)
{
	float t = pVec0->x;
	float u = pVec0->y;
	float v = pVec1->x;
	float w = pVec1->y;

	float h = t*v + u*w;

	return h;
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(VECTOR *pResult, float angle)
{
	float angleRad = angle * PI / 180;

	Vector2DFromAngleRad(pResult, angleRad);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(VECTOR *pResult, float angle)
{
	float x = sin(angle);
	float y = sin(angle);

	Vector2DSet(pResult, x, y);
}

// ---------------------------------------------------------------------------

void GetNormalVec(VECTOR *pResult, VECTOR *pVec)
{
	AEVec2Set(pResult, -pVec->y, pVec->x);
}

// ---------------------------------------------------------------------------

void GetNormal2Pts(VECTOR *pResult, VECTOR *pPt0, VECTOR *pPt1)
{
	float p1 = pPt0->x;
	float p2 = pPt0->y;
	float q1 = pPt1->x;
	float q2 = pPt1->y;
	VECTOR v;
	v.x = p1 - q1;
	v.y = p2 - q2;

	GetNormalVec(pResult, &v);
}

// ---------------------------------------------------------------------------

void Project(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1)
{
	float scalar = AEVec2DotProduct(pVec0, pVec1) / (AEVec2Length(pVec0) * AEVec2Length(pVec1));
	float x = pVec0->x * scalar;
	float y = pVec0->y * scalar;

	AEVec2Set(pResult, x, y);
}

// ---------------------------------------------------------------------------
