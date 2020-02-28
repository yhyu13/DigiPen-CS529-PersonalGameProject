#pragma once
#include <sstream>
#include "Common/Interface/IRunTimeModule.hpp"
#include "Common/GfxConfiguration.hpp"
#include "Common/MemoryManager.hpp"

namespace My
{
	class BaseApplication : public IRunTimeModule
	{
	public:

		BaseApplication() = delete;
		BaseApplication(const BaseApplication&) = delete;
		BaseApplication& operator=(const BaseApplication&) = delete;
		explicit BaseApplication(const GfxConfiguration& config);

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;

		virtual int BeginDraw();
		virtual int OnDraw();
		virtual int EndDraw();

		virtual void ShowMessage(const std::wstring& title, const std::wstring& message);

		const GfxConfiguration& GetConfig() const noexcept;
		virtual void SetConfig(const GfxConfiguration& config) noexcept;

		static void UpdateFrameTime(double deltaTime);
		static bool IsQuit();
		static bool IsPuased();
		static void SetQuit(bool q);
		static void TogglePause();

	public:
		static double frametime;
		static double time_modifier;
		static double static_frametime;
		
	protected:
		GfxConfiguration m_config;
	private:
		static bool m_bQuit;
		static bool m_bPaused;
		
	};
	extern std::shared_ptr<BaseApplication> g_pBaseApplication;
}

#include "Common/Utility/Scheduler.hpp"