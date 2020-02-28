#include "WinSDLInputManager.hpp"

My::WinSDLInputManager::WinSDLInputManager() noexcept
	:
	m_current_keyboard_state(nullptr),
	m_previous_keyboard_state(nullptr),
	keyboard_state(nullptr)
{
}

int My::WinSDLInputManager::Initialize()
{
	int size = 1024;
	m_previous_keyboard_state = new Uint8[size];
	m_current_keyboard_state = new Uint8[size];
	return 0;
}

int My::WinSDLInputManager::Tick(double deltaTime)
{
	int size = 1024;
	keyboard_state = SDL_GetKeyboardState(&size);
	memcpy(m_previous_keyboard_state, m_current_keyboard_state, size);
	memcpy(m_current_keyboard_state, keyboard_state, size);
	IRunTimeModule::Tick(deltaTime);										// Execute additional callback functions added to the input manager.

	return 0;
}

int My::WinSDLInputManager::Finalize()
{
	delete[] m_previous_keyboard_state;
	delete[] m_current_keyboard_state;
	return 0;
}

int My::WinSDLInputManager::KeyIsPressed(int KeyScanCode) noexcept
{
	return (m_previous_keyboard_state[KeyScanCode] != 1 && m_current_keyboard_state[KeyScanCode] == 1) ? 1 : 0;
}

int My::WinSDLInputManager::KeyIsReleased(int KeyScanCode) noexcept
{
	return (m_previous_keyboard_state[KeyScanCode] == 1 && m_current_keyboard_state[KeyScanCode] != 1) ? 1 : 0;
}

int My::WinSDLInputManager::KeyIsDown(int KeyScanCode) noexcept
{
	return (m_current_keyboard_state[KeyScanCode] == 1) ? 1 : 0;
}
