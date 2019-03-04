#pragma once
#if !defined(COMMON_PARSING_LOGS_AST_TIMESTAMP_H)
#  define COMMON_PARSING_LOGS_AST_TIMESTAMP_H

#	include <cassert>

namespace common {
	namespace parsing {
		namespace logs {
			namespace ast {
				namespace interface1 {


					class timestamp_token 
					{
					protected:
						unsigned int year;
						unsigned char month;
						unsigned char day;
						unsigned char hour;
						unsigned char minute;
						unsigned char second;
						unsigned int millisecond;

					public:
						timestamp_token(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second, unsigned int millisecond) :
							year(year), month(static_cast<unsigned char>(month)), day(static_cast<unsigned char>(day)),
							hour(static_cast<unsigned char>(hour)), minute(static_cast<unsigned char>(minute)), second(static_cast<unsigned char>(second)),
							millisecond(millisecond)
						{
							assert(1970 <= year && year <= 2100 && "Invalid year specification. 1970 <= year <= 2100.");
							assert(1 <= month && month <= 12 && "Invalid month specification. 1 <= month <= 12.");
							assert(1 <= day && ((!(month == 2 && year % 4 == 0) || day <= 29) ||
								(!(month == 2 && year % 4 != 0) || day <= 28) ||
								(!(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 11) || day <= 31) ||
								(!(month == 4 || month == 6 || month == 9 || month == 11) || day <= 30)) &&
								"Invalid day specification. 1 <= day <= 28, if february; 1 <= day <= 29, if february in leap year;\
1 <= day <= 30, if april, june, september or november; 1 <= day <= 31 otherwise.");
						}

						template < class visitor_t > void accept(visitor_t& visitor)
						{
							//nothing to do.
						}
					};
				}
			}
		}
	}
}


#endif