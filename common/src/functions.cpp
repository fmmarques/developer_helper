#include <functions.hpp>

template<> TCHAR* to<TCHAR*, std::string>(std::error_code& error, const std::string& input, TCHAR*& output)
{
	if (output == nullptr)
	{
		error = std::make_error_code(std::errc::invalid_argument);
		return nullptr;
	}

	auto output_buffer_length = sizeof(output) / sizeof(decltype(output));
	if (output_buffer_length <= input.length())
	{
		error = std::make_error_code(std::errc::no_buffer_space);
		return nullptr;
	}

	const char * const c_str = input.c_str();
	std::copy(c_str, &c_str[output_buffer_length], output);

	return output;
}


std::string win32_error_code_to_string(std::error_code& errc, DWORD error_code)
{
	std::string result{};
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
		return std::string("");
	}
	result = std::string(output_text);

	return result;
}


std::string com_error_code_to_string(std::error_code& errc, DWORD hresult)
{
	// From https://stackoverflow.com/questions/455434/how-should-i-use-formatmessage-properly-in-c
	std::string result{};
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
		return std::string("");

	}

	if (NULL != errorText)
	{
		// ... do something with the string `errorText` - log it, display it to the user, etc.
		result = std::string(errorText);
		// release memory allocated by FormatMessage()
		LocalFree(errorText);
		errorText = NULL;
	}
}

