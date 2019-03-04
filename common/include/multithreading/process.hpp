#pragma once
#if !defined(PROCESS_H)
#	define PROCESS_H
#	include "definitions.hpp"
#	include <system_error>
#	include <string>
#	include <Windows.h>
#	include <ProcessSnapshot.h>

#	include "empty_base.hpp"
#	include "errors.hpp"

#	include <iostream>

namespace common {
	namespace multithreading {
		namespace interface1 {

			template < typename _type > class pipe;

			enum class user_type { current_user, impersonate_user, logon_user };
			
			class process_mixin : public empty_base
			{
			public:
				virtual void terminate();
			};

			template < user_type type, typename _input, typename _output > class process;

			template < > class process< user_type::current_user, HANDLE, HANDLE > 
			{
			protected:
				HANDLE input;
				HANDLE output;
				STARTUPINFO startup_information;
				PROCESS_INFORMATION process_information;
				DWORD flags;
				std::string directory;
			public:
				static const unsigned int MAX_COMMAND_LINE = 32768;
			public:
				process(process&& rhs) :
					input{ rhs.input },
					output{ rhs.output },
					startup_information{ rhs.startup_information },
					process_information{ rhs.process_information },
					flags{ rhs.flags },
					directory{ rhs.directory }
				{
					rhs.input = rhs.output = INVALID_HANDLE_VALUE;
				}

				process(const process& other):
					input{ other.input },
					output{ other.output },
					startup_information{ other.startup_information },
					process_information{ other.process_information },
					flags{ other.flags },
					directory{ other.directory }
				{}

				process(const std::string& image_file, HANDLE input_handle, HANDLE output_handle, const std::string& arguments, DWORD process_flags, BOOL inherit_handle = false,  const std::string& process_directory = ".") noexcept(false) :
					input{ input_handle },
					output{ output_handle },
					startup_information{ 0 },
					process_information{ 0 },
					flags{ process_flags },
					directory{ process_directory }
				{
					using common::errors::win32_error;

					BOOL was_process_created = false;
					SECURITY_ATTRIBUTES thread_security_attributes = { 0 }, process_security_attributes = { 0 };
					char modifiable_arguments[ process::MAX_COMMAND_LINE ] = { 0 };
					char current_directory[MAX_PATH] = { 0 };
					int current_directorys_length = 0;

					// Dealing with arguments argument,
					std::stringstream oss{};
					std::string command_line{};

					oss << "\"" << image_file << "\"" << ' ' << arguments << '\0';
					command_line = oss.str();

					if (command_line.length() > process::MAX_COMMAND_LINE)
					{
						std::stringstream oss{};
						oss << "process::process: the image name and arguments given exceeds the " << process::MAX_COMMAND_LINE << " limit of CreateProcess API.";
						throw new std::invalid_argument( oss.str());
					}

					std::copy_n(std::begin(command_line), command_line.length(), modifiable_arguments);
					modifiable_arguments[command_line.length() - 1] = '\0';
		

					// Dealing with process directory argument,

					if (process_directory == ".")
					{
						current_directorys_length = ::GetCurrentDirectory(MAX_PATH, current_directory);
						if (0 == current_directorys_length)
						{
							win32_error err{};
							throw std::runtime_error("Couldn't get the current directory name. Error: " /* + err.to_string() */ );
						}
						current_directory[current_directorys_length] = '\0';
					}
		
					was_process_created = ::CreateProcess(nullptr, modifiable_arguments, &thread_security_attributes, &process_security_attributes, inherit_handle, EXTENDED_STARTUPINFO_PRESENT | flags, NULL, current_directory, &startup_information, &process_information);
					if (!was_process_created)
					{
						std::stringstream oss{};
						win32_error err{};
						oss << "process::process: Couldn't start a process using " << command_line << ". Error: " + err.to_string();
						throw std::runtime_error( oss.str());
					}
				}

				process(const std::string& image_file, const std::string& arguments, DWORD process_flags) :
					process(image_file, ::GetStdHandle(0), ::GetStdHandle(1), arguments, process_flags)
				{ }

				virtual ~process()
				{ 
					CloseHandle(input);
					CloseHandle(output);
				}

				// methods,

			};

			// Implement a process that impersonates an user using CreateProcessAsUser/LogonUser or CreateProcessWithToken/LogonUser, but check if process has SE_INCREASE_QUOTA_NAME 
			// privilege (and maybe SE_ASSIGNPRIMARYTOKEN_NAME), TOKEN_QUERY, TOKEN_DUPLICATE, and TOKEN_ASSIGN_PRIMARY
			


			// Implement a process that logs on an user using CreateProcessWithLogon 

			// Add in all implementations the possibility to add a custom environment block.

			template < typename _in, typename _out > process< user_type::current_user, _in, _out > make_process(const std::string& image_name, const std::string& argument_name, _in input, _out output);
			
			process< user_type::current_user, HANDLE, HANDLE > make_process(const std::string& image_name, const std::string& argument_name, HANDLE input, HANDLE output);
			
			process< user_type::current_user, HANDLE, HANDLE > make_process(const std::string& image_name, const std::string& argument_name, DWORD flags);

			template < typename _in, typename _out > process< user_type::impersonate_user, _in, _out > make_process_as_logon(const std::string& username, const std::string& password, const std::string& image_name, const std::string& argument_name);
			
			template < typename _in, typename _out > process< user_type::logon_user, _in, _out > make_process_as_logon(const std::string& username, const std::string& password, const std::string& image_name, const std::string& argument_name);

		}

		using namespace interface1;
	}

}

#endif