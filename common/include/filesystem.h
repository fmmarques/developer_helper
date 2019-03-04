/* A directory iterator in C++
 * Date: 15/02/2019
 */
#pragma once
#if !defined(CPPCOMMON_FILESYSTEM_H)
#  define CPPCOMMON_FILESYSTEM_H

#include <string>
#include <memory>
#include <cassert>

#include "functions.hpp"
#include "exceptions.h"



struct IUnknown;
#undef UNICODE
#include <Windows.h>

namespace cppcommon {
	namespace filesystem {
		namespace interface1 {
			namespace
			{
#if defined(WIN32)
				struct win32_enabled
				{
					static constexpr bool enabled = true;
					using handle = HANDLE;
				};
#endif
			}

			/**
			 * An interface to a fs element.
			 */
			struct fs_elem_interface
			{
				virtual std::wstring name() const = 0;
				virtual void name(const std::wstring& element_name) = 0;
			
			};

			/**
			 * An abstraction over fs elements.
			 */
			class fs_elem_abstract : public virtual fs_elem_interface
			{
			protected:
				std::wstring _name;
			public:
				fs_elem_abstract(const std::wstring element_name) noexcept(true) :
					_name{ element_name }
				{}

				std::wstring name() const override { return _name; }
				void name(const std::wstring& element_name) override { _name = element_name; }
			};

			/**
			 * A concrete implementation using CRTP
			 */ 
			template < typename implementation_t >
			struct fs_elem : public virtual fs_elem_interface, protected fs_elem_abstract
			{
				using implementation_t::name;
			};


			struct file_impl : protected fs_elem_abstract
			{
			protected:
				HANDLE file_handle;
			public:

				file_impl(const std::wstring& filename) noexcept(false):
					fs_elem_abstract{filename}
				{
					using cppcommon::functions::to_TCHAR;
					wchar_t filename_buffer[MAX_PATH] = { 0, };
					std::error_code error{};

					to_TCHAR< wchar_t, wchar_t >(error, std::begin(filename), std::end(filename), &filename_buffer[0], &filename_buffer[0] + MAX_PATH);
					//file_handle = CreateFile()
				}
				operator HANDLE() const { return file_handle; }
			};

			/* An iterator over a directory
			 **/
			class dir_it : std::iterator< std::random_access_iterator_tag,
				fs_elem_interface *,
				ptrdiff_t,
				fs_elem_interface **,
				fs_elem_interface *&>
			{
			private:
				class invariant
				{
				private:
					const dir_it& it;

					void _invariant() const
					{
						assert(it.directory_handle != INVALID_HANDLE_VALUE && "The directory handle for which to iterate is invalid.");
						assert(it.directory_handle != INVALID_HANDLE_VALUE && "The iterator handle is invalid.");
					}

				public:
					invariant(const dir_it& it) :
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
				dir_it(const HANDLE directory_handle, const std::string& pathname) noexcept(false) :
					filename{ pathname },
					directory_handle{ directory_handle },
					iterator_handle{ nullptr },
					iterator_find_data{ 0 }
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

				dir_it& end() const
				{
				}


				~dir_it()
				{
					{ invariant invariant(*this); }
					FindClose(iterator_handle);
				}

			};


			template <  
				typename element_type = fs_elem_interface * ,
				typename iterator_type = dir_it 
			> class dir : public virtual fs_elem_interface
			{
			private:
				std::string name;
				HANDLE hDirectory;
			public:
				dir(const std::string& directory_name) noexcept(false) :
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

				iterator_type begin()
				{
					return iterator_type(*this);
				}

				~dir()
				{
					CloseHandle(hDirectory);
				}

			};

		}
	}
}


#endif

