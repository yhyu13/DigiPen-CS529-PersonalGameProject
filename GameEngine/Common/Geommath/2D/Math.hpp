/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Math
Purpose: header file of Math
Language: c++ 11
Platform: win32 x86
Project: CS529_project_2
Author: Hang Yu hang.yu 60001119
Creation date: 10/03/2019
- End Header ----------------------------*/

#pragma once

#include <math.h>

#ifndef EPSILON
#define EPSILON 0.0001f
#endif

#ifndef F_UNDEFINE
#define F_UNDEFINE 19951215.0f					// Weird number, hum?
#endif

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef TWO_PI
#define TWO_PI 3.14159265358979323846f * 2.0f
#endif

#ifndef RAD
#define RAD 0.0174532925f
#endif

#define RAND_I(LO, HI) LO + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (HI - LO)))
#define RAND_F(LO, HI) LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)))
#define IN_RANGE(a, x, y) (a >= x && a <= y)

#define MAX(x, y) (x>y)?x:y
#define MIN(x, y) (x<y)?x:y