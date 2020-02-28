#include "BaseApplication.hpp"

bool My::BaseApplication::m_bQuit = false;
double My::BaseApplication::frametime = 0.0;
double My::BaseApplication::time_modifier = 1.0;
double My::BaseApplication::static_frametime = 0.0;
bool My::BaseApplication::m_bPaused = false;

My::BaseApplication::BaseApplication(const GfxConfiguration& config)
	:
	m_config(config)
{}

int My::BaseApplication::Initialize()
{
	return 0;
}

int My::BaseApplication::Tick(double deltaTime)
{
	return 0;
}

int My::BaseApplication::Finalize()
{
	return 0;
}

int My::BaseApplication::BeginDraw()
{
	return 0;
}

int My::BaseApplication::OnDraw()
{
	return 0;
}

int My::BaseApplication::EndDraw()
{
	return 0;
}

void My::BaseApplication::ShowMessage(const std::wstring& title, const std::wstring& message)
{
	std::wcerr << title << std::endl;
	std::wcerr << message << std::endl;
}

const My::GfxConfiguration& My::BaseApplication::GetConfig() const noexcept { return m_config; }

void My::BaseApplication::SetConfig(const GfxConfiguration& config) noexcept { m_config = config; }

void My::BaseApplication::UpdateFrameTime(double deltaTime)
{
	BaseApplication::frametime = deltaTime;
	BaseApplication::static_frametime = deltaTime;

	(m_bPaused) ? frametime = 0.0 : frametime = time_modifier * static_frametime;
}

bool My::BaseApplication::IsQuit()
{
	return m_bQuit;
}

bool My::BaseApplication::IsPuased()
{
	return m_bPaused;
}

void My::BaseApplication::SetQuit(bool q)
{
	m_bQuit = q;
}

void My::BaseApplication::TogglePause()
{
	m_bPaused = !m_bPaused;
}
