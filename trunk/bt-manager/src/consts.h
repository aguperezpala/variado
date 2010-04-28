#ifndef CONSTS_H
#define CONSTS_H

/*! Maximo numero de conexiones por dongle */
#define MAX_CON_PER_DONGLE	8

/*! Funcion que determina minimo y maximo */
#ifndef min
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#undef min
#endif
#ifndef max
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))
#undef max
#endif

#endif
