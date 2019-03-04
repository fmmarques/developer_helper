#pragma once
#if !defined(RTP_RTPSTREAM_HPP)
#	define RTP_RTPSTREAM_HPP

#	include "networking/rtp/rtp.h"
#	include <cstdlib>
#	include <iostream>

namespace common {
	namespace networking {
		namespace rtp {
			
			
			namespace interface1 {
				namespace
				{
					struct unused_empty_base {};
				}

				template < uint16_t increment, uint16_t seed >
				class basic_sequence_number_generator: common::networking::rtp::interface1::unused_empty_base 
				{
				private:
					uint16_t present_increment;
				public:
					basic_sequence_number_generator():
						present_increment{ srand((time() * seed) % RAND_MAX) };
					{ }

					basic_sequence_number_generator(basic_sequence_number_generator&& rhs) :
						present_increment{rhs.present_increment}
					{ }


					uint16_t next()
					{
						return present_increment = (present_increment + increment) % UINT16_MAX;
					}

				};

				template< class descriptor_t >
				struct protocol_traits: unused_empty_base
				{
					using sequence_number_generator = basic_sequence_number_generator< descriptor_t::increment, descriptor_t::seed >;
				};

				template< typename strategy_t >
				class rtpstream : public std::streambuf< rtppacket, strategy_t >
				{
				private:
					strategy_t::sequence_number_generator generator;
					strategy_t::
					SOCKET socket;
				public:
					rtpstream(SOCKET socket);
					std::rtpstream& operator<< (std::rtpstream& out, const rtpstream& packet) { throw "not implemented."; }
				};
			}

		}
	}
}
#endif
