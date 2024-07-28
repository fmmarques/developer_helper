/**
 * @file stack-machine.hpp
 * @brief Implements a stackable state machine.
 */
#pragma once
#if !defined(PATTERNS_STACK_MACHINE)
#    define PATTERNS_STACK_MACHINE

#include "definitions.hpp"
#include <system_error>

namespace softjenga {
    namespace patterns {
        namespace stack_machine {
            namespace v0_1_0 {

                /**
                 * @class errc
                 * @brief A group of possible errors
                 *
                 * Represents the errors that the state machine can produce.
                 */
                enum class errc {
        
                };


                /**
                 * @class state
                 * @brief Abstract representation of a state. 
                 *
                 * A state represents the status of an application, during a portion of the 
                 * app's lifecycle.
                 */

                struct state {
    
                    /**
                     * @brief A callback called once, and only once, the state is pushed onto the stack.
                     * @return Returns a status that is either 0, if successful, or one of errc conditions.
                     */ 
                    virtual std::error_code onPush() = 0;
                    
                    /**
                     * @brief A callback called onto the stack.
                     * @return Returns a status that is either 0, if successful, or one of errc conditions.
                     */ 
                    virtual std::error_code onEntry() = 0;
    
                    virtual std::error_code onPop() = 0;
                }; 

                struct stack_machine {
                    virtual std::error_code push(const state& state) = 0;

                };

            }
            using namespace v0_1_0;
        }
    }
}

#endif
