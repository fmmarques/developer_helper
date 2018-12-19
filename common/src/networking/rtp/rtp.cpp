#include "networking/rtp/rtp.h"

namespace common {
	namespace interface1 
	{
	
		inline uint8_t version(const struct _rtphdr& hdr) { return ((hdr.first_octet & VERSION_MASK) >> (sizeof(UINT8_MAX) - VERSION_OFFSET - VERSION_BITLENGTH)); }
		inline void version(struct _rtphdr& hdr, uint8_t version) { hdr.first_octet |= ((version << (sizeof(UINT8_MAX) - VERSION_BITLENGTH)) & VERSION_MASK); }

		inline uint8_t padding(const struct _rtphdr& hdr) { return ((hdr.first_octet & PADDING_MASK) >> (sizeof(UINT8_MAX) - PADDING_OFFSET - PADDING_BITLENGTH)); }
		inline void padding(struct _rtphdr& hdr, uint8_t padding) { hdr.first_octet |= ((padding << (sizeof(UINT8_MAX) - PADDING_BITLENGTH - PADDING_OFFSET)) & PADDING_MASK); }

		inline uint8_t extension(const struct _rtphdr& hdr) { return ((hdr.first_octet & EXTENSION_MASK) >> (sizeof(UINT8_MAX) - EXTENSION_OFFSET - EXTENSION_BITLENGTH)); }
		inline void extension(struct _rtphdr& hdr, uint8_t extension) { throw "not implemented."; }
		
		inline uint8_t csrc_count(const struct _rtphdr& hdr) { return ((hdr.first_octet & CSRC_COUNT_MASK) >> (sizeof(UINT8_MAX) - CSRC_COUNT_OFFSET - CSRC_COUNT_BITLENGTH)); }
		inline void csrc_count(struct _rtphdr& hdr, uint8_t count) { throw "not implemented.";  }
		
		inline uint8_t marker(const struct _rtphdr& hdr) { return ((hdr.second_octet & MARKER_MASK) >> (sizeof(UINT8_MAX) - MARKER_OFFSET - MARKER_BITLENGTH)); }
		
		
		inline uint8_t payload_type(const struct _rtphdr& hdr) { return ((hdr.second_octet & PAYLOAD_MASK) >> (sizeof(UINT8_MAX) - PAYLOAD_OFFSET - PAYLOAD_BITLENGTH)); }
	
	}
}