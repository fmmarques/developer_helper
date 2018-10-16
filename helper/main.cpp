#include "definitions.hpp"

#include <iostream>
#include <connector.hpp>
#include <Windows.h>

#include <boost\asio.hpp>

#include <boost\program_options.hpp>
#include <process.hpp>

using networking::ssl::connector;
using boost::asio::ip::tcp;
namespace po = boost::program_options;


void print_help();
int main(int argc, const char *argv[])
try
{
	int port{ -1 };
	std::string host{ "" };

	std::wcerr << "A1" << std::endl;
	process< user_type::current_user, HANDLE, HANDLE > process = make_process("c:\\windows\\explorer.exe", "%LOCALAPPDATA%", 0x00);
	std::wcerr << "A2" << std::endl;
	return 1;
	
	po::options_description options("Possible arguments");
	options.add_options()
		("help,?", "Help message")
		("host,h", po::value< std::string >(), "The destination's hostname.")
		("ip,i", po::value< std::string >(), "The destination's ip.")
		("port,p", po::value< int >(&port)->default_value(-1), "The destination's port.");
	;
	/*
	po::positional_options_description positional;
	positional.add("host", 0);
	positional.add("port", -1);
	*/


	po::variables_map vm;

	try 
	{
		//po::store(po::command_line_parser(argc, argv).options(options).positional(positional).run(), vm);
		po::store(po::parse_command_line(argc, argv, options), vm);
		if (vm.count("help"))
			return print_help(), ERROR_SUCCESS;
		
		po::notify(vm);
	}
	catch (const po::error& e)
	{
		std::wcerr << "error: " << e.what() << std::endl;
	}
	
	if (vm.count("ip") && vm.count("host"))
		throw std::invalid_argument("cannot specify both remote ip and remote hostame.");
	
	if (!(vm.count("ip") || vm.count("host")))
		throw std::invalid_argument("must specify either remote ip or hostame.");


	if (vm.count("ip")) 
	{
		host = vm["ip"].as< std::string >();
	}

	if (vm.count("port"))
	{
		port = vm["port"].as< int >();
	}
	else {
		throw std::invalid_argument("must specify remote port.");
	}

	if (vm.count("host"))
	{
		host = vm["host"].as< std::string >();
	}


	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	auto endpoint_iterator = resolver.resolve({ host, std::to_string(port) });

	
	connector conn{ io_service, endpoint_iterator };
	std::thread service([&io_service]() { io_service.run(); });
	std::thread th_conn([&io_service, &conn]() { conn.run_until_interrupted(); });

	service.join();
	th_conn.join();
	return ERROR_SUCCESS;
} 
catch (const std::invalid_argument& e)
{
	std::wcerr << "error: " << e.what() << std::endl;
	print_help();
}
catch (const std::exception& e)
{
	std::cerr << "Unhandled exception at main: " << e.what() << ", application will now exit." << std::endl;
	return ERROR_UNHANDLED_EXCEPTION;
}



void print_help()
try
{
	wchar_t file_path[MAX_PATH];
	::GetModuleFileNameW(nullptr, file_path, MAX_PATH);
	std::wstring tool_name(file_path);
	tool_name = tool_name.substr( tool_name.find_last_of(L"\\")+1, tool_name.find_last_of(L".") );
	std::wcerr << L"Usage: " << tool_name << L"[options] --port=<remote port>" << std::endl;
	std::wcerr << L"options:\n  --help\t: this help message.\n  --hostname\t: a FQHN of the remote peer or,\n  --ip\t: an IPv4 of the remote peer.\n\nrequired:\n  --port\t: the remote port." << std::endl;
}
catch (const std::exception& e)
{
	std::wcerr << "Unhandled exception at print_help: " << e.what() << ", application will now exit." << std::endl;
}