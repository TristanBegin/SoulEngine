#include "SoulMath.h"

/*
This function gets the normal of the vector pVec and stores the result in pResult
*/
void GetNormalVec(VECTOR *pResult, VECTOR *pVec)
{
	AEVec2Set(pResult, -pVec->y, pVec->x);
}

/*
This function gets the normal of the vector between pPt0 and pPt1 and stores the result in pResult
*/
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

/*
This function projects pVec1 onto pVec0 and stores the result in pResult
*/
void Project(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1)
{
	float scalar = AEVec2DotProduct(pVec0, pVec1) / (AEVec2Length(pVec0) * AEVec2Length(pVec1));
	float x = pVec0->x * scalar;
	float y = pVec0->y * scalar;

	AEVec2Set(pResult, x, y);
}
