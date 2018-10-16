#if !defined(FMMARQUES_DEVHELPER_SERVICE_CONTROL)
#	define FMMARQUES_DEVHELPER_SERVICE_CONTROL
#	

#include <windows.h>

namespace 
{
	class token 
	{
	private:
		HANDLE process_handle;
		HANDLE process_primary_token_handle;
	public:
		token();
		~token();
	};
}

namespace fmmarques {
	namespace devhelper {
		namespace service {
			namespace interface1 {


class service;
class service_control
{
public:
	service_control();
	~service_control();

	void install(const service&);
protected:
	static void adjust_privilege();
private:
	SC_HANDLE service_control_manager_handle;
};

class service
{
public:
	service();
	~service();

	void on_start();
	void on_stop();

};
			}

		}
	}
}

#endif