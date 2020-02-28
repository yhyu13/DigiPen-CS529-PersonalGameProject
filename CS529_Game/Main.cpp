/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Main
Purpose: implementation of Main
Language: c++ 11
Platform: win32 x86
Project: CS529_project_final
Author: Hang Yu hang.yu 60001119
Creation date: 10/17/2019
- End Header ----------------------------*/
#include "Main.hpp"
using namespace My;

int main(int argc, char* args[])
{
	// This api call make the process be aware of dpi 
	// i.e. managing dpi by ourself instead of Desktop Window Manager(DWM)
	// i.e. ignoring user dpi scales which are common to be 125%, 150%, etc
	// Not making this api call would result in the window we created to be scaled with 
	// user dpi setting (e.g. "Change Display Setting" in Win10).
	//https://docs.microsoft.com/en-us/windows/win32/api/windef/ne-windef-dpi_awareness
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Initialization
	{
		Initialize();
	}
	// Game Loop
	{
		GameLoop();
	}

	// Finalize()
	{
		Finalize();
	}

	return 0;
}