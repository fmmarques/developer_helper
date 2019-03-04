#pragma once
#if !defined(COMMON_PARSING_LOGS_ABSTRACTSYNTAXTREE_LINE_H)
#	define COMMON_PARSING_LOGS_ABSTRACTSYNTAXTREE_LINE_H

#	include <string>

#	include <boost\spirit\include\qi.hpp>

#	include "parsing/logs/abstract_syntax_tree/timestamp_token.h"
#   include "parsing/logs/abstract_syntax_tree/expression.h"

namespace common {
	namespace parsing {
		namespace logs {
			namespace ast {
				namespace interface1 {


					struct line 
					{
						boost::variant<
							boost::nfs::nil,
							boost::recursive_wrapper< message >,
							boost::recursive_wrapper< exception_message >
						> type;


						line() :
							expr(nil())
						{}

						template < typename expression_t >
						expression(expression_t& expr) :
							expr(expr)
						{}

						type expr;

						timestamp_token timestamp;
						level_token level;
						unsigned int thread_id;
						unsigned int number_of_thread_on_NET_for_operations;
						unsigned int number_of_threads_on_NET_for_IO;
						unsigned int number_of_threads_available_for_operations_on_component;
						unsigned int number_of_threads_created_to_execute_operations_on_component;
						std::string instance;

						

					};
				}
			}
		}
	}
}
#endif