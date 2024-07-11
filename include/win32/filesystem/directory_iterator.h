/* A directory iterator in C++
 * Date: 15/02/2019
 */
#pragma once
#if !defined(CPPCOMMON_FILESYSTEM_DIRECTORY_ITERATOR_H)
#  define CPPCOMMON_FILESYSTEM_DIRECTORY_ITERATOR_H

#include <string>
#include <memory>
#include <cassert>

#include "../exceptions.h"
#include "directory.h"

namespace cppcommon {
	namespace filesystem {
		namespace interface1 {

#if defined(WIN32)
#	undef UNICODE
			namespace win32 
			{
				class directory_iterator
				{
				private:


					class invariant
					{
					private:
						const directory_iterator& it;

						void _invariant() const
						{
							assert(it.directory_handle != INVALID_HANDLE_VALUE && "The directory handle for which to iterate is invalid.");
							assert(it.directory_handle != INVALID_HANDLE_VALUE && "The iterator handle is invalid.");
						}

					public:
						invariant(const directory_iterator& it) :
							it(it)
						{
							_invariant();
						}

						~invariant()
						{
							_invariant();
						}
					};
					std::string filename;
					HANDLE directory_handle;
					HANDLE iterator_handle;
					WIN32_FIND_DATAA iterator_find_data;
					ULONG64 file_count;

				public:
					directory_iterator(const HANDLE directory_handle, const std::string& pathname) noexcept(false) :
						filename { pathname },
						directory_handle{ directory_handle },
						iterator_handle{ nullptr },
						iterator_find_data { 0 } 
					{
						auto c_filename = filename.c_str();
						iterator_handle = FindFirstFileA(c_filename, &iterator_find_data);
						if (INVALID_HANDLE_VALUE == iterator_handle)
						{
							throw GetLastError();
						}
						
						invariant invariant(*this);
						do
						{
							file_count++;
						} while (FindNextFile(iterator_handle, &iterator_find_data));
					

					}

					directory_iterator& end() const 
					{
					}


					~directory_iterator()
					{
						{ invariant invariant(*this); }
						FindClose(iterator_handle);
					}

				};
			}
			using namespace win32;
#endif
		}
	}
}

#endif

