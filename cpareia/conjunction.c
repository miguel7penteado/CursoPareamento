#include "conjunction.h"

part_t *
part_new(int field, char *transform, int size) {
  part_t *part;

  part = malloc(sizeof(part_t));
  part->field = field;
  part->transform = transform ? strdup(transform) : NULL;
  part->size = size;

  return part;
}

void
part_print(part_t *part) {
  printf("part:\n");
  printf("    field: %d\n", part->field);
  printf("    transform: %s\n", part->transform);
  printf("    size: %d\n", part->size);
}

void
part_free(part_t *part) {
  free(part->transform);
  free(part);
}

conjunction_t *
conjunction_new(size_t size) {
  conjunction_t *conjunction;

  conjunction = malloc(sizeof(conjunction_t));
  conjunction->parts = array_new(size);

  return conjunction;
}

void
conjunction_add_part(
    conjunction_t *conjunction,
    int field,
    char *transform,
    int size) {
  part_t *part;
  part = part_new(field, transform, size);

  array_push(conjunction->parts, part);
}

void
conjunction_free(conjunction_t *conjunction) {
  size_t i;

  for(i = 0; i < array_size(conjunction->parts); i++) {
    part_free(array_get(conjunction->parts, i));
  }

  array_free(conjunction->parts);
  free(conjunction);
}

void
conjunction_print(conjunction_t *conjunction) {
  size_t i;

  for(i = 0; i < array_size(conjunction->parts); i++) {
    part_print(array_get(conjunction->parts, i));
  }
}
