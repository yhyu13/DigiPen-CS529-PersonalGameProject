/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D
Purpose: header file of Vector2D
Language: c++ 11
Platform: win32 x86
Project: CS529_project_2
Author: Hang Yu hang.yu 60001119
Creation date: 10/10/2019
- End Header ----------------------------*/

#pragma once
#include "Math.hpp"

namespace My {

	typedef struct Vector2D
	{
		float x, y;
		Vector2D() : x(0.0f), y(0.0f) {};
		Vector2D(float _x, float _y) : x(_x), y(_y) {};
		Vector2D(const Vector2D & vec) : x(vec.x), y(vec.y) {};
	}Vector2D;

	/*
	This function sets the coordinates of the 2D vector (pResult) to 0
	*/
	void Vector2DZero(Vector2D* pResult);

	/*
	This function sets the coordinates of the 2D vector (pResult) to pVec0
	*/
	void Vector2DSet(Vector2D* pResult, Vector2D* pVec0);

	/*
	This function sets the coordinates of the 2D vector (pResult) to pVec0
	*/
	void Vector2DSet(Vector2D* pResult, const Vector2D* pVec0);

	/*
	This function sets the coordinates of the 2D vector (pResult) to x &y
	*/
	void Vector2DSet(Vector2D* pResult, float x, float y);

	/*
	In this function, pResult will be set to the opposite of pVec0
	*/
	void Vector2DNeg(Vector2D* pResult, Vector2D* pVec0);

	/*
	In this function, pResult will be the sum of pVec0 and pVec1
	*/
	void Vector2DAdd(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1);

	/*
	In this function, pResult will be the difference between pVec0 *pVec1: pVec0 - pVec1
	*/
	void Vector2DSub(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1);

	/*
	In this function, pResult will be the unit vector of pVec0
	*/
	void Vector2DNormalize(Vector2D* pResult, Vector2D* pVec0);

	/*
	In this function, pResult will be the vector pVec0 scaled by the value c
	*/
	void Vector2DScale(Vector2D* pResult, Vector2D* pVec0, float c);

	/*
	In this function, pResult will be the vector pVec0 scaled by c and added to pVec1
	*/
	void Vector2DScaleAdd(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1, float c);

	/*
	In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it
	*/
	void Vector2DScaleSub(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1, float c);

	/*
	This function returns the length of the vector pVec0
	*/
	float Vector2DLength(Vector2D* pVec0);

	/*
	This function returns the square of pVec0's length. Avoid the square root
	*/
	float Vector2DSquareLength(Vector2D* pVec0);

	/*
	In this function, pVec0 and pVec1 are considered as 2D points.
	The distance between these 2 2D points is returned
	*/
	float Vector2DDistance(Vector2D* pVec0, Vector2D* pVec1);

	/*
	In this function, pVec0 and pVec1 are considered as 2D points.
	The squared distance between these 2 2D points is returned. Avoid the square root
	*/
	float Vector2DSquareDistance(Vector2D* pVec0, Vector2D* pVec1);

	/*
	This function returns the dot product between pVec0 and pVec1
	*/
	float Vector2DDotProduct(Vector2D* pVec0, Vector2D* pVec1);

	/*
	This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees
	*/
	void Vector2DFromAngleDeg(Vector2D* pResult, float angle);

	/*
	This function computes the coordinates of the vector represented by the angle "angle", which is in Radian
	*/
	void Vector2DFromAngleRad(Vector2D* pResult, float angle);

	/*
	This function computes the theta value of the vector in polar coordinate system in radians.
	*/
	float Vector2DAngleFromVec2(Vector2D* pVec);

	Vector2D operator-(Vector2D& vec1, Vector2D& vec2);
	Vector2D operator+(Vector2D& vec1, Vector2D& vec2);
}