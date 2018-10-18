#pragma once
#if !defined(EXCEPTIONS)
#	define EXCEPTIONS
#
#	include <stdexcept>

namespace 
{
	std::string strperror();
}

namespace fmmarques {
	namespace devhelper {

		class couldnt_open_process_token : public std::exception 
		{};
	}
}

#endif