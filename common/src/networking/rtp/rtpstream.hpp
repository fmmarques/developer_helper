#pragma once
#if !defined(RTP_RTPSTREAM_HPP)
#	define RTP_RTPSTREAM_HPP

#	include "networking/rtp/rtp.h"
#	include <iostream>

namespace common {
	namespace networking {
		namespace rtp {
		
			struct empty_base {};

			template < uint16_t increment, uint16_t seed >
			class sequence_number_generator: empty_base
			{
			private:
				uint16_t present_increment;
				uint16_t used_seed;
			public:
				sequence_number_generator() 
				{ 
					present_increment = seed;
				}

				uint16_t next()
				{
					return present_increment += increment;
				}


			};

			template< class descriptor_t >
			struct protocol_traits: empty_base
			{
				using seq_num_generator = sequence_number_generator< descriptor_t::increment, descriptor_t::seed >
			};

			template< typename strategy_t >
			class rtpstream : public std::basic_stream< rtppacket, strategy_t >
			{
			private:
				strategy_t::seq_num_generator generator;
				strategy_t::
				SOCKET socket;
			public:
				rtpstream(SOCKET socket);
				std::rtpstream& operator<< (std::rtpstream& out, const rtpstream& packet) { throw "not implemented."; }
			};

		}
	}
}
#endif
