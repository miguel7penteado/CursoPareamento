#include "uint_array.h"

uint_array_t *
uint_array_new(size_t size) {
  uint_array_t *uint_array;

  uint_array = malloc(sizeof(uint_array_t));

  kv_init(*uint_array);

  kv_resize(uint32_t, *uint_array, size);

  return uint_array;
}

void
uint_array_push(uint_array_t *uint_array, uint32_t el) {
  kv_push(uint32_t, *uint_array, el);
}

void
uint_array_free(uint_array_t *uint_array) {
  kv_destroy(*uint_array);
  free(uint_array);
}
