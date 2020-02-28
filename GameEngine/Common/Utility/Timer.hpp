#pragma once
#include <chrono>
#include "EngineException.hpp"

using namespace std::chrono;

namespace My
{
	class Timer
	{
	public:
		Timer();

		void Reset() noexcept;
		double Mark() noexcept;
	private:
		std::chrono::steady_clock::time_point last;
	};
}