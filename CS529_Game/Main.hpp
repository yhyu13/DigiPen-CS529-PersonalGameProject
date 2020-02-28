/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Main
Purpose: header file of Main
Language: c++ 11
Platform: win32 x86
Project: CS529_project_final
Author: Hang Yu hang.yu 60001119
Creation date: 10/17/2019
- End Header ----------------------------*/

#pragma once

#include "stdio.h"
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE* __cdecl __iob_func(void)
{
	return _iob;
}
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include "src/GameApplication.hpp"

namespace My
{
	extern MY_CALL_BACK Initialize();
	extern MY_CALL_BACK GameLoop();
	extern MY_CALL_BACK Finalize();
}