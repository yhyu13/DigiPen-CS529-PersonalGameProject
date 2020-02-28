/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.hpp
Purpose: header file of InputManager
Language: c++ 11
Platform: win32 x86
Project: CS529_final_project
Author: Hang Yu hang.yu 60001119
Creation date: 10/13/2019
- End Header ----------------------------*/


#pragma once
#include <SDL.h>
#include "Common/InputManager.hpp"

namespace My
{
	class WinSDLInputManager : public InputManager
	{
	public:

		WinSDLInputManager() noexcept;
		WinSDLInputManager(const WinSDLInputManager&) = delete;
		WinSDLInputManager& operator=(const WinSDLInputManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;

		virtual int KeyIsPressed(int KeyScanCode) noexcept override;
		virtual int KeyIsReleased(int KeyScanCode) noexcept override;

		virtual int KeyIsDown(int KeyScanCode) noexcept override;

	private:
		Uint8* m_previous_keyboard_state;
		Uint8* m_current_keyboard_state;
		const Uint8* keyboard_state;
	};
}