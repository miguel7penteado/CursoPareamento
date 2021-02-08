#include "mem.h"

void mem_print() {
  int who;
  struct rusage usage;

  who = RUSAGE_SELF;

  getrusage(who, &usage);

  printf_yellow("Memória utilizada: %luMB\n", usage.ru_maxrss / 1024);

}
