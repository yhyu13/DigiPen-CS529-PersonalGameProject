#pragma once
#include "Interface/IRunTimeModule.hpp"

namespace My
{
	class GraphicsManager : public IRunTimeModule
	{
	public:

		GraphicsManager() noexcept;
		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
	};
}