
#include "functions.hpp"

#include <memory>
#include <locale>
#include <codecvt>
#include <system_error>
#include <filesystem>
#include <algorithm>

struct IUnknown;

#include <Windows.h>
#include <Psapi.h>


std::wstring cppcommon::functions::win32_error_code_to_string(std::error_code& errc, unsigned long error_code)
{
	using cppcommon::functions::to_wstring;

	std::wstring result{};
	LPSTR output_text = nullptr;

	if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		error_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&output_text,
		0,
		NULL))
	{
		errc = std::make_error_code(std::errc::bad_message);
		return std::wstring(L"");
	}
	result = to_wstring(output_text);
	return result;
}

std::wstring cppcommon::functions::com_error_code_to_string(std::error_code& errc, unsigned long hresult)
{
	using cppcommon::functions::to_wstring;

	// From https://stackoverflow.com/questions/455434/how-should-i-use-formatmessage-properly-in-c
	std::wstring result{};
	TCHAR * errorText = NULL;

	if (!FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		hresult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL)   // arguments - see note 
		)
	{
		errc = std::make_error_code(std::errc::bad_message);
		return std::wstring(L"");

	}

	if (NULL != errorText)
	{
		// ... do something with the string `errorText` - log it, display it to the user, etc.
		result = to_wstring(errorText);
		// release memory allocated by FormatMessage()
		LocalFree(errorText);
		errorText = NULL;
	}
	return result;
}

std::wstring cppcommon::functions::current_execution_directory()
{
	using cppcommon::functions::to_wstring;

	std::unique_ptr< TCHAR[] > current_directory_buffer = nullptr;
	auto length = GetCurrentDirectory(0, current_directory_buffer.get());
	current_directory_buffer = std::make_unique< TCHAR[] >(length);
	length = GetCurrentDirectory(length, current_directory_buffer.get());
	return to_wstring(current_directory_buffer.get());
}

std::wstring cppcommon::functions::executable_directory(std::error_code& code)
{
	using cppcommon::functions::to_wstring;

	TCHAR executable_directory_buffer[MAX_PATH] = { 0, };
	DWORD executable_directory_length = MAX_PATH;

	if (!QueryFullProcessImageName(GetCurrentProcess(), 0, executable_directory_buffer, &executable_directory_length))
	{
		code = std::make_error_code(std::errc::no_such_process);
		return std::wstring(L"");
	}
	auto executable_directory = to_wstring(executable_directory_buffer);
	auto offset_of_last_separator = executable_directory.find_last_of(std::experimental::filesystem::path::preferred_separator);
	executable_directory.erase(executable_directory.begin() + offset_of_last_separator, executable_directory.end());
	return executable_directory;
}
