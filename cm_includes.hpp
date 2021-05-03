#ifndef cm_includes_hpp
#define cm_includes_hpp

#include <stdio.h>
#include <string.h>
#include <map>
#include <string>
#include "cfg_manipulator.hpp"

#if defined(WIN32) || defined(_WIN32) \
	|| defined(__WIN32) && !defined(__CYGWIN__)
	#include <io.h>
#else
	#include <unistd.h>
#endif

#include <limits.h>
#include <algorithm>
#include <locale>
#include <vector>

using namespace std;

#endif	// cm_includes_hpp