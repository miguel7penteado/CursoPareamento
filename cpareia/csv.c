#include "csv.h"

size_t
open_file(char *fname, char **buf) {
  int fd;
  struct stat fs;

	fd = open(fname, O_RDONLY);

	if (fd == -1)
		handle_error("Unable to open file %s\n", fname);

  if (fstat(fd, &fs) == -1)
    handle_error("fstat");

	*buf = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (*buf == MAP_FAILED)
    handle_error("mmap");

  if(madvise(*buf, fs.st_size, MADV_SEQUENTIAL) == -1)
    handle_error("madvise");

  close(fd);

  return fs.st_size;
}

csv_fields_t *
csv_fields_new(size_t num_fields) {
  csv_fields_t *csv_fields;

  csv_fields = malloc(sizeof(csv_fields_t));
  csv_fields->fields = malloc(sizeof(char *) * num_fields);
  csv_fields->size = num_fields;

  return csv_fields;
}

csv_row_t *
csv_row_new(char *begin, char *end) {
  csv_row_t *csv_row;

  csv_row = malloc(sizeof(csv_row_t));
  csv_row->begin = begin;
  csv_row->end = end;

  return csv_row;
}

void
csv_row_free(csv_row_t *csv_row) {
  free(csv_row);
}

void
csv_free(csv_t *csv) {
  munmap(csv->buf, csv->size);
  free(csv);
}

void
csv_fields_free(csv_fields_t *csv_fields) {
  free(csv_fields->fields);
  free(csv_fields);
}

void
csv_fields_deep_free(csv_fields_t *csv_fields) {
  size_t i, total;
  total = csv_fields->size;

  for(i = 0; i < total; i++) {
    free(csv_fields->fields[i]);
  }
  csv_fields_free(csv_fields);
}

csv_t *
csv_new(char *filename) {
  csv_t *csv;

  csv = malloc(sizeof(csv_t));

  csv->size = open_file(filename, &(csv->buf));
  csv->current = csv->buf;
  csv->end = csv->buf + csv->size - 1;

  return csv;
}

void
csv_row_print(csv_row_t *csv_row) {
  char *current;

  current = csv_row->begin;

  while(current <= csv_row->end) {
    printf("%c", *current);
    current++;
  }
  printf("\n");
}

int
csv_get_row(csv_t *csv, csv_row_t *csv_row) {
  char *begin, *end;

  if(csv->current == csv->end)
    return 0;

  begin = end = csv->current;

  while(*end != '\n')
    end++;

  csv_row->begin = begin;
  csv_row->end = end - 1;

  /* Skipping multiple \n's */
  while(*end == '\n' && end != csv->end)
    end++;

  csv->current = end;

  return 1;
}

void
csv_get_fields(csv_fields_t *csv_fields,csv_row_t *csv_row, char sep) {
  char *begin, *current;
  size_t i, size;

  i = 0;
  begin = current = csv_row->begin;

  while(current <= csv_row->end) {
    if(*current == sep || current == csv_row->end) {
      if(begin == current) {
        csv_fields->fields[i++] = strdup("\0");
      }
      else {
        size = (*current == sep) ? current - begin: current - begin + 1;
        csv_fields->fields[i++] = strndup(begin, size);
      }
      if(*current == sep && current == csv_row->end) {
        csv_fields->fields[i++] = strdup("\0");
      }
      begin = current + 1;
    }
    current++;
  }
}
