#pragma once
#if !defined(COMMON_SECURITY_TOKEN_HPP)
#	define COMMON_SECURITY_TOKEN_HPP 

#include <Windows.h>

namespace common {
	namespace security {
		namespace interface1 {

			class token
			{
			protected:
				HANDLE _token;
			public:
				token(const HANDLE& token);
				virtual ~token();


			};
		}
	}
}
#endif