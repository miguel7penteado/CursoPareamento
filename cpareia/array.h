#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>
#include <strings.h>

#include "kvec.h"

typedef kvec_t(void *) array_t;

array_t *array_new(size_t);
array_t *array_new_zeroed(size_t);
void array_free(array_t *);
void array_push(array_t *, void *);

#define array_size(array) (kv_size(*array))
#define array_get(array, i) (kv_A(*array, i))

#endif
