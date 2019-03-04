#include "parsing/logs/abstract_syntax_tree/level.h"

namespace common {
	namespace parsing {
		namespace logs {
			namespace ast {
				namespace interface1 {



					level make_level(char level) 
					{
						
						switch (level)
						{
						case 'E':
							return level::error;
						case 'W':
							return level::warning;
						case 'I':
							return level::informational;
						case 'V':
							return level::verbose;
						case 'D':
							return level::debug;
						default:
							return level::informational;
						}
					}

				}
			}
		}
	}
}