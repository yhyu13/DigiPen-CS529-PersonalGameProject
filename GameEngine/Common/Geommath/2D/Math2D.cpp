/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D
Purpose: implementation of Math2D
Language: c++ 11
Platform: win32 x86
Project: CS529_project_2
Author: Hang Yu hang.yu 60001119
Creation date: 10/03/2019
- End Header ----------------------------*/

#include "Math2D.hpp"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int My::StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	if (pP && pCenter && Radius > 0.0f)
	{
		return (Vector2DSquareDistance(pP, pCenter) <= powf(Radius, 2)) ? 1 : 0;
	}
	else
	{
		return 0;
	}
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int My::StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	if (pPos && pRect && Width > 0.0f && Height > 0.0f)
	{
		Vector2D vec;
		Vector2DSub(&vec, pPos, pRect);
		return (fabs(vec.x) <= (Width / 2.0f) && fabs(vec.y) <= (Height / 2.0f)) ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int My::StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	if (pCenter0 && pCenter1 && Radius0 > 0.0f && Radius1 > 0.0f)
	{
		return (Vector2DSquareDistance(pCenter0, pCenter1) <= powf(Radius0 + Radius1, 2)) ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int My::StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	if (pRect0 && pRect1 && Width0 > 0.0f && Height0 > 0.0f && Width1 > 0.0f && Height1 > 0.0f)
	{
		Vector2D vec;
		Vector2DSub(&vec, pRect0, pRect1);
		return (fabs(vec.x) <= ((Width0 + Width1) / 2.0f) && fabs(vec.y) <= ((Height0 + Height1) / 2.0f)) ? 1 : 0;
	}
	else
	{
		return 0;
	}
}


//////////////////////
// New to project 2 //
//////////////////////


/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float My::StaticPointToStaticLineSegment(Vector2D* P, LineSegment2D* LS)
{
	return (P && LS) ? Vector2DDotProduct(&(LS->mN), P) - LS->mNdotP0 : -F_UNDEFINE;
}

/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::AnimatedPointToStaticLineSegment(Vector2D* Ps, Vector2D* Pe, LineSegment2D* LS, Vector2D* Pi)
{
	if (Ps && Pe && LS && Pi)
	{
		float t, dot;
		Vector2D v;
		Vector2DSub(&v, Pe, Ps);
		dot = Vector2DDotProduct(&(LS->mN), &v);
		if (dot != 0.0f)							// Parallel line
		{
			t = (LS->mNdotP0 - Vector2DDotProduct(&(LS->mN), Ps)) / dot;
			if (t >= 0.0f && t <= 1.0f)				// Intersection is on the line segment of [Ps, Pe]
			{
				Vector2D Pi_P1;
				Vector2D P0_P1;
				Vector2D Pi_P0;
				Vector2D P1_P0;
				Vector2DScaleAdd(Pi, &v, Ps, t);
				Vector2DSub(&Pi_P1, Pi, &(LS->mP1));
				Vector2DSub(&P0_P1, &(LS->mP0), &(LS->mP1));
				Vector2DSub(&Pi_P0, Pi, &(LS->mP0));
				Vector2DSub(&P1_P0, &(LS->mP1), &(LS->mP0));
				if (Vector2DDotProduct(&Pi_P1, &P0_P1) >= 0.0f && Vector2DDotProduct(&Pi_P0, &P1_P0) >= 0.0f)	// Intersection is on the line segment of LS
				{
					return t;
				}
				else
				{
					return -1.0f;
				}
			}
			else
			{
				return -1.0f;
			}
		}
		else
		{
			return -1.0f;
		}
	}
	else
	{
		return -F_UNDEFINE;
	}
}


/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::AnimatedCircleToStaticLineSegment(Vector2D* Ps, Vector2D* Pe, float Radius, LineSegment2D* LS, Vector2D* Pi)
{

	if (Ps && Pe && LS && Pi && Radius > 0.0f)
	{
		float D = StaticPointToStaticLineSegment(Ps, LS);
		if (D > 0)					// Ps is outside
		{
			D = Radius;
		}
		else						// Ps is inside
		{
			D = -Radius;
		}
		float t, dot;
		Vector2D v;
		Vector2DSub(&v, Pe, Ps);
		dot = Vector2DDotProduct(&(LS->mN), &v);
		if (dot != 0.0f)							// Parallel line
		{
			t = (LS->mNdotP0 - Vector2DDotProduct(&(LS->mN), Ps) + D) / dot;
			if (t >= 0.0f && t <= 1.0f)				// Intersection is on the line segment of [Ps, Pe]
			{
				Vector2D Pi_P1;
				Vector2D P0_P1;
				Vector2D Pi_P0;
				Vector2D P1_P0;
				Vector2DScaleAdd(Pi, &v, Ps, t);
				Vector2DSub(&Pi_P1, Pi, &(LS->mP1));
				Vector2DSub(&P0_P1, &(LS->mP0), &(LS->mP1));
				Vector2DSub(&Pi_P0, Pi, &(LS->mP0));
				Vector2DSub(&P1_P0, &(LS->mP1), &(LS->mP0));
				if (Vector2DDotProduct(&Pi_P1, &P0_P1) >= 0.0f && Vector2DDotProduct(&Pi_P0, &P1_P0) >= 0.0f)	// Intersection is on the line segment of LS
				{
					return t;
				}
				else									// TODO(Don't know how?): Need to handle the case where the circle hit the start or end point of the line segment.
				{
					return -1.0f;
				}
			}
			else
			{
				return -1.0f;
			}
		}
		else														
		{
			return -1.0f;								// TODO(Don't know how?): Need to handle the case the distance between the center of the circle and the line segement is samller than the circle radius.
		}
	}
	else
	{
		return -F_UNDEFINE;
	}
}


/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::ReflectAnimatedPointOnStaticLineSegment(Vector2D* Ps, Vector2D* Pe, LineSegment2D* LS, Vector2D* Pi, Vector2D* R)
{
	float t = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);
	if (t >= 0.0f)											// Intersection success
	{
		if (R)
		{
			Vector2D i;
			Vector2DSub(&i, Pe, Pi);						// i = Pe - Pi
			Vector2DScaleAdd(R, &(LS->mN), &i, -2 * Vector2DDotProduct(&i, &(LS->mN)));	// R = i - 2 (N dot i) N
			return t;
		}
		else
		{
			return -1.0f;
		}
	}
	else
	{
		return t;
	}
}


/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::ReflectAnimatedCircleOnStaticLineSegment(Vector2D* Ps, Vector2D* Pe, float Radius, LineSegment2D* LS, Vector2D* Pi, Vector2D* R)
{
	float t = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);
	if (t >= 0.0f)										// Intersection success
	{
		if (R)
		{
			Vector2D i;
			Vector2DSub(&i, Pe, Pi);					// i = Pe - Pi
			Vector2DScaleAdd(R, &(LS->mN), &i, -2 * Vector2DDotProduct(&i, &(LS->mN))); // R = i - 2 (N dot i) N						
			return t;
		}
		else
		{
			return -1.0f;
		}
	}
	else
	{
		return t;
	}
}


/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::AnimatedPointToStaticCircle(Vector2D* Ps, Vector2D* Pe, Vector2D* Center, float Radius, Vector2D* Pi)
{
	if (Ps && Pe && Center && Radius > 0.0f && Pi)
	{
		LineSegment2D LS;
		float n, m, v_len_inv;
		Vector2D v;
		Vector2D Ps_Center;

		BuildLineSegment2D(&LS, Ps, Pe);
		Vector2DSub(&v, Pe, Ps);
		Vector2DSub(&Ps_Center, Center, Ps);

		v_len_inv = 1.0f / Vector2DLength(&v);
		m = Vector2DDotProduct(&v, &Ps_Center) * v_len_inv;
		n = Vector2DSquareLength(&Ps_Center) - powf(m, 2);

		if (n <= powf(Radius,2) && !(m < 0.0f && Vector2DSquareLength(&Ps_Center) > powf(Radius,2)))
		{
			float s = sqrtf(powf(Radius, 2) - n);
			float t = (m - s) * v_len_inv;
			if (t >= 0.0f && t <= 1.0f)
			{
				Vector2DScaleAdd(Pi, &v, Ps, t);
				return t;
			}
			else
			{
				return -1.0f;
			}
		}
		else
		{
			return -1.0f;
		}
	}
	else
	{
		return -1.0f;
	}
}


/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::ReflectAnimatedPointOnStaticCircle(Vector2D* Ps, Vector2D* Pe, Vector2D* Center, float Radius, Vector2D* Pi, Vector2D* R)
{
	float t = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);
	if (t >= 0.0f && t <= 1.0f && R)
	{
		Vector2D n, m;
		Vector2DSub(&n, Pi, Center);
		Vector2DNormalize(&n, &n);
		Vector2DSub(&m, Ps, Pi);
		Vector2DScale(&n, &n, 2 * Vector2DDotProduct(&m, &n));
		Vector2DSub(R, &n, &m);
		return t;
	}
	else
	{
		return -1.0f;
	}
}


/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s:		The starting position of the animated circle's center
	- Center0e:		The ending position of the animated circle's center
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::AnimatedCircleToStaticCircle(Vector2D* Center0s, Vector2D* Center0e, float Radius0, Vector2D* Center1, float Radius1, Vector2D* Pi)
{
	return AnimatedPointToStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi);
}


/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one

 - Parameters
	- Center0s:		The starting position of the animated circle's center
	- Center0e:		The ending position of the animated circle's center
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:			Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float My::ReflectAnimatedCircleOnStaticCircle(Vector2D* Center0s, Vector2D* Center0e, float Radius0, Vector2D* Center1, float Radius1, Vector2D* Pi, Vector2D* R)
{
	return ReflectAnimatedPointOnStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi, R);
}
