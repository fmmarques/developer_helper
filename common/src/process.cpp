#include "process.hpp"
template < typename _in, typename _out > process< user_type::current_user, _in, _out > make_process(const std::string& image_name, const std::string& argument_name, _in input, _out output);
process< user_type::current_user, HANDLE, HANDLE > make_process(const std::string& image_name, const std::string& argument_name, HANDLE input, HANDLE output);

process< user_type::current_user, HANDLE, HANDLE > make_process(const std::string& image_name, const std::string& arguments, DWORD flags = 0x0)
{

	std::wcerr << "C1" << std::endl;
	process< user_type::current_user, HANDLE, HANDLE > result{ image_name, arguments, flags };

	std::wcerr << "C2" << std::endl;
	return result;
}