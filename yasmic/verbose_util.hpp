/*
 * verbose_util.hpp
 * David Gleich
 * Stanford University
 * 17 March 2006
 */

/**
 * @file verbose_util.hpp
 * Add defines to control verbose statement in the code.
 */

//
// this file is somewhat strange.  There are two sections; the first section 
// simple makes a few define statements
//
#ifdef YASMIC_VERBOSE_UTIL_DEFINE
int verbose;
#endif // YASMIC_VERBOSE_UTIL_DEFINE

#ifndef YASMIC_VERBOSE_UTIL
#define YASMIC_VERBOSE_UTIL

#ifdef YASMIC_USE_VERBOSE

	#define YASMIC_VERBOSE \
		if (verbose)

#else

	#define YASMIC_VERBOSE \
		if (0)

#endif 




#endif // YASMIC_VERBOSE_UTIL

