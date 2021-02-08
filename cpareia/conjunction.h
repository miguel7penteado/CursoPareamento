#ifndef _CONJUNCTION_H_
#define _CONJUNCTION_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "array.h"

typedef struct part_t {
  int field, size;
  char *transform;
} part_t;

typedef struct conjunction_t {
  array_t *parts;
} conjunction_t;

part_t *part_new(int, char *, int);
void part_free(part_t *);
void part_print(part_t *);
conjunction_t *conjunction_new(size_t);
void conjunction_free(conjunction_t *);
void conjunction_add_part(conjunction_t *, int, char *, int);
void conjunction_print(conjunction_t *);

#endif
