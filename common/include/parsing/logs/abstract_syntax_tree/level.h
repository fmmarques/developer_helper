#pragma once
#if !defined(COMMON_PARSING_LOGS_ABSTRACTSYNTAXTREE_LEVEL_H)
#	define COMMON_PARSING_LOGS_ABSTRACTSYNTAXTREE_LEVEL_H


namespace common {
	namespace parsing {
		namespace logs {
			namespace ast {
				namespace interface1 {

					enum class level { error, warning, informational, verbose, debug };

					level make_level(char level);

				}
			}
		}
	}
}

#endif