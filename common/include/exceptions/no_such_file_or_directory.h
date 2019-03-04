#pragma once
#if !defined(CPPCOMMON_EXCEPTIONS_NO_SUCH_FILE_OR_DIRECTORY_H)
#  define CPPCOMMON_EXCEPTIONS_NO_SUCH_FILE_OR_DIRECTORY_H

#include <stdexcept>

namespace cppcommon 
{
	namespace exceptions 
	{
		namespace interface1 
		{
			class no_such_file_or_directory : public std::runtime_error
			{
			public:
				no_such_file_or_directory(const std::string& msg) :
					std::runtime_error(msg)
				{};

				no_such_file_or_directory(const char * what_arg) :
					std::runtime_error(what_arg)
				{};
			};
		}

		using namespace interface1;
	}
}

#endif