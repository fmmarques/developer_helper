#pragma once
#if !defined(COMMON_PARSING_LOGS_ABSTRACTSYNTAXTREE_EXPRESSION)
#	define COMMON_PARSING_LOGS_ABSTRACTSYNTAXTREE_EXPRESSION

namespace common {
	namespace parsing {
		namespace logs {
			namespace ast {
				namespace interface1 {

					struct expression
					{
						boost::variant< 
							nil,
							boost::recursive_wrapper< expression >,
							boost::recursive_wrapper< line >
						> type;

						expression(): 
							expr(nil())
						{}

						template < typename expression_t >
						expression(expression_t& expr) :
							expr(expr)
						{}

						type expr;

					};
				}
			}
		}
	}
}
#endif
