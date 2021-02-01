#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "hash.h"
#include "array.h"
#include "colors.h"
#include "errors.h"
#include "csv.h"

typedef struct comparator_t {
  int exact, use_weight_table, field1;
  double u, m, missing, min_value_to_be_match, default_weight;
  double log2_m_u, log2_1m_1u;
  char *function;
  char *frequency_table_name;
  hash_t *frequency_table;
} comparator_t;

typedef struct classifier_t {
  int match_min;
  int not_match_max;
  array_t *comparators;
} classifier_t;

comparator_t *
comparator_new(int, int, double, double, double, int, char*, char *, double, double);
void comparator_free(comparator_t *);
void comparator_print(comparator_t *);
classifier_t *classifier_new(int, int);
void classifier_free(classifier_t *);
void classifier_print(classifier_t *);
void classifier_add_comparator(classifier_t *, comparator_t *);

#endif
