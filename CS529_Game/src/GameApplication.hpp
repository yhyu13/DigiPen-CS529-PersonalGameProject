/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameApplication.hpp
Purpose: header file of GameApplication
Language: c++ 11
Platform: win32 x86
Project: CS529_final_project
Author: Hang Yu hang.yu 60001119
Creation date: 10/13/2019
- End Header ----------------------------*/

#pragma once
#include "src/Platform/Win32_SDL/WinSDLOpenGLApplication.hpp"

#define MY_CALL_BACK			void

namespace My
{
	// Dummy Class
	class GameApplication : public WinSDLOpenGLApplication
	{
	public:
		GameApplication() = delete;
		GameApplication(const GameApplication&) = delete;
		GameApplication& operator=(const GameApplication&) = delete;

		explicit GameApplication(const GfxConfiguration& config) : WinSDLOpenGLApplication(config)
		{}
	};

	extern MY_CALL_BACK Initialize();
	extern MY_CALL_BACK GameLoop();
	extern MY_CALL_BACK Finalize();
}