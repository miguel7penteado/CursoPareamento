#ifndef _FIELD_COMPARATOR_H_
#define _FIELD_COMPARATOR_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MIN3(a, b, c) (a < b ? ( a < c ? a : c) : (b < c ? b : c))

void brsoundex(char *, char *, size_t);
double winkler(char *, char *);

#endif
