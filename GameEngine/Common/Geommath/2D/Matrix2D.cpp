/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D
Purpose: implementation of Matrix2D
Language: c++ 11
Platform: win32 x86
Project: CS529_project_2
Author: Hang Yu hang.yu 60001119
Creation date: 10/03/2019
- End Header ----------------------------*/

#include "Matrix2D.hpp"

/*
This function sets the matrix Result to the identity matrix
*/
void My::Matrix2DIdentity(Matrix2D* pResult)
{
	if (pResult)
	{						
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				pResult->m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
	}
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void My::Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	if (pResult && pMtx)
	{
		// Not the same pointer, just swap
		if (pResult != pMtx)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					pResult->m[i][j] = pMtx->m[j][i];
				}
			}
		}
		// The same pointer, needs a temporal matrix to store values
		else
		{
			Matrix2D mat;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					mat.m[i][j] = pMtx->m[j][i];
				}
			}
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					pResult->m[i][j] = mat.m[i][j];
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void My::Matrix2DConcat(Matrix2D* pResult, const Matrix2D* pMtx0, const Matrix2D* pMtx1)
{
	if (pResult && pMtx0 && pMtx1)
	{
		Matrix2D mat;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				mat.m[i][j] = pMtx0->m[i][0] * pMtx1->m[0][j] \
								+ pMtx0->m[i][1] * pMtx1->m[1][j] \
								+ pMtx0->m[i][2] * pMtx1->m[2][j];
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				pResult->m[i][j] = mat.m[i][j];
			}
		}
	}
}

void My::Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1)
{
	if (pResult && pMtx0 && pMtx1)
	{
		Matrix2D mat;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				mat.m[i][j] = pMtx0->m[i][0] * pMtx1->m[0][j] \
					+ pMtx0->m[i][1] * pMtx1->m[1][j] \
					+ pMtx0->m[i][2] * pMtx1->m[2][j];
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				pResult->m[i][j] = mat.m[i][j];
			}
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void My::Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	if (pResult)
	{
		Matrix2DIdentity(pResult);
		pResult->m[0][2] = x;
		pResult->m[1][2] = y;
	}
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void My::Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	if (pResult)
	{
		Matrix2DIdentity(pResult);
		pResult->m[0][0] = x;
		pResult->m[1][1] = y;
	}
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void My::Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	if (pResult)
	{
		Angle *= ((float)PI / 180.0f);
		float c = cosf(Angle);
		float s = sinf(Angle);
		Matrix2DIdentity(pResult);
		pResult->m[0][0] = c;
		pResult->m[0][1] = -s;
		pResult->m[1][0] = s;
		pResult->m[1][1] = c;
	}
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void My::Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	if (pResult)
	{
		float c = cosf(Angle);
		float s = sinf(Angle);
		Matrix2DIdentity(pResult);
		pResult->m[0][0] = c;
		pResult->m[0][1] = -s;
		pResult->m[1][0] = s;
		pResult->m[1][1] = c;
	}
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void My::Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	if (pResult && pMtx && pVec)
	{
		// pVec has w coord as 1.0f, which can be ignored as a translation multiplier.
		float x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2];
		float y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2];
		Vector2DSet(pResult, x, y);
	}
}

void My::Matrix2DMultVec(Vector2D* pResult, const Matrix2D* pMtx, const Vector2D* pVec)
{
	if (pResult && pMtx && pVec)
	{
		// pVec has w coord as 1.0f, which can be ignored as a translation multiplier.
		float x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2];
		float y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2];
		Vector2DSet(pResult, x, y);
	}
}

My::Matrix2D operator*(const My::Matrix2D& mat1, const My::Matrix2D& mat2)
{
	My::Matrix2D ret;
	Matrix2DConcat(&ret, &mat1, &mat2);
	return ret;
}

My::Vector2D operator*(const My::Matrix2D& mat1, const My::Vector2D& mat2)
{
	My::Vector2D ret;
	Matrix2DMultVec(&ret, &mat1, &mat2);
	return ret;
}

// ---------------------------------------------------------------------------
