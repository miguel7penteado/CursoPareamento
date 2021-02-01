#ifndef _RECORD_H_
#define _RECORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

typedef struct record_t {
  uint8_t num_fields, _used_fields, *_indexes;
  size_t _size;
  char *_fields;
} record_t;

record_t *record_new(uint8_t);
void record_free(record_t *);
void record_add_field(record_t *, char *);
char *record_get_field(record_t *, uint8_t);
void record_print(record_t *);

#endif
