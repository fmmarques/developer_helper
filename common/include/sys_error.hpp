#pragma once
#if !defined(ERROR_HPP)
#	define ERROR_HPP

#include "definitions.hpp"
#include <sstream>
#include <Windows.h>

#include "functions.hpp"

class sys_error;
std::ostream& operator<<(std::ostream& os, const sys_error& error);

class sys_error
{
	DWORD error_code;
public:
	sys_error():
		error_code{::GetLastError()}
	{
		;
	}

	virtual ~sys_error()
	{
		;
	}

	DWORD code() const
	{
		return error_code;
	}

	void code(DWORD code)
	{
		error_code = code;
	}

	std::string to_string() const
	{
		std::ostringstream formatter;
		formatter << this;
		return formatter.str();
	}

};


#endif