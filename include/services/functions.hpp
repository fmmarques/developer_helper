#pragma once
#if !defined(CPPCOMMON_FUNCTIONS_H)
#	 include <string>
#	 include <sstream>
#	 include <locale>
#	 include <codecvt>
#	 include <algorithm>

namespace cppcommon
{
	namespace functions
	{
		using std::wstring;

		std::wstring win32_error_code_to_string(std::error_code& errc, unsigned long error_code);

		std::wstring com_error_code_to_string(std::error_code& errc, unsigned long hresult);

		std::wstring current_execution_directory();

		/**
		 * Gets the executable image's directory, if possible.
		 * @param error_code the error_code holder for the case that an error occours
		 * @return the executable image's directory or an empty wstring.
		 * @note client code must test the following error conditions:
		 *	 errc::no_such_process - couldn't use the process's handle to read the image's directory.
		 */
		std::wstring executable_directory(std::error_code& error_code);

		
		/**
		 * Converts an input type to wstring.
		 * @param original the original value to convert
		 * @return the wstring converted.
		 */
		template < typename input_t >
		std::wstring to_wstring(const input_t& original)
		{
			std::wostringstream woss;
			woss << original;
			return woss.str();
		}

		/**
		 * Converts an input type to wstring.
		 * @param str the original value to convert
		 * @return the wstring converted.
		 */
		template < >
		std::wstring to_wstring< std::string >(const std::string& str)
		{
			return std::wstring_convert< std::codecvt_utf8< wchar_t > >().from_bytes(str);
		}

		/**
		 * Converts an input type to wstring.
		 * @param str the original value to convert
		 * @return the wstring converted.
		 */
		template < typename const_forward_iterator_t = std::string::const_iterator >
		std::wstring to_wstring(const const_forward_iterator_t& begin, const const_forward_iterator_t& end)
		{
			std::wstringstream result{};
			std::for_each(begin, end, [&result](char ch) { result << ch; });
			return result.str();
		}

		/**
		 * Converts an input value to a string.
		 * @param value a string convertable primitive value
		 * @return a std::string
		 */
		template < typename input_t >
		std::string to_string(const input_t& value)
		{
			return std::to_string(value);
		}

		/**
		 * Converts a wide string into a string, using UTF8 facet.
		 */
		template <>
		std::string to_string(const std::wstring& wstr)
		{
			return std::wstring_convert< std::codecvt_utf8< wchar_t > >().to_bytes(wstr);
		}

		template < typename const_forward_iterator_t = std::wstring::const_iterator >
		std::string to_string(const const_forward_iterator_t& begin, const const_forward_iterator_t& end)
		{
			std::wstringstream wss{};
			std::for_each(begin, end, [&wss](auto&& ch) { wss << ch; });
			return std::wstring_convert< std::codecvt_utf8< wchar_t > >().to_bytes(wss.str());
		}
	
	
		template<
			typename input_char_type = TCHAR,
			typename output_char_type = TCHAR,
			typename output_iterator_type = output_char_type *,
			typename input_char_type_traits_type = std::char_traits< input_char_type >,
			typename input_iterator_type = typename std::basic_string< input_char_type, input_char_type_traits_type, std::allocator< input_char_type > >::iterator >
			void to_TCHAR(
				std::error_code& error_code,
				const input_iterator_type& input_begin,
				const input_iterator_type& input_end,
				output_iterator_type output_begin,
				output_iterator_type output_end 
			) 
		{
		
			if (std::distance(input_begin, input_end) > std::distance(output_begin, output_end))
			{
				error_code = std::make_error_code(std::errc::no_buffer_space);
				return;
			}

			std::copy( input_begin, input_end, output_begin );
			output_end = output_begin + std::distance(input_begin, input_end);
		}

		template<>
		void to_TCHAR<char, wchar_t>(
			std::error_code& error_code,
			const std::string::const_iterator& input_begin,
			const std::string::const_iterator& input_end,
			wchar_t *&output_begin,
			wchar_t *&output_end)
		{
			if (std::distance(input_begin, input_end) > std::distance(output_begin, output_end))
			{
				error_code = std::make_error_code(std::errc::no_buffer_space);
				return;
			}

			auto&& wide_string_input = to_wstring(input_begin, input_end);
			auto&& output_iterator = output_begin;
			for (auto&& input_iterator : wide_string_input)
				*output_iterator++ = input_iterator;
		}

		template<>
		void to_TCHAR<wchar_t, char>(
			std::error_code& error_code,
			const std::wstring::const_iterator& input_begin,
			const std::wstring::const_iterator& input_end,
			char *&output_begin,
			char *&output_end)
		{
			if (std::distance(input_begin, input_end) > std::distance(output_begin, output_end))
			{
				error_code = std::make_error_code(std::errc::no_buffer_space);
				return;
			}
			auto&& string_input = to_string(input_begin, input_end);
			std::copy(std::begin(string_input), std::end(string_input), output_begin);

		}

	}

}

#endif
