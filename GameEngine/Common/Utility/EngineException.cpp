#include "EngineException.hpp"

const std::wstring& My::EngineException::GetNote() const
{
    return note;
}

const std::wstring& My::EngineException::GetFile() const
{
    return file;
}

unsigned int My::EngineException::GetLine() const
{
    return line;
}

std::wstring My::EngineException::GetLocation() const
{
    return std::wstring( L"Line [" ) + std::to_wstring( line ) + L"] in " + file;
}

std::wstring My::EngineException::GetFullMessage() const
{
#if 1
    return GetNote() + L"\nAt: " + GetLocation();
#else
	return GetNote();
#endif
}

std::wstring My::EngineException::GetExceptionType() const
{
    return L"Engine Exception";
}

std::wstring My::str2wstr(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

std::string My::wstr2str(const std::wstring& wstr)
{
	return std::string(wstr.begin(), wstr.end());
}
