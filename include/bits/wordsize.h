
#ifndef __QUARK_BITS_WORDSIZE_H
#define __QUARK_BITS_WORDSIZE_H 1


#if defined(_WIN32) || defined(WIN32)
	#define __WORDSIZE 32
#elif defined(_WIN64) || defined(WIN64)
	#define __WORDSIZE 64
#endif


#endif /** __QUARK_BITS_WORDSIZE_H */
