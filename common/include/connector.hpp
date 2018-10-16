#pragma once
#if !defined(CONNECTOR_HPP)
#	include "definitions.hpp"
#	include <atomic>
#	include <string>
#	include <deque>
#	include <condition_variable>
#	include <mutex>
#	include <boost\asio.hpp>


using boost::asio::ip::tcp;

namespace networking {
	namespace ssl {


class connector
{
private:
	static constexpr const size_t helo_sz = 72;
	static constexpr const unsigned char helo[helo_sz] = { 
		0x80, 0x46,                                            // msg len
		0x01,                                                  // CLIENT_HELLO
		0x03, 0x01,                                            // SSL 3.1
		0x00, 0x2d,                                            // ciphersuite len
		0x00, 0x00,                                            // session id len
		0x00, 0x10,                                            // challenge len
		0x01, 0x00, 0x80, 0x03, 0x00, 0x80, 0x07, 0x00, 0xc0,  // ciphersuites
		0x06, 0x00, 0x40, 0x02, 0x00, 0x80, 0x04, 0x00, 0x80,  //
		0x00, 0x00, 0x04, 0x00, 0xfe, 0xff, 0x00, 0x00, 0x0a,  //
		0x00, 0xfe, 0xfe, 0x00, 0x00, 0x09, 0x00, 0x00, 0x64,  //
		0x00, 0x00, 0x62, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06,  //
		0x1f, 0x17, 0x0c, 0xa6, 0x2f, 0x00, 0x78, 0xfc,        // challenge
		0x46, 0x55, 0x2e, 0xb1, 0x83, 0x39, 0xf1, 0xea };
public:
	connector(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_it);

	void run_until_interrupted();

private:

	void connect();

	void interrupt(const boost::system::error_code& ec, int signal);
	
	void send_helo();

	void read_until_unexpected_close();


private:

	std::atomic<bool> running;
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::socket socket_;
	tcp::resolver::iterator iterator_;
	boost::asio::signal_set signals_;
	size_t output_sz_;
	unsigned char output_[4096];
	size_t input_sz_;
	unsigned char input_[4096];
	std::mutex mutex;
	std::condition_variable connection_in_progress;
};


	}
}
#endif