#include "args.h"

args_t *
args_new() {
  args_t *args;

  args = malloc(sizeof(args_t));
  args->project_file = NULL;

  args->max_threads = sysconf(_SC_NPROCESSORS_ONLN);

  args->blocking_file = NULL;

  return args;
}

void
args_free(args_t *args) {
  free(args->project_file);
  free(args->blocking_file);
  free(args);
}

void
args_print_usage() {
  printf("Usage: ./cpareia -p project_file [-t max_threads] ");
  printf("[-b blocking_file]\n");
  exit(-1);
}

void
args_parse(args_t *args, int argc, char *argv[]) {
  int c, max_threads;
  struct option options[] = {
    {"project",   required_argument, 0, 'p'},
    {"threads",   optional_argument, 0, 't'},
    {"blocking",  optional_argument, 0, 'b'},
    {0, 0, 0, 0}
  };
  static char optstr[] = "p:t:b:";

  while ((c = getopt_long(argc, argv, optstr, options, NULL)) != -1) {
    switch(c) {
      case 'p':
        args->project_file = strdup(optarg);
        check_file(args->project_file);
        break;
      case 't':
        if((max_threads = atoi(optarg)) > 1) {
          args->max_threads = atoi(optarg);
        } else {
          printf("max_threads deve ser maior que um.\n");
          printf("Usando valor padrão de %d\n", args->max_threads);
        }
        break;
      case 'b':
        args->blocking_file = strdup(optarg);
        check_file(args->blocking_file);
        break;
      case '?':
      default:
        args_print_usage();
    }
  }
  if(!args->project_file) {
    args_print_usage();
  }
}
