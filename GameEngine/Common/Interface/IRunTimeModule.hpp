#pragma once

#include <functional>
#include <vector>
#include <memory>
#include "Common/Utility/EngineException.hpp"

namespace My
{
	class IRunTimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual int Tick(double deltaTime) { for (auto& func : m_callbacks) { func(deltaTime); }; return 0; };
		virtual int Finalize() = 0;
		virtual void Serialize(std::wostream& wostream) {};
		void AddCallBackToTick(std::function<void(double)> func) { m_callbacks.push_back(func); };
		void ClearCallBackToTick() { m_callbacks.clear(); };
	private:
		std::vector<std::function<void(double)>> m_callbacks;
	};
}