#ifndef VECTOR2_H
#define VECTOR2_H

#include "math.h"
#include "SoulEngine.h"

/*
This function sets the coordinates of the 2D vector (pResult) to 0
*/
void Vector2DZero(VECTOR *pResult);

/*
This function sets the coordinates of the 2D vector (pResult) to x &y
*/
void Vector2DSet(VECTOR *pResult, float x, float y);

/*
In this function, pResult will be set to the opposite of pVec0
*/
void Vector2DNeg(VECTOR *pResult, VECTOR *pVec0);

/*
In this function, pResult will be the sum of pVec0 and pVec1
*/
void Vector2DAdd(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1);

/*
In this function, pResult will be the difference between pVec0 *pVec1: pVec0 - pVec1
*/
void Vector2DSub(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1);

/*
In this function, pResult will be the unit vector of pVec0
*/
void Vector2DNormalize(VECTOR *pResult, VECTOR *pVec0);

/*
In this function, pResult will be the vector pVec0 scaled by the value c
*/
void Vector2DScale(VECTOR *pResult, VECTOR *pVec0, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and added to pVec1
*/
void Vector2DScaleAdd(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it
*/
void Vector2DScaleSub(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1, float c);

/*
This function returns the length of the vector pVec0
*/
float Vector2DLength(VECTOR *pVec0);

/*
This function returns the square of pVec0's length. Avoid the square root
*/
float Vector2DSquareLength(VECTOR *pVec0);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The distance between these 2 2D points is returned
*/
float Vector2DDistance(VECTOR *pVec0, VECTOR *pVec1);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The squared distance between these 2 2D points is returned. Avoid the square root
*/
float Vector2DSquareDistance(VECTOR *pVec0, VECTOR *pVec1);

/*
This function returns the dot product between pVec0 and pVec1
*/
float Vector2DDotProduct(VECTOR *pVec0, VECTOR *pVec1);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees
*/
void Vector2DFromAngleDeg(VECTOR *pResult, float angle);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Radian
*/
void Vector2DFromAngleRad(VECTOR *pResult, float angle);

/*
This function gets the normal of the vector pVec and stores the result in pResult
*/
void GetNormalVec(VECTOR *pResult, const VECTOR *pVec);

/*
This function gets the normal of the vector between pPt0 and pPt1 and stores the result in pResult
*/
void GetNormal2Pts(VECTOR *pResult, VECTOR *pPt0, VECTOR *pPt1);

/*
This function projects pVec1 onto pVec0 and stores the result in pResult
*/
void Project(VECTOR *pResult, VECTOR *pVec0, VECTOR *pVec1);

#endif