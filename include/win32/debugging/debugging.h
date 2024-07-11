/* OneDebug library
 * Written by fmmarques
 * 
 * SINOPSYS
 *	This library can be used to help in debugging a Windows application.
 *	
 * ;TLDR;
 * Change the compilation settings to include:
 *	/GS - enhanced security checks
 *	/RTC - Runtime checks
 */

//TAG:SECTION:GLOBAL
//TAG:SECTION:GLOBAL:SEGMENT:DEFINES:OFFSET:ERRORS
#define LOG_CRITICAL_MESSAGES_BY_DEFAULT
//#undef LOG_CRITICAL_MESSAGES_BY_DEFAULT														// Uncomment this line to suppress all debug information.
#define LOG_DEBUG_		0
#define LOG_VERBOSE_		1
#define LOG_INFO_		2
#define LOG_WARNING_		3
#define LOG_ERROR_		4
#define LOG_CRITICAL_	5
#define LOG_WTF_			6  // What Terrible Failure ...


//TAG:SECTION:GLOBAL:SEGMENT:DEFINES:OFFSET:HEAP
//#	define USE_HEAP_CHECKS
# define USE_BUFFER_CHECKS

#pragma once
#if !defined(ONEDEBUG_H)
#	define ONEDEBUG_H
#	if defined(_DEBUG)
#		pragma strict_gs_check(on)
#	endif
#	include <iostream>
#	include <sstream>
#	include <string>
#	include <chrono>
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>

// Function templates
//   Unary
#define DECLARE_UNARY_TEMPLATE(Ret,Attr,Fn,Ty,...)\
Ret Attr Fn_##Ty(__VA_ARGS__)

#define MARK_AS_UNUSED(x)\
	(void)(x);

// Function templates
#define DEFINE_UNARY_TEMPLATE(Ret,Attr,Fn,Ty,Body,...)\
DECLARE_UNARY_TEMPLATE(Ret,Attr,Fn,Ty,__VA_ARGS__)\
{\
	Body;;\
}

//DECLARE_FUNCTION(std::string, , GetLastErrorMessage, void, const std::string& lpszFunction);
std::string GetLastErrorMessage(const char * const lpszFunction);


#	if defined(_DEBUG)  // Change the Configuration Properties\ C/C++ \Code Generation\Runtime Library
#		define CTASTR2(pre,post) pre ## post
#		define CTASTR(pre,post) CTASTR2(pre,post)
#		define STATIC_ASSERT(cond,msg) \
typedef struct { int CTASTR(static_assertion_failed_,msg) : !!(cond); } \
CTASTR(static_assertion_failed_,__COUNTER__)
#		define ASSERT(X,Y) { std::stringstream oss; oss << __FILE__ << "::" << __func__ << "(" << __LINE__ << "): " << Y; \
if (!(X)) { OutputDebugStringA(oss.str().c_str()); throw std::logic_error(oss.str()); } }
#	else
#		define STATIC_ASSERT(X,Y) ((__assume(X)))
#		define ASSERT(X,Y) ((__assume(X))) // When not in debug, this will actually make code faster. So do not delete ASSERTs
#	endif

#	if defined(_DEBUG)
#		define NOEXCEPT
#	else
#		define NOEXCEPT noexcept
#	endif

/**
 * Error logging section. 
 */
//TAG:SECTION:ERROR_LOGGING
//TAG:SECTION:ERROR_LOGGING;SEGMENT:DEFINES
#	if defined(LOG_CRITICAL_MESSAGES_BY_DEFAULT) && !defined(ERROR_SEVERITY)
#		define ERROR_SEVERITY V_ERROR
#	endif
#
#	if defined(ERROR_SEVERITY)
#		define logm(T,M)\
if (T>=ERROR_SEVERITY)\
{\
std::stringstream __oss; \
__oss << "[OneSIPConnector][" << (T==LOG_WTF_?"WTF":(T==LOG_CRITICAL_?"CRIT":(T==LOG_ERROR_?"ERR":(T==LOG_WARNING_?"WARN":(T==LOG_INFO_?"INFO":(T==LOG_VERBOSE_?"VERBOSE":(T==LOG_DEBUG_?"DEBUG":"NO_PRINT"))))))) << "] " << __FILE__ << "::" << __func__ << "(" << __LINE__ << ", " << GetCurrentThreadId() << "): " << M;\
OutputDebugStringA(__oss.str().c_str());\
}
#		if (ERROR_SEVERITY <= LOG_WTF_)
#			define wtf(M) logm(LOG_WTF_,M)
#			if (ERROR_SEVERITY <= LOG_CRITICAL_)
#				define logc(M) logm(LOG_CRITICAL_,M)
#				if (ERROR_SEVERITY <= LOG_ERROR_)
#					define loge(M) logm(LOG_ERROR_,M)
#					if (ERROR_SEVERITY <= LOG_WARNING_)
#						define logw(M) logm(LOG_WARNING_,M)
#						if (ERROR_SEVERITY <= LOG_INFO_)
#							define logi(M) logm(LOG_INFO_,M)
#							if (ERROR_SEVERITY <= LOG_DEBUG_)
#								define logd(M) logm(LOG_DEBUG_, M)
#							else
#								define logd(M) 
#							endif
#						else
#							define logd(M) 
#							define logi(M) 
#						endif
#					else
#						define logd(M) 
#						define logi(M) 
#						define logw(M) 
#					endif
#				else
#					define logd(M) 
#					define logi(M) 
#					define logw(M) 
#					define loge(M) 
#				endif
#			else
#				define logd(M) 
#				define logi(M) 
#				define logw(M) 
#				define loge(M) 
#				define logc(M) 
#			endif
#		else
#			define logd(M) 
#			define logi(M) 
#			define logw(M) 
#			define loge(M) 
#			define logc(M) 
#			define wtf(M) 
#		endif
#	else
#		define logd(M) 
#		define logi(M) 
#		define logw(M) 
#		define loge(M) 
#		define logc(M) 
#		define wtf(M) 
#	endif // ERROR_SEVERITY

/**
* buffer validation section.
*/
//TAG:SECTION:BUFFER_VALIDATION
//TAG:SECTION:BUFFER_VALIDATION;SEGMENT:DEFINES
#	if defined(USE_BUFFER_CHECKS)
#		define __COOKIE__ ((int8_t)(0xDEADBEEF & SCHAR_MAX))
typedef struct {
	union 
	{ 
		uint16_t data[2];
		uint32_t mask; 
	};
} __buff_h, *__pbuff_h, **__lpbuff_h;

#		define __CAPACITY  0
#		define __OCCUPANCY 1

#		define __HDR_SIZE__ sizeof(__buff_h)
#		define __COOKIE_SIZE__ sizeof(__COOKIE__) 
#		define __SET_BUFFER_PROPERTY(V,P,v)\
((__pbuff_h)(((uint8_t *)&V[0]) - __HDR_SIZE__))->data[P]=v

#		define __GET_BUFFER_PROPERTY(V,P)\
(((__pbuff_h)(((uint8_t *)&V[0]) - __HDR_SIZE__))->data[P])

/* Sets a buffer's cookie.
 * arg V - a buffer.
 */
/*ASSERT(__GET_BUFFER_PROPERTY(V,__CAPACITY) >= __GET_BUFFER_PROPERTY(V,__OCCUPANCY), "the buffer's occupancy exceeds buffer's capacity." ); \*/
#		define SET_BUFFER_COOKIE(V)\
V[__GET_BUFFER_PROPERTY(V,__OCCUPANCY)] = __COOKIE__

#		define GET_BUFFER_COOKIE(V)\
(int8_t)(V[__GET_BUFFER_PROPERTY(V,__OCCUPANCY)]&__COOKIE__) 

 /* Gets a buffer's cookie.
  * arg V - a buffer.
  */
#		define CHECK_BUFFER_COOKIE(V)\
ASSERT(__GET_BUFFER_PROPERTY(V,__CAPACITY)>=__GET_BUFFER_PROPERTY(V,__OCCUPANCY),"the buffer's occupancy exceeds buffer's capacity."); \
ASSERT(GET_BUFFER_COOKIE(V)==__COOKIE__,"cookie failed")

  /* Sets a buffer's capacity.
  * arg V - a buffer.
  * arg C - an integer representing the buffer's capacity.
  */
#		define SET_BUFFER_CAPACITY(V,C)\
__SET_BUFFER_PROPERTY(V,__CAPACITY,C)

  /* Gets a buffer's capacity.
   * arg V - a buffer.
   */
/*
ASSERT(__GET_BUFFER_PROPERTY(V,__CAPACITY) >= __GET_BUFFER_PROPERTY(V,__OCCUPANCY), "the buffer's occupancy exceeds buffer's capacity." ); \
ASSERT(__GET_BUFFER_PROPERTY(V,__OCCUPANCY) ==__COOKIE__,"cookie failed"); \
*/
#		define GET_BUFFER_CAPACITY(V)\
__GET_BUFFER_PROPERTY(V,__CAPACITY)

/*
ASSERT( GET_BUFFER_CAPACITY(V) >= __GET_BUFFER_PROPERTY(V,__OCCUPANCY), "the buffer's occupancy exceeds buffer's capacity." ); \
ASSERT(__GET_BUFFER_PROPERTY(V,__OCCUPANCY) ==__COOKIE__,"cookie failed"); \
*/
#		define GET_BUFFER_OCCUPANCY(V)\
__GET_BUFFER_PROPERTY(V,__OCCUPANCY)

/*
ASSERT( GET_BUFFER_CAPACITY(V) >= GET_BUFFER_OCCUPANCY(V), "the buffer's occupancy exceeds buffer's capacity." ); \
ASSERT( GET_BUFFER_CAPACITY(V) >= O, "the new buffer's occupancy exceeds buffer's capacity." ); \
*/
#		define SET_BUFFER_OCCUPANCY(V,O)\
__SET_BUFFER_PROPERTY(V,__OCCUPANCY,O);


#		define DECLARE_ARRAY(T,V,C)\
T __ ## V[ C + __HDR_SIZE__ + __COOKIE_SIZE__ ] = { 0 };\
T * V = &__ ## V[ __HDR_SIZE__ ];\
SET_BUFFER_CAPACITY(V,C);\
SET_BUFFER_OCCUPANCY(V,0);\
SET_BUFFER_COOKIE(V)

//DECLARE_UNARY_TEMPLATE(void,__fastcall,_validate_buffer,int16_t,unsigned int occupancy_of_vector, unsigned int length_of_vector);
void __fastcall _validate_buffer_int16_t(int16_t* vector, unsigned int occupancy_of_vector, unsigned int length_of_vector, const char * const file, unsigned int line);
//DECLARE_UNARY_TEMPLATE(void,__fastcall,_validate_buffer,int8_t,unsigned int occupancy_of_vector,unsigned int length_of_vector);
void __fastcall _validate_buffer_int8_t(int8_t* vector, unsigned int occupancy_of_vector, unsigned int length_of_vector, const char * const file, unsigned int line);
//DECLARE_UNARY_TEMPLATE(void,__fastcall,_validate_buffer,int8_t,unsigned int occupancy_of_vector,unsigned int length_of_vector);
void __fastcall _validate_buffer_uint8_t(uint8_t* vector, unsigned int occupancy_of_vector, unsigned int length_of_vector, const char * const file, unsigned int line);

#define validate_buffer(T,V,O,C,F,L)\
_validate_buffer_ ## T (V, (unsigned int) O, (unsigned int) C, F, L)


void atohex_uint8_t(int8_t destination[], uint32_t* destination_length, uint32_t destination_buffer_length, const uint8_t original[], uint32_t original_length);
void atohex_int8_t(int8_t destination[], uint32_t* destination_length, uint32_t destination_buffer_length, const int8_t original[], uint32_t original_length);

#define atohex(T,D,DL,DBL,O,OL)\
atohex_ ## T (D, DL, DBL, O, OL)


/*
#define atohex(D,DL,DBL,O,OL) _Generic((O), int8_t*: atohex_int8_t, \
											uint8_t*: atohex_uint8_t, \
											int16_t*: atohex_int16_t, \
											uint16_t*: atohex_uint16_t, \
											int32_t*: atohex_int32_t, \
											uint32_t*: atohex_uint32_t, \
											int64_t*: atohex_int64_t, \
											uint64_t*: atohex_uint64_t, \
											default: atohex_FAIL)(D,DL,DBL,O,OL)
*/

#	else
#		define DECLARE_ARRAY(T,V,S,F,L)\
T V[S] = { 0 };

#define validate_buffer(T,V,O,C,F,L) logd("Buffer validation on " #V " skipped.")

#	endif // USE_BUFFER_CHECKS


/**
* buffer validation section.
*/
//TAG:SECTION:HEAP_VALIDATION
//TAG:SECTION:HEAP_VALIDATION;SEGMENT:DEFINES
#	if defined(USE_HEAP_CHECKS)
void __fastcall _validate_heap( IN HANDLE );
#		define validate_heap() \
_validate_heap(GetProcessHeap())
#	else
#		define validate_heap()	//logd("Heap validation skipped.")
#	endif // USE_HEAP_CHECKS


#endif // eof


/*
PARA O STACKTRACE STACK TRACE
StackTrace^ st = gcnew StackTrace(true);
for (int i = 0; i< st->FrameCount; i++)
{
	// Note that high up the call stack, there is only
	// one stack frame.
	StackFrame^ sf = st->GetFrame(i);
	wtf("Frame " << i << ": " << marshal_as<  std::string >(sf->GetFileName()->ToString()) << ", " << marshal_as< std::string >(sf->GetMethod()->ToString()) << ", line " << sf->GetFileLineNumber());
}
*/