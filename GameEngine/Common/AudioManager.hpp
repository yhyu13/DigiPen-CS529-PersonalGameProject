#pragma once
#include "Interface/IRunTimeModule.hpp"

namespace My
{
	class AudioManager : public IRunTimeModule
	{
	public:

		AudioManager() noexcept;
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
	};
}