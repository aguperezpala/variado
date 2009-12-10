#ifndef DEBUG_H
#define DEBUG_H

/* definimos algunas funciones */
#ifdef __DEBUG
#include <assert.h>

#define debugp(x)	\
	printf("%s:" x "\t%d\n", __FUNCTION__,__LINE__)
#define debugp2(x)	printf(x)
#define ASSERT(x)	assert(x)
#else
#define debugp(x)	
#define ASSERT(x)	
#endif

#endif
