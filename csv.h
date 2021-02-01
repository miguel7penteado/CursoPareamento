#ifndef _CSV_H_
#define _CSV_H_

#include <stdlib.h>

#include "database.h"
#include "errors.h"

typedef struct csv_t {
  size_t size, num_fields;
  char *buf;
  char *end;
  char *current;
} csv_t;

typedef struct csv_row_t {
  char *begin;
  char *end;
} csv_row_t;

typedef struct csv_fields_t {
  char **fields;
  size_t size;
} csv_fields_t;

csv_t *csv_new(char *);
int csv_get_row(csv_t *, csv_row_t *);
void csv_get_fields(csv_fields_t *, csv_row_t *, char);
void csv_free(csv_t *);
void csv_fields_deep_free(csv_fields_t *);
void csv_row_free(csv_row_t *);
void csv_fields_free(csv_fields_t *);
csv_row_t *csv_row_new(char *, char *);
csv_fields_t *csv_fields_new(size_t);

#endif
