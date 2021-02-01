#include "output.h"

gzFile
output_get_file(output_t *output, int file) {
  return output->files[file];
}

void
output_write(
    output_t *output,
    char *id1,
    char *id2,
    char status,
    double score,
    double *scores,
    int num_scores,
    int file_id) {
  int i;
  gzFile file;

  if(score < output->min || score > output->max) {
    return;
  }

  file = output_get_file(output, file_id);

  gzprintf(file, "%c %c %s %s %f", status, 'X', id1, id2, score);

  for(i = 0; i < num_scores; i++) {
    gzprintf(file, " %f", scores[i]);
  }
  gzprintf(file, "\n");
}

output_t *
output_new(char *filename, double min, double max) {
  output_t *output;

  output = malloc(sizeof(output_t));

  output->filename = filename;
  output->min = min;
  output->max = max;
  output->files = NULL;
  output->num_files = 0;

  return output;
}

void
output_open_files(output_t *output, int num_files) {
  int i;
  size_t size;
  char *name;

  size = strlen(output->filename);

  name = malloc(sizeof(char) * (size + 10));

  output->num_files = num_files;
  output->files = malloc(sizeof(gzFile) * num_files);

  for(i = 0; i < num_files; i++) {
    sprintf(name, "%s%d.gz", output->filename, i);

    if(!(output->files[i] = gzopen(name, "wb"))) {
      handle_error("Erro ao abrir arquivo %s\n", name);
    }
  }

  free(name);
}

void
output_print(output_t *output) {
  printf("  Filename: %s\n", output->filename);
  printf("  Num files: %d\n", output->num_files);
  printf("  Min: %f\n", output->min);
  printf("  Max: %f\n", output->max);
}

void
output_free(output_t *output) {
  int i;

  for(i = 0; i < output->num_files; i++) {
    gzclose(output->files[i]);
  }
  free(output->files);

  free(output->filename);
  free(output);
}
