#ifndef MATH2D_H
#define MATH2D_H

#include "SoulEngine.h"
#include "VectorMath.h"

/*
This function takes the current Unit and checks for collision with all other units
*/
void UpdateCollision(COLLIDER *pCollider);

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(VECTOR *pP, VECTOR *pCenter, float Radius);


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(VECTOR *pPos, VECTOR *pRect, float Width, float Height);

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(VECTOR *pCenter0, float Radius0, VECTOR *pCenter1, float Radius1);

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(VECTOR *pRect0, float Width0, float Height0, VECTOR *pRect1, float Width1, float Height1);


/*
This function checks the direction of collision for 2 rectangles
The direction will be based off of pRect0 (if 0 is above 1, it's a collision from below)
*/
int CollisionDirection(VECTOR *pRect0, float Width0, float Height0, VECTOR *pRect1, float Width1, float Height1, COLLIDER * pCollider);
#endif
