#include "classifier.h"

void
comparator_read_frequency_table(comparator_t *comparator) {
  char *key, *name;
  double *field_value;
  csv_t *csv;
  csv_row_t *csv_row;
  csv_fields_t *csv_fields;

  name = comparator->frequency_table_name;

  csv = csv_new(name);
  csv_row = csv_row_new(NULL, NULL);

  comparator->frequency_table = hash_new();

  while(csv_get_row(csv, csv_row)) {
    csv_fields = csv_fields_new(4);

    csv_get_fields(csv_fields, csv_row, '\t');

    field_value = malloc(sizeof(double));

    key = csv_fields->fields[3];
    *field_value = atof(csv_fields->fields[2]);

    hash_insert(comparator->frequency_table, key, field_value);
    csv_fields_deep_free(csv_fields);
  }

  csv_row_free(csv_row);
  csv_free(csv);
}

comparator_t *
comparator_new(
    int exact,
    int use_weight_table,
    double m,
    double u,
    double missing,
    int field1,
    char *frequency_table_name,
    char *function,
    double min_value_to_be_match,
    double default_weight) {
  comparator_t *comparator;

  comparator = malloc(sizeof(comparator_t));

  comparator->exact = exact;
  comparator->use_weight_table = use_weight_table;
  comparator->m = m;
  comparator->u = u;
  comparator->missing = missing;
  comparator->field1 = field1;
  comparator->function = function;
  comparator->min_value_to_be_match = min_value_to_be_match;
  comparator->default_weight = default_weight;
  comparator->frequency_table_name = frequency_table_name;

  comparator->log2_m_u = log2(m / u);
  comparator->log2_1m_1u = log2(1 - m / 1 - u);

  if(use_weight_table) {
    if(frequency_table_name && strlen(frequency_table_name)) {
      comparator_read_frequency_table(comparator);
    } else {
      handle_error("use-weight-table is true but no table name given\n");
    }
  }

  return comparator;
}

void
comparator_print(comparator_t *comparator) {
  printf("  Exact: %d\n", comparator->exact);
  printf("  Use_weight_table: %d\n", comparator->use_weight_table);
  printf("  M: %f\n", comparator->m);
  printf("  U: %f\n", comparator->u);
  printf("  Missing: %f\n", comparator->missing);
  printf("  Field1: %d\n", comparator->field1);

  /*
   *How to Get a "Name" for the Hash. Is it needed?
   *printf("  Frequency_table: %s\n", comparator->frequency_table);
  */
  printf("  Function: %s\n", comparator->function);
  printf("  Min_value_to_be_match: %f\n", comparator->min_value_to_be_match);
  printf("  Default_weight: %f\n", comparator->default_weight);
  printf("  Log2(m, u): %f\n", comparator->log2_m_u);
  printf("  Log2(1 - m, 1 - u): %f\n", comparator->log2_1m_1u);
}

void
comparator_array_free(const char *key, array_t *array, void *data) {
  size_t i, total;
  (void) key;
  (void) data;

  total = array_size(array);

  for(i = 0; i < total; i++) {
    free(array_get(array, i));
  }
}

void
comparator_free(comparator_t *comparator) {
  free(comparator->function);
  free(comparator->frequency_table_name);

  if(comparator->use_weight_table) {
    hash_foreach(comparator->frequency_table, comparator_array_free, 0);
    hash_free(comparator->frequency_table);
  }

  free(comparator);
}

classifier_t *
classifier_new(int match_min, int not_match_max) {
  classifier_t *classifier;
  classifier = malloc(sizeof(classifier_t));

  classifier->match_min = match_min;
  classifier->not_match_max = not_match_max;

  classifier->comparators = array_new(1);

  return classifier;
}

void
classifier_add_comparator(
    classifier_t *classifier,
    comparator_t *comparator) {
    array_push(classifier->comparators, comparator);
}

void
classifier_free(classifier_t *classifier) {
  size_t i;

  for(i = 0; i < array_size(classifier->comparators); i++) {
    comparator_free(array_get(classifier->comparators, i));
  }
  array_free(classifier->comparators);

  free(classifier);
}

void
classifier_print(classifier_t *classifier) {
  size_t i;

  printf("    match_min: %d\n", classifier->match_min);
  printf("    not_match_max: %d\n", classifier->not_match_max);

  printf("\nComparators:");
  for(i = 0; i < array_size(classifier->comparators); i++) {
    printf("\n  Comparator %d\n", (int) i);
    comparator_print(array_get(classifier->comparators, i));
  }
}
