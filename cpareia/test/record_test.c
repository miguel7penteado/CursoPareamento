#include "record_test.h"

void test_record() {
  record_t *record;
  int i;
  char *fields[] = {"field1", "field2", "field3", "field4"};
  int tam = 4;

  record = record_new(tam);

  assert_int_equal(record->num_fields, tam);

  for(i = 0; i < tam; i++) {
    record_add_field(record, fields[i]);
  }

  for(i = 0; i < tam; i++) {
    assert_int_equal(strcmp(record_get_field(record, i), fields[i]), 0);
  }

  record_free(record);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_record),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
