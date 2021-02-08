#include "hash.h"

hash_t *
hash_new() {
  hash_t *hash;

  hash = malloc(sizeof(hash_t));

  hash->table = kh_init(str);

  pthread_mutex_init(&hash->mutex, NULL);

  return hash;
}

int
hash_internal_free(const char *key, array_t *array, void *data) {
  (void) key;
  (void) array;
  (void) data;
  return 1;
}

void
hash_free(hash_t *hash) {
  hash_foreach_remove(hash, hash_internal_free, NULL);

  kh_destroy(str, hash->table);

  free(hash);
}

void
hash_insert(hash_t *hash, char *key, void *record) {
  int absent;
  array_t *array;
  khint_t k;

  pthread_mutex_lock(&hash->mutex);

  k = kh_put(str, hash->table, key, &absent);

  if(absent) {
    kh_key(hash->table, k) = strdup(key);
    kh_value(hash->table, k) = array_new(1);
  }

  k = kh_get(str, hash->table, key);

  array = kh_val(hash->table, k);

  array_push(array, record);

  pthread_mutex_unlock(&hash->mutex);
}

array_t *
hash_get(hash_t *hash, char *key) {
  khint_t k;

  k = kh_get(str, hash->table, key);
  return (k == kh_end(hash->table) ? NULL : kh_val(hash->table, k));
}

void
hash_print_pair(const char *key, array_t *array, void *data) {
  (void) data;
  size_t i;
  printf("  %s =>\n", key);

  for(i = 0; i < array_size(array); i++) {
    record_print(array_get(array, i));
  }
}

void
hash_internal_remove(hash_t *hash, const char *key, array_t *array) {
  khint_t k;

  k = kh_get(str, hash->table, key);
  kh_del(str, hash->table, k);
  free((char *) key);
  array_free(array);
}

void
hash_foreach_remove(hash_t *hash, hash_foreach_rm_fn fn, void *data) {
  array_t *array;
  const char *key;

  kh_foreach(
      hash->table,
      key,
      array,
      if(fn(key, array, data))
      hash_internal_remove(hash, key, array)
      );
}

void
hash_foreach(hash_t *hash, hash_foreach_fn fn, void *data) {
  array_t *array;
  const char *key;

  kh_foreach(hash->table, key, array, fn(key, array, data));
}

size_t
hash_size(hash_t *hash) {
  return kh_size(hash->table);
}

void
hash_print(hash_t *hash) {
  printf("{\n");
  hash_foreach(hash, hash_print_pair, NULL);
  printf("}\n");
}
