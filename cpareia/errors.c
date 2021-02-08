#include "errors.h"

void handle_error(const char *fmt, ...) {
  va_list ap;

  printf(ANSI_COLOR_RED);

  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);

  printf(ANSI_COLOR_RESET);

  exit(-1);
}

void
check_file(char *file) {
  if(access(file, R_OK) == -1) {
    handle_error("File '%s' does not exist\n", file);
  }
}
