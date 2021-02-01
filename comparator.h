#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include "field_comparator.h"
#include "classifier.h"
#include "record.h"
#include "project.h"
#include "output.h"

#define between(val, min, max) val >= min && val <= max
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

typedef struct work_t {
  uint_array_t *array;
  int start, step;
} work_t;

typedef struct comparator_pthread_params_t {
  project_t *project;
  int id;
  int num_threads;
} comparator_pthread_params_t;

work_t *work_new(uint_array_t *, int, int);
void work_free(work_t *);

pthread_t **comparator_run_async(project_t *);
#endif
