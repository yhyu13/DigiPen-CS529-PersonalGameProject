#pragma once
#include <future>
#include "Common/Utility/Timer.hpp"
#include "Common/BaseApplication.hpp"

namespace My
{
	extern std::shared_ptr<BaseApplication> g_pBaseApplication;

	template< class Function, class... Args>
	int Run(Function&& f, Args&& ... args)
	{
		try
		{
			f(args...);
		}
		catch (const EngineException& e)
		{
			BaseApplication::SetQuit(true);
			const std::wstring eMsg = e.GetFullMessage() +
				L"\n\nEngineException caught.";
			g_pBaseApplication->ShowMessage(e.GetExceptionType(), eMsg);
		}
		catch (const std::exception& e)
		{
			BaseApplication::SetQuit(true);
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr(e.what());
			const std::wstring w_whatStr(whatStr.begin(), whatStr.end());
			const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
				L"\n\nException caught.";
			g_pBaseApplication->ShowMessage(w_whatStr, eMsg);
		}
		catch( ... )
		{
			BaseApplication::SetQuit(true);
			g_pBaseApplication->ShowMessage( L"Unhandled Non-STL Exception",L"Exception caught at Run." );
		}
		return 0;
	}

	template<class Function, class... Args>
	int GameLoopUpdate(double waitTime, Function&& f, Args&& ... args)
	{
		Timer drawTimer;
		try
		{
			while (!g_pBaseApplication->IsQuit())
			{
				drawTimer.Reset();
				f(args...);
				while (drawTimer.Mark() < waitTime) {};
				BaseApplication::frametime = drawTimer.Mark();
			}
		}
		catch (const EngineException& e)
		{
			BaseApplication::SetQuit(true);
			const std::wstring eMsg = e.GetFullMessage() +
				L"\n\nException caught at Windows message loop.";
			g_pBaseApplication->ShowMessage(e.GetExceptionType(), eMsg);
		}
		catch (const std::exception& e)
		{
			BaseApplication::SetQuit(true);
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr(e.what());
			const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
				L"\n\nException caught at Windows message loop.";
			g_pBaseApplication->ShowMessage(L"Unhandled STL Exception", eMsg);
		}
		catch( ... )
		{
			BaseApplication::SetQuit(true);
			g_pBaseApplication->ShowMessage( L"Unhandled Non-STL Exception",L"Exception caught at GameLoopUpdate." );
		}
		return 0;
	}
}