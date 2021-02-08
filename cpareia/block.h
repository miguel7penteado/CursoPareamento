#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdint.h>
#include <pthread.h>
#include "khash.h"
#include "uint_array.h"

KHASH_MAP_INIT_STR(string, uint_array_t *)

typedef void (*block_foreach_fn)(const char *, uint_array_t *, void *);
typedef int (*block_foreach_rm_fn)(const char *, uint_array_t *, void *);

typedef struct block_t {
  khash_t(string) *blocks;
  pthread_mutex_t mutex;
} block_t;

block_t *block_new();
void block_insert(block_t *, char *, uint32_t);
void block_free(block_t *);
size_t block_size(block_t *);

void block_foreach(block_t *, block_foreach_fn, void *);
void block_foreach_remove(block_t *, block_foreach_rm_fn, void *);

#endif
