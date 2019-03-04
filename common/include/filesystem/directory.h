/* A directory iterator in C++
 * Date: 15/02/2019
 */
#pragma once
#if !defined(CPPCOMMON_FILESYSTEM_DIRECTORY_H)
#  define CPPCOMMON_FILESYSTEM_DIRECTORY_H

#include <string>
#include <memory>

#include "../exceptions.h"

#undef UNICODE
#include <Windows.h>

namespace cppcommon {
	namespace filesystem {
		namespace interface1 {

#if defined(WIN32)
			namespace win32
			{
				class directory_iterator;
				template < typename iterator_type = directory_iterator >
				class directory
				{
				private:
					std::string name;
					HANDLE hDirectory;
				public:
					directory(const std::string& directory_name) noexcept(false) :
						name(directory_name)
					{
						std::unique_ptr< TCHAR[] > ptc_directory_name = nullptr;
						ptc_directory_name = std::make_unique< TCHAR[] >(directory_name.length() + 1);
						std::copy(directory_name.cbegin(), directory_name.cend(), ptc_directory_name.get());
						ptc_directory_name[directory_name.length()] = '\0';

						hDirectory = CreateFile(ptc_directory_name.get(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

						if (INVALID_HANDLE_VALUE == hDirectory)
						{
							switch (GetLastError())
							{
							case ERROR_FILE_NOT_FOUND:
								throw new cppcommon::exceptions::no_such_file_or_directory("The directory \"" + directory_name + "\" was not found.");
								break;
							default:
								throw new std::runtime_error("Opening directory for iteration returned error: " + std::to_string(GetLastError()));
							}
						}
					}

					iterator_type& begin()
					{

					}

					~directory()
					{
						CloseHandle(hDirectory);
					}

				};

			}

			using namespace win32;
#endif

		}


	}
}

#endif

