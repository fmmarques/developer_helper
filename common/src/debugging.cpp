#include <sstream>
#include <string>
#include <windows.h>

#	define _DEBUG
#	define USE_HEAP_CHECKS
#   define USE_BUFFER_CHECKS
#	define ERROR_SEVERITY 4
#	include "../../include/cppcommon/debugging.h"
#	undef _DEBUG

//static int (*pferror)(const char *msg, );
std::string GetLastErrorMessage(const char * const lpszFunction)
{

	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();
	std::stringstream oss;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);
	oss << lpszFunction << " failed with " << dw << ": " << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
	return oss.str();
}

/**
* Buffer validation section.
*/
//TAG:SECTION:BUFFER_VALIDATION
//TAG:SECTION:BUFFER_VALIDATION;SEGMENT:DEFINITION

void __fastcall _validate_buffer_int8_t(int8_t* vector, unsigned int vector_occupancy, unsigned int vector_capacity, const char * const file, unsigned int line)
{
	ASSERT(NULL != vector,
		"the buffer to be checked is null in " << file << "@" << line << '.');
	ASSERT(vector_capacity > 0,
		"the capacity of the buffer " << vector_capacity << " is less than 0 in " << file << "@" << line << '.');
	ASSERT(vector_occupancy >= 0,
		"the occupancy of the buffer " << vector_occupancy << " is less than 0 in " << file << "@" << line << '.');
	ASSERT(vector_capacity >= vector_occupancy,
		"the occupancy of the buffer " << vector_occupancy << " is bigger than the buffer's capacity " << vector_capacity << " in " << file << "@" << line << '.');
	ASSERT(GET_BUFFER_CAPACITY(vector) == vector_capacity,
		"the vector's capacity " << GET_BUFFER_CAPACITY(vector) << " does not coincide with the given capacity " << vector_capacity << " in " << file << "@" << line << '.');
	ASSERT(GET_BUFFER_OCCUPANCY(vector) == vector_occupancy,
		"the vector's occupancy " << GET_BUFFER_OCCUPANCY(vector) << " does not coincide with the given occupancy " << vector_occupancy << " in " << file << "@" << line << '.');
	ASSERT(GET_BUFFER_COOKIE(vector) == __COOKIE__,
		"the vector's cookie is corrupted in " << file << "@" << line << '.');
}

void __fastcall _validate_buffer_int16_t(int16_t* vector, unsigned int vector_occupancy, unsigned int vector_capacity, const char * const file, unsigned int line)
{
	ASSERT(NULL != vector,
		"the buffer to be checked is null in " << file << "@" << line << '.');
	ASSERT(vector_capacity > 0,
		"the capacity of the buffer " << vector_capacity << " is less than 0 in " << file << "@" << line << '.');
	ASSERT(vector_occupancy >= 0,
		"the occupancy of the buffer " << vector_occupancy << " is less than 0 in " << file << "@" << line << '.');
	ASSERT(vector_capacity >= vector_occupancy,
		"the occupancy of the buffer " << vector_occupancy << " is bigger than the buffer's capacity " << vector_capacity << " in " << file << "@" << line << '.');
	ASSERT(GET_BUFFER_CAPACITY(vector) == vector_capacity,
		"the vector's capacity " << GET_BUFFER_CAPACITY(vector) << " does not coincide with the given capacity " << vector_capacity << " in " << file << "@" << line << '.');
	ASSERT(GET_BUFFER_OCCUPANCY(vector) == vector_occupancy,
		"the vector's occupancy " << GET_BUFFER_OCCUPANCY(vector) << " does not coincide with the given occupancy " << vector_occupancy << " in " << file << "@" << line << '.');
	_validate_buffer_int8_t((int8_t *)vector, vector_occupancy, vector_capacity, file, line);
}

void __fastcall _validate_buffer_uint8_t(uint8_t* vector, unsigned int vector_occupancy, unsigned int vector_capacity, const char * const file, unsigned int line)
{
//	ASSERT(NULL != vector,
//		"the buffer to be checked is null in " << file << "@" << line << '.');
//	ASSERT(vector_capacity > 0,
//		"the capacity of the buffer " << vector_capacity << " is less than 0 in " << file << "@" << line << '.');
//	ASSERT(vector_occupancy >= 0,
//		"the occupancy of the buffer " << vector_occupancy << " is less than 0 in " << file << "@" << line << '.');
//	ASSERT(vector_capacity >= vector_occupancy,
//		"the occupancy of the buffer " << vector_occupancy << " is bigger than the buffer's capacity " << vector_capacity << " in " << file << "@" << line << '.');
//	ASSERT(GET_BUFFER_CAPACITY(vector) == vector_capacity,
//		"the vector's capacity " << GET_BUFFER_CAPACITY(vector) << " does not coincide with the given capacity " << vector_capacity << " in " << file << "@" << line << '.');
//	ASSERT(GET_BUFFER_OCCUPANCY(vector) == vector_occupancy,
//		"the vector's occupancy " << GET_BUFFER_OCCUPANCY(vector) << " does not coincide with the given occupancy " << vector_occupancy << " in " << file << "@" << line << '.');
	_validate_buffer_int8_t((int8_t*) vector, vector_occupancy, vector_capacity, file, line);
}


/**
 * Heap validation section.
 * 
 */
//TAG:SECTION:HEAP_VALIDATION
//TAG:SECTION:HEAP_VALIDATION;SEGMENT:DEFINITION

void __fastcall _validate_heap( IN HANDLE heap )
{
//	ASSERT( nullptr != heap,
//		    "a null heap was given to validate." );
	std::stringstream oss{};
	DWORD dwLastError = 0;
	PROCESS_HEAP_ENTRY heap_entry = { 0 };

	if (FALSE == HeapLock(heap))
	{
		logd(GetLastErrorMessage(__func__));
		return;
	} 
	

	while (HeapWalk(heap, &heap_entry) != FALSE) 
	{
		if ((heap_entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0) 
		{
			oss << "Allocated block";

			if ((heap_entry.wFlags & PROCESS_HEAP_ENTRY_MOVEABLE) != 0) 
			{
				oss << ", movable with HANDLE " << heap_entry.Block.hMem;
			}

			if ((heap_entry.wFlags & PROCESS_HEAP_ENTRY_DDESHARE) != 0) 
			{
				oss << ", DDESHARE";
			}
			oss << std::endl;
		}
		else if ((heap_entry.wFlags & PROCESS_HEAP_REGION) != 0) 
		{
			oss << "Region\n  " << heap_entry.Region.dwCommittedSize << " bytes committed " << heap_entry.Region.dwUnCommittedSize << " bytes uncommitted." << std::endl
				<< "\tFirst block address: " << heap_entry.Region.lpFirstBlock << std::endl
				<< "\tLast block address: " << heap_entry.Region.lpLastBlock << std::endl;
		}
		else if ((heap_entry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) != 0) 
		{
			oss << "Uncommitted range" << std::endl;
		}
		else 
		{
			oss << "Block" << std::endl;
		}

		oss << "  Data portion begins at: " << heap_entry.lpData << "\n" 
			<< "  Size: " << heap_entry.cbData << "bytes\n"
			<< "  Overhead: " << heap_entry.cbOverhead << "bytes\n"
			<< "  Region index: " << heap_entry.iRegionIndex << '\n' << std::endl;

		logd(oss.str());
		oss.str("");
	}

	if (dwLastError != ERROR_NO_MORE_ITEMS) 
		logd(GetLastErrorMessage("HeapWalk"));
	
	

	if (HeapUnlock(heap) == FALSE) 
		logd(GetLastErrorMessage("HeapUnlock"));
}


void atohex_int8_t(int8_t destination[], uint32_t* destination_length, uint32_t destination_buffer_length, const int8_t origin[], uint32_t origin_length)
{
	const static unsigned int SCALE = 3;
	const static unsigned int FOLDING = 16 * SCALE;
	const char* hex = { "0123456789ABCDEF" };
	unsigned int hex_pos = 0;
	uint32_t index = 0;
//	ASSERT(NULL != destination,
//		"the destination buffer is null.");
//	ASSERT(NULL != destination_length,
//		"the destination_length is null.");
//	ASSERT(destination_buffer_length > 0,
//		"the destination_buffer_length is less or equal to 0.");
//	ASSERT(NULL != origin,
//		"the origin buffer is null.");
//	ASSERT(origin_length > 0,
//		"the origin_length is less or euqal to 0.");
//	ASSERT(destination_buffer_length >= SCALE * origin_length + (origin_length % FOLDING),
//		"the destination buffer cannot hold the hexadecimal conversion of the present buffer.");
	
	for (index = 0; index < origin_length; index++)
	{
		auto pos = index * SCALE;
		hex_pos = ((origin[index] & 0xF0) >> 4);
		destination[pos] = hex[hex_pos];
		pos++;
		hex_pos = origin[index] & 0x0f;
		destination[pos] = hex[hex_pos];
		pos++;
		destination[pos] = ((pos > 0 && (pos+1) % FOLDING == 0) ? '\n' : ' ');
	}

	destination[index] = '\0';
	
	if (nullptr != destination_length)
		*destination_length = index;

}

void atohex_uint8_t(int8_t destination[], uint32_t* destination_length, uint32_t destination_buffer_length, const uint8_t original[], uint32_t original_length)
{
	atohex_int8_t(destination, destination_length, destination_buffer_length, (const int8_t*)original, original_length);
}

