#ifndef DEBUG_H_
#define DEBUG_H_


#ifdef DEBUG

// Relative file name string, i.e. without path.
#define  __FILENAME__  (basename(__FILE__))

/**
 * Define the different colors prints
 */
#define DEBUG_BLACK     "\33[22;30m"
#define DEBUG_GRAY      "\33[01;30m"
#define DEBUG_RED       "\33[22;31m"
#define DEBUG_LRED      "\33[01;21m"
#define DEBUG_GREEN     "\33[22;32m"
#define DEBUG_LGREEN    "\33[01;32m"
#define DEBUG_BLUE      "\33[22;34m"
#define DEBUG_LBLUE     "\33[01;34m"
#define DEBUG_BROWN     "\33[22;33m"
#define DEBUG_YELLOW    "\33[00;33m"
#define DEBUG_CYAN      "\33[22;36m"
#define DEBUG_LCYAN     "\33[22;36m"
#define DEBUG_MAGENTA   "\33[22;35m"
#define DEBUG_LMAGENTA  "\33[01;35m"
#define DEBUG_NC        "\33[0m"
#define DEBUG_BOLD      "\33[1m"
#define DEBUG_ULINE     "\33[4m" //underline
#define DEBUG_BLINK     "\33[5m"
#define DEBUG_INVERT    "\33[7m"

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <cstring>
#include <assert.h>
#include <iostream>
#include <stdio.h>


// common stuff
//
#define ASSERT(x)   assert(x);


#define debugOPTIMIZATION(format, ...) {fprintf(stderr, DEBUG_ULINE DEBUG_INVERT "DEBUG[%s, %s, %d]: ", \
                 __FILENAME__, __FUNCTION__, __LINE__); \
                fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}

#define debugERROR(format, ...) {fprintf(stderr, DEBUG_RED DEBUG_INVERT "DEBUG[%s, %s, %d]: ", \
                     __FILE__, __FUNCTION__, __LINE__); \
                    fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}

#define debugWARNING(format, ...) {fprintf(stderr, DEBUG_ULINE DEBUG_RED "DEBUG[%s, %s, %d]: ", \
                     __FILE__, __FUNCTION__, __LINE__); \
                    fprintf(stderr, format "\33[0m", ## __VA_ARGS__);}


#else
    #define ASSERT(x)
    #define debugOPTIMIZATION(format, ...)
    #define debugERROR(format, ...)
    #define debugWARNING(format, ...)

#endif

#endif /* DEBUG_H_ */
