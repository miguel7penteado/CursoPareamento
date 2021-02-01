#include "record.h"

record_t *
record_new(uint8_t num_fields) {
  record_t *record;

  record = malloc(sizeof(record_t));
  record->num_fields = num_fields;
  record->_used_fields = 0;
  record->_size = 0;
  record->_indexes = malloc(sizeof(uint8_t) * num_fields);
  record->_fields = NULL;

  return record;
}

void
record_add_field(record_t *record, char *field) {
  size_t size;

  assert(record->_used_fields < record->num_fields);

  size = strlen(field) + 1;

  record->_indexes[record->_used_fields++] = size;

  record->_fields = realloc(record->_fields, record->_size + size);

  memcpy(record->_fields + record->_size, field, size);
  record->_size += size;
}

char *
record_get_field(record_t *record, uint8_t field) {
  int i;
  uint8_t acc = 0;

  for(i = 0; i < field; i++) {
    acc += record->_indexes[i];
  }

  return record->_fields + acc;
}

void
record_free(record_t *record) {
  free(record->_fields);
  free(record->_indexes);
  free(record);
}

void
record_print(record_t *record) {
  int i;

  printf("FIELDS:\n");

  for(i = 0; i < record->_used_fields; i++) {
    printf("\t%d: %s\n", i, record_get_field(record, i));
  }
}
