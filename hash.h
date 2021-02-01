#ifndef _HASH_H_
#define _HASH_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "khash.h"
#include "array.h"
#include "record.h"

KHASH_MAP_INIT_STR(str, array_t *)

typedef void (*hash_foreach_fn)(const char *, array_t *, void *);
typedef int (*hash_foreach_rm_fn)(const char *, array_t *, void *);

typedef struct hash_t{
  khash_t(str) *table;

  pthread_mutex_t mutex;

} hash_t;

hash_t* hash_new();

void hash_free(hash_t*);
void hash_insert(hash_t*, char *, void *);
void hash_print(hash_t*);
void hash_foreach(hash_t *, hash_foreach_fn, void *);
void hash_foreach_remove(hash_t *, hash_foreach_rm_fn, void *);
size_t hash_size(hash_t *);
array_t *hash_get(hash_t *, char *);

#endif
