#pragma once
#if !defined(CPP_COMMON_RTP)
#	define CPP_COMMON_RTP

#include <ostream>
#include <cstdint>
#include <winsock2.h>


namespace common {

//#if !defined(_WIN64)
//#error "The current implementation does not support 32b arch"
//#endif
	namespace interface1
	{

#		define VERSION_BITLENGTH 2
#		define VERSION_MASK 0xC0
#		define VERSION_OFFSET 0

#		define PADDING_BITLENGTH 1
#		define PADDING_MASK 0x20
#		define PADDING_OFFSET 2

#		define EXTENSION_BITLENGTH 1
#		define EXTENSION_MASK 0x10
#		define EXTENSION_OFFSET 2

#		define CSRC_COUNT_BITLENGTH 4
#		define CSRC_COUNT_MASK 0xf
#		define CSRC_COUNT_OFFSET 3

#		define MARKER_BITLENGTH 1
#		define MARKER_MASK 0x80
#		define MARKER_OFFSET 0

#		define PAYLOAD_BITLENGTH 1
#		define PAYLOAD_MASK 0x80
#		define PAYLOAD_OFFSET 0

		struct empty_base { };

		/// <summary>Representation of an RTP packet's header.</summary>
		struct rtphdr: empty_base
		{
			union 
			{
				uint8_t first_octet;
				uint8_t version : VERSION_BITLENGTH;
				uint8_t padding : PADDING_BITLENGTH;
				uint8_t extension : EXTENSION_BITLENGTH;
				uint8_t csrc_count : CSRC_COUNT_BITLENGTH;
			};
			union 
			{
				uint8_t second_octet;
				uint8_t marker : MARKER_BITLENGTH;
				uint8_t payload_type : PAYLOAD_BITLENGTH;
			};
			uint16_t sequence_number;
			uint32_t timestamp;
			uint32_t ssrc;
			uint32_t *csrc_list;
		};
		
		static_assert(sizeof(struct rtphdr) >= 12, "The RTP header does not match the RFC 3550.");

		inline uint8_t version(const rtphdr& hdr);
		inline void version(rtphdr& hdr, uint8_t version);

		inline uint8_t padding(const rtphdr& hdr);
		inline void padding(rtphdr& hdr, uint8_t padding);

		inline uint8_t extension(const rtphdr& hdr);
		inline void extension(rtphdr& hdr, uint8_t extension);

		inline uint8_t csrc_count(const rtphdr& hdr);
		inline void csrc_count(struct rtphdr& hdr, uint8_t csrc_count);

		inline uint8_t marker(const rtphdr& hdr);
		inline void marker(rtphdr& hdr, uint8_t marker);

		inline uint8_t payload_type(const rtphdr& hdr);
		inline void payload_type(rtphdr&, uint8_t payload_type);

		inline uint16_t sequence_number(const rtphdr& hdr);
		inline void sequence_number(rtphdr& hdr, uint16_t sequence_number);

		inline uint32_t timestamp(const rtphdr& hdr);
		inline void timestamp(struct rtphdrt& hdr, uint32_t timestamp);

		inline uint32_t ssrc(const struct rtphdr& hdr);
		inline void ssrc(struct rtphdr& hdr, uint32_t ssrc); 

		inline uint32_t* csrc_list(const struct rtphdr& hdr, uint32_t*& list, uint8_t& list_element_count);
		inline void csrc_list(struct rtphdr& hdr, uint32_t* list, uint8_t list_element_count);

		
		struct rtppacket : public rtphdr
		{
			uint8_t *payload;
		
			rtppacket():
				rtphdr{}, 
				payload{nullptr}
			{};

			friend std::ostream & operator << (std::ostream& out, const rtppacket& packet);
			friend std::istream & operator >> (std::istream& in, rtppacket& packet);
		};

		
		


	}
}
#endif