#include "MatrixMath.h"

//#define PI      3.1415926535897932384626433832795

// ---------------------------------------------------------------------------

void Matrix2DIdentity(Matrix2D *pResult)
{
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;

}

// ---------------------------------------------------------------------------

void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	pResult->m[0][0] = pMtx->m[0][0];
	pResult->m[1][1] = pMtx->m[1][1];
	pResult->m[2][2] = pMtx->m[2][2];

	pResult->m[0][1] = pMtx->m[1][0];
	pResult->m[0][2] = pMtx->m[2][0];
	pResult->m[1][2] = pMtx->m[2][1];

	pResult->m[1][0] = pMtx->m[0][1];
	pResult->m[2][0] = pMtx->m[0][2];
	pResult->m[2][1] = pMtx->m[1][2];

}

// ---------------------------------------------------------------------------

void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	float a1 = pMtx0->m[0][0];
	float a2 = pMtx0->m[0][1];
	float a3 = pMtx0->m[0][2];
	float b1 = pMtx0->m[1][0];
	float b2 = pMtx0->m[1][1];
	float b3 = pMtx0->m[1][2];
	float c1 = pMtx0->m[2][0];
	float c2 = pMtx0->m[2][1];
	float c3 = pMtx0->m[2][2];

	float d1 = pMtx1->m[0][0];
	float d2 = pMtx1->m[0][1];
	float d3 = pMtx1->m[0][2];
	float e1 = pMtx1->m[1][0];
	float e2 = pMtx1->m[1][1];
	float e3 = pMtx1->m[1][2];
	float f1 = pMtx1->m[2][0];
	float f2 = pMtx1->m[2][1];
	float f3 = pMtx1->m[2][2];


	pResult->m[0][0] = a1 * d1 + a2 * e1 + a3 * f1;
	pResult->m[0][1] = a1 * d2 + a2 * e2 + a3 * f2;
	pResult->m[0][2] = a1 * d3 + a2 * e3 + a3 * f3;

	pResult->m[1][0] = b1 * d1 + b2 * e1 + b3 * f1;
	pResult->m[1][1] = b1 * d2 + b2 * e2 + b3 * f2;
	pResult->m[1][2] = b1 * d3 + b2 * e3 + b3 * f3;

	pResult->m[2][0] = c1 * d1 + c2 * e1 + c3 * f1;
	pResult->m[2][1] = c1 * d2 + c2 * e2 + c3 * f2;
	pResult->m[2][2] = c1 * d3 + c2 * e3 + c3 * f3;

}

// ---------------------------------------------------------------------------

void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][2] = x;
	pResult->m[1][2] = y;
}

// ---------------------------------------------------------------------------

void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
}

// ---------------------------------------------------------------------------

void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	float angleRad = Angle * (float)PI / 180;

	Matrix2DRotRad(pResult, angleRad);

}

// ---------------------------------------------------------------------------

void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);
	float c = (float)cos(Angle);
	float s = (float)sin(Angle);

	pResult->m[0][0] = c;
	pResult->m[0][1] = -s;
	pResult->m[1][0] = s;
	pResult->m[1][1] = c;
}

// ---------------------------------------------------------------------------

void Matrix2DMultVec(VECTOR *pResult, Matrix2D *pMtx, VECTOR *pVec)
{
	float x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2];
	float y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2];

	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------
