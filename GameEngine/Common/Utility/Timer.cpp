#include "Timer.hpp"

My::Timer::Timer()
{
	if (std::chrono::steady_clock::is_steady == false)
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"C++11 std::chrono::steady_clock::is_steady() returns FALSE.");
	last = steady_clock::now();
}


void My::Timer::Reset() noexcept
{
	last = steady_clock::now();
}

double My::Timer::Mark() noexcept
{
	const duration<double> Time = steady_clock::now() - last;
	return Time.count();
}
