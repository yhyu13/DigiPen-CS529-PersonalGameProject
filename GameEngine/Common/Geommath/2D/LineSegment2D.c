#include "LineSegment2D.h"

using namespace My;

int BuildLineSegment2D(LineSegment2D *LS, Vector2D *Point0, Vector2D *Point1)
{
	if (LS && Point0 && Point1)
	{
		Vector2DSet(&(LS->mP0), Point0->x, Point0->y);
		Vector2DSet(&(LS->mP1), Point1->x, Point1->y);
		Vector2DSub(&(LS->mN), Point1, Point0);
		Vector2DSet(&(LS->mN), LS->mN.y, -LS->mN.x);
		Vector2DNormalize(&(LS->mN), &(LS->mN));
		LS->mNdotP0 = Vector2DDotProduct(&(LS->mN), Point0);
		return 1;
	}
	else
	{
		return 0;
	}
}