#include "service.h"
#include "exceptions.h"


#include <Windows.h>

namespace
{
	token::token():
		process_handle(nullptr),
		process_primary_token_handle(nullptr)
	{
		bool has_opened_process_token = false;

		process_handle = GetCurrentProcess();



		has_opened_process_token = OpenProcessToken(process_handle, TOKEN_QUERY, &process_primary_token_handle);
		if (!has_opened_process_token)
		{
			throw fmmarques::devhelper::couldnt_open_process_token();
		}

	};

}


namespace fmmarques {
	namespace devhelper {
		namespace service {
			namespace interface1 {

				void service_control::adjust_privilege()
				{}

				service_control::service_control():
					service_control_manager_handle(NULL)
				{
					char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
					u_long computer_names_length = 0;
					
					if (false == GetComputerName(computer_name, &computer_names_length))
						throw std::runtime_error("Couldn't obtain the computer's name.");
			
					computer_name[computer_names_length] = '\0';

					service_control_manager_handle = OpenSCManager(computer_name, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
					if (NULL == service_control_manager_handle)
					{
						;
					}
				}


				service_control::~service_control()
				{
				}

			}
		}
	}
}