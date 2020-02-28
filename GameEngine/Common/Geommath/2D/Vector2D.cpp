/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D
Purpose: implementation of Vector2D
Language: c++ 11
Platform: win32 x86
Project: CS529_project_2
Author: Hang Yu hang.yu 60001119
Creation date: 10/30/2019
- End Header ----------------------------*/

#include "Vector2D.hpp"

// ---------------------------------------------------------------------------

void My::Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

void My::Vector2DSet(Vector2D* pResult, Vector2D* pVec0)
{
	pResult->x = pVec0->x;
	pResult->y = pVec0->y;
}

void My::Vector2DSet(Vector2D* pResult, const Vector2D* pVec0)
{
	pResult->x = pVec0->x;
	pResult->y = pVec0->y;
}

// ---------------------------------------------------------------------------

void My::Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void My::Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
}

// ---------------------------------------------------------------------------

void My::Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = (pVec0->x + pVec1->x);
	pResult->y = (pVec0->y + pVec1->y);
}

// ---------------------------------------------------------------------------

void My::Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = (pVec0->x - pVec1->x);
	pResult->y = (pVec0->y - pVec1->y);
}

// ---------------------------------------------------------------------------

void My::Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	float len_inverse = 1.0f / (Vector2DLength(pVec0) + EPSILON);
	pResult->x = pVec0->x * len_inverse;
	pResult->y = pVec0->y * len_inverse;
}

// ---------------------------------------------------------------------------

void My::Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------

void My::Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = (pVec0->x * c + pVec1->x);
	pResult->y = (pVec0->y * c + pVec1->y);
}

// ---------------------------------------------------------------------------

void My::Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = (pVec0->x * c - pVec1->x);
	pResult->y = (pVec0->y * c - pVec1->y);
}

// ---------------------------------------------------------------------------

float My::Vector2DLength(Vector2D *pVec0)
{
	return (pVec0) ? sqrtf(powf(pVec0->x,2) + powf(pVec0->y,2)) : F_UNDEFINE;
}

// ---------------------------------------------------------------------------

float My::Vector2DSquareLength(Vector2D *pVec0)
{
	return (pVec0) ? (powf(pVec0->x, 2) + powf(pVec0->y, 2)) : F_UNDEFINE;
}

// ---------------------------------------------------------------------------

float My::Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0 && pVec1) ? sqrtf(powf(pVec0->x - pVec1->x, 2) + powf(pVec0->y - pVec1->y, 2)) : F_UNDEFINE;
}

// ---------------------------------------------------------------------------

float My::Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0 && pVec1) ? (powf(pVec0->x - pVec1->x, 2) + powf(pVec0->y - pVec1->y, 2)) : F_UNDEFINE;
}

// ---------------------------------------------------------------------------

float My::Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0 && pVec1) ? (pVec0->x * pVec1->x + pVec0->y * pVec1->y) : F_UNDEFINE;
}

// ---------------------------------------------------------------------------

void My::Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	Vector2DFromAngleRad(pResult, 0.01745329251f * angle);
}

// ---------------------------------------------------------------------------

void My::Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	float x = pResult->x * c - pResult->y * s;
	float y = pResult->x * s + pResult->y * c;
	pResult->x = x;
	pResult->y = y;
}

float My::Vector2DAngleFromVec2(Vector2D* pVec)
{
	return (pVec) ? atanf(pVec->y / pVec->x) : F_UNDEFINE;
}

namespace My
{
	Vector2D operator-(Vector2D& vec1, Vector2D& vec2)
	{
		Vector2D ret;
		Vector2DSub(&ret, &vec1, &vec2);
		return ret;
	}

	Vector2D operator+(Vector2D& vec1, Vector2D& vec2)
	{
		Vector2D ret;
		Vector2DAdd(&ret, &vec1, &vec2);
		return ret;
	}
}


// ---------------------------------------------------------------------------
