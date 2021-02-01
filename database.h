#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

#include "record.h"
#include "errors.h"
#include "array.h"
#include "csv.h"

typedef struct database_t {
  array_t *records;
  unsigned char **fields, sep;
  size_t num_fields, num_rows;
  char *filename;
} database_t;

database_t *database_new(int, size_t);
void database_free(database_t *);
void database_read(database_t *);
pthread_t *database_read_async(database_t *);
void database_print(database_t *);

#endif
