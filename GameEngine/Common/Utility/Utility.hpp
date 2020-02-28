#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include "Common/Geommath/2D/Math.hpp"

#define PRINT(str)  std::cout << str
#define wPRINT(str) std::wcout << str
#define ADJUST_fVALUE_60HZ(value, frameTime) value * static_cast<float>(frameTime) * 60.0f
#define ADJUST_dVALUE_60HZ(value, frameTime) value * frameTime * 60.0

#ifdef _DEBUG
#define DEBUG_PRINT(str)  std::cout << str
#define DEBUG_wPRINT(str) std::wcout << str
#else
#define DEBUG_PRINT(str) 
#define DEBUG_wPRINT(str)
#endif // DEBUG

namespace My
{
	template <typename T>
	std::string Str(const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

    template <typename T>
	std::wstring wStr(const T& t)
	{
		std::wstringstream ss;
		ss << t;
		return ss.str();
	}

	template<class T>
	void SafeRelease(T **ppInterfaceToRelease)
	{
		if (*ppInterfaceToRelease != nullptr)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = nullptr;
		}
	}

	void print_fstream(std::ifstream& fstream);
	void rewind_fstream(std::ifstream& fstream);
}
