#ifndef _ARGS_H_
#define _ARGS_H_

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

typedef struct args_t {
  char *project_file;
  char *blocking_file;
  int max_threads;
} args_t;

args_t *args_new();
void args_free(args_t *);
void args_parse(args_t *, int, char **);

#endif
