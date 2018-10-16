#include <iostream>
#include <condition_variable>
#include <mutex>
#include "connector.hpp"

namespace networking {
	namespace ssl {


connector::connector( boost::asio::io_service& io_service,
					  tcp::resolver::iterator endpoint_it  ):
	running(true),
	io_service_(io_service),
	socket_(io_service),
	signals_(io_service, SIGINT),
	iterator_(endpoint_it),
	output_sz_(4096),
	output_(),
	input_sz_(4096),
	input_()
{
	output_sz_ = std::min<>(helo_sz, output_sz_);
	std::copy_n(helo, output_sz_, output_);
	signals_.async_wait([this](const boost::system::error_code& ec, int signal) { interrupt(ec, signal); });
}

void connector::connect()
{
	std::wcout << "Connecting to " << iterator_->host_name().data() << ":" << iterator_->service_name().data();
	boost::asio::async_connect( socket_, 
							    iterator_,
								[this](boost::system::error_code ec, tcp::resolver::iterator it)
								{
									if (!running)
										return;
								
									if (!ec)
										send_helo();
									else
									{ 
										std::wcout << "... failed. Trying again." << std::endl;
										std::unique_lock< std::mutex > l(mutex);
										socket_.close();
										connection_in_progress.notify_one();
									}
								}
	);
}


void connector::interrupt(const boost::system::error_code& ec, int signal)
{
	while (!running.exchange(false))
		;
	std::unique_lock< std::mutex > l(mutex);
	if (socket_.is_open())
		socket_.close();
	connection_in_progress.notify_one();
	std::wcout << std::endl << "Interrupting!" << std::endl;

}

void connector::send_helo()
{
	if (!running)
		return;
	std::wcout << " successful." << std::endl << "Sending HELO...";;
	boost::asio::async_write(	socket_,
								boost::asio::buffer(output_, output_sz_),
								[this](boost::system::error_code ec, std::size_t length)
								{
									if (!ec)
									{
										std::cout << "successful." << std::endl;
										read_until_unexpected_close();
									}
									else
									{
										std::cout << "failed. Closing the socket to try again." << std::endl;
										std::unique_lock< std::mutex > l(mutex);
										socket_.close();
										connection_in_progress.notify_one();
									}
								}
	);
}

void connector::read_until_unexpected_close()
{
	if (!running)
		return;
	boost::asio::async_read(	socket_,
								boost::asio::buffer(input_, input_sz_),
								[this](boost::system::error_code ec, std::size_t length)
								{
									if (!running)
										return;

									if (!ec)
										read_until_unexpected_close();
									else
									{ 
										std::cout << "The connection was unexpectedly closed. Retrying the connection." << std::endl;
										std::unique_lock< std::mutex > l(mutex);
										socket_.close();
										connection_in_progress.notify_one();
									}
								}
	);
}




void connector::run_until_interrupted()
{
	std::unique_lock<std::mutex> lock(mutex);
	std::cout << "Running sequential connections to " << iterator_->host_name() << " until interrupted... (press Ctrl-C to interrupt)" << std::endl;
	while (running)
	{
		connect();
		while (socket_.is_open())
			connection_in_progress.wait(lock);
	}
}

	}
}