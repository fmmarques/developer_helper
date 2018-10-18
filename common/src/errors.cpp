#include "sys_error.hpp"


std::ostream& operator<<(std::ostream& os, const sys_error& error)
{
	os << win32_error_code_to_string(error.error_code());
	return os;
}