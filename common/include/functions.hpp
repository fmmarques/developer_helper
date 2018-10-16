#pragma once

#if !defined(FUNCTIONS_H)
#	define FUNCTIONS_H
#include "definitions.hpp"
#include <string>
#include <system_error>
#include <windows.h>

template < class output_char_t, class input_char_t > output_char_t to(std::error_code&, const input_char_t&, output_char_t&);

template<> TCHAR* to<TCHAR*, std::string>(std::error_code& error, const std::string& input, TCHAR*& output);

std::string win32_error_code_to_string(std::error_code& errc, DWORD error_code);


std::string com_error_code_to_string(std::error_code& errc, DWORD hresult);



#endif
