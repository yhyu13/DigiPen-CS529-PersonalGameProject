#pragma once
#include "Interface/IRunTimeModule.hpp"

namespace My
{
	class InputManager : public IRunTimeModule
	{
	public:

		InputManager() noexcept;
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;

		virtual int KeyIsPressed(int KeyScanCode) noexcept;
		virtual int KeyIsReleased(int KeyScanCode) noexcept;

		virtual int KeyIsDown(int KeyScanCode) noexcept;
	};
	extern std::shared_ptr<InputManager> g_pInputManager;
}