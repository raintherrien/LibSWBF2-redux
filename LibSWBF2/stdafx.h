#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Don't include rarely used windows stuff
#include <windows.h>


// TODO: Place additional headers here
#include <string>
#include <vector>
#include <fstream>
#include <type_traits>
#include <cmath>
#include <algorithm>

namespace LibSWBF2
{
	// we don't want to lift up everything from std!
	// so just lift up what we need
	using std::clamp;
	using std::string;
	using std::vector;
	using std::unique_ptr;

	// define types
	typedef int32_t ChunkHeader;
	typedef int32_t ChunkSize;		// might be an unsigned int32? further investigation needed!
}