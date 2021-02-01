#include "block.h"

block_t *
block_new() {
  block_t *block;

  block = malloc(sizeof(block_t));

  block->blocks = kh_init(string);
  pthread_mutex_init(&block->mutex, NULL);

  return block;
}

void
block_insert(block_t *block, char *key, uint32_t val) {
  int absent;
  uint_array_t *array;
  khint_t k;

  pthread_mutex_lock(&block->mutex);

  k = kh_put(string, block->blocks, key, &absent);

  if(absent) {
    kh_key(block->blocks, k) = strdup(key);
    kh_value(block->blocks, k) = uint_array_new(1);
  }

  k = kh_get(string, block->blocks, key);

  array = kh_val(block->blocks, k);

  uint_array_push(array, val);

  pthread_mutex_unlock(&block->mutex);
}

size_t
block_size(block_t *block) {
  return kh_size(block->blocks);
}

void
block_foreach(block_t *block, block_foreach_fn fn, void *data) {
  uint_array_t *array;
  const char *key;

  kh_foreach(block->blocks, key, array, fn(key, array, data));
}

void
block_internal_remove(
    block_t *block,
    const char *key,
    uint_array_t *array) {
  khint_t k;

  k = kh_get(string, block->blocks, key);
  kh_del(string, block->blocks, k);
  free((char *) key);
  uint_array_free(array);
}

void
block_foreach_remove(block_t *block, block_foreach_rm_fn fn, void *dt) {
  uint_array_t *array;
  const char *key;

  kh_foreach(
      block->blocks,
      key,
      array,
      if(fn(key, array, dt))
      block_internal_remove(block, key, array)
      );
}

int
block_internal_free(const char *key, uint_array_t *array, void *data) {
  (void) key;
  (void) array;
  (void) data;

  return 1;
}

void
block_free(block_t *block) {
  block_foreach_remove(block, block_internal_free, NULL);

  kh_destroy(string, block->blocks);

  free(block);
}
