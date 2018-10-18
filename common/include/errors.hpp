#pragma once
#if !defined(ERROR_HPP)
#	define ERROR_HPP

#include "definitions.hpp"
#include <sstream>
#include <Windows.h>

#include "functions.hpp"

namespace common {
	namespace errors {
		namespace interface1 {
		
		
			class win32_error;
			std::ostream& operator<<(std::ostream& os, const win32_error& error);

			class win32_error
			{
				DWORD error_code;
			public:
				win32_error():
				error_code{::GetLastError()}
				{
					;
				}

				virtual ~win32_error()
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

		}

		using namespace common::errors::interface1;
	}
}

#endif