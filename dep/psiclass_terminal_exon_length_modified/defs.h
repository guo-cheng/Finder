#ifndef _LSONG_CLASSES_DEFS_HEADER
#define _LSONG_CLASSES_DEFS_HEADER

#include <stdint.h>

#define MAX_SEG_COUNT 127

struct _pair
{
	int64_t a, b ;
} ;

struct _pair32
{
	int a, b ;
} ;

/*char nucToNum[26] = { 0, -1, 1, -1, -1, -1, 2, 
	-1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, 3,
	-1, -1, -1, -1, -1, -1 } ;

char numToNuc[26] = {'A', 'C', 'G', 'T'} ;*/
#endif
