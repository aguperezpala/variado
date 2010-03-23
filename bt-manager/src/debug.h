#ifndef DEBUG_H
#define DEBUG_H

/* definimos algunas funciones */
#ifdef __DEBUG
#include <assert.h>
#include <errno.h>

/*! para imprimir un error del sistema */
	#define printErr(x)		\
		fprintf(stderr, x "\nErrCode %d: %s\n", errno, strerror(errno));

	#define debugp(x)	\
		printf("%s:" x "\t%d\n", __FUNCTION__,__LINE__)
	#define ASSERT(x)	assert(x)
#else
	#define debugp(x)	
	#define ASSERT(x)	
#endif

#endif
