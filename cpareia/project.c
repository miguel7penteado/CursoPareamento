#include "project.h"

project_t *
project_new() {
  project_t *project;

  project = malloc(sizeof(project_t));

  project->conjunctions = array_new(1);

  project->block = block_new();

  project->args = args_new();

  project->d0 = NULL;
  project->d1 = NULL;

  return project;
}

void
project_print(project_t *project) {
  size_t i;

  printf("Project: %s\n", project->name);
  printf("Task: %s\n", project->task);
  for(i = 0; i < array_size(project->conjunctions); i++) {
    printf("Conjunction %d:\n", (int) i);
    conjunction_print(array_get(project->conjunctions, i));
  }

  printf("\nClassifier\n");
  classifier_print(project->classifier);

  printf("\nD0:\n");
  database_print(project->d0);

  if(project->d1) {
    printf("\nD1:\n");
    database_print(project->d1);
  }

  printf("\nOutput:\n");
  output_print(project->output);
}

void
project_free(project_t *project) {
  size_t i;

  /* Output depends on everything else, so we need to lock here first */
  output_free(project->output);

  free(project->task);
  free(project->name);
  database_free(project->d0);
  database_free(project->d1);

  for(i = 0; i < array_size(project->conjunctions); i++) {
    conjunction_free(array_get(project->conjunctions, i));
  }

  array_free(project->works);

  block_free(project->block);
  array_free(project->conjunctions);
  classifier_free(project->classifier);
  args_free(project->args);
  free(project);
}

void
project_add_conjunction(project_t *project, conjunction_t *conjunction) {
  array_push(project->conjunctions, conjunction);
}

void
project_parse_classifier(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;
  xmlChar *match_min, *not_match_max, *m, *u, *missing, *function;
  xmlChar *field1, *default_weight;
  xmlChar *min_value_to_be_match, *use_weight_table, *frequency_table;
  int exact;
  char *child_name;
  xmlNodePtr child;
  comparator_t *comparator;

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/classifier",
      ctx);

  match_min = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "match-min-result");

  not_match_max = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "not-match-max-result");

  project->classifier = classifier_new(
      atoi((char *) match_min),
      atoi((char *) not_match_max));

  child = xpath->nodesetval->nodeTab[0]->children;

  while(child) {
    child_name = (char *) child->name;

    m = xmlGetProp(child, BAD_CAST "m");
    u = xmlGetProp(child, BAD_CAST "u");
    missing = xmlGetProp(child, BAD_CAST "missing");
    min_value_to_be_match = xmlGetProp(child, BAD_CAST "minValueToBeMatch");
    use_weight_table = xmlGetProp(child, BAD_CAST "use-weight-table");
    frequency_table = xmlGetProp(child, BAD_CAST "frequency-table");
    function = xmlGetProp(child,BAD_CAST "function");
    field1 = xmlGetProp(child, BAD_CAST "field1");
    default_weight = xmlGetProp(child, BAD_CAST "default-weight");

    if(!strcmp(child_name, "exact-string-comparator")) {
      exact = 1;
    } else if(!strcmp(child_name, "approx-string-comparator")) {
      exact = 0;
    } else {
      exact = -1;
    }

    if(exact != -1) {
      comparator = comparator_new(
          exact,
          use_weight_table ? (!strcmp((char *) use_weight_table, "true") ? 1 : 0) : 0,
          m ? atof((char *) m) : 0,
          u ? atof((char *) u) : 0,
          missing ? atof((char *) missing) : 0,
          project_get_field_id(project, (char *) field1),
          (char *) frequency_table,
          (char *) function,
          min_value_to_be_match ? atof((char *) min_value_to_be_match) : 0,
          default_weight ? atof((char *) default_weight) : 0);
      classifier_add_comparator(project->classifier, comparator);
    }

    free(m);
    free(u);
    free(field1);
    free(missing);
    free(min_value_to_be_match);
    free(default_weight);
    free(use_weight_table);

    child = child->next;
  }

  xmlXPathFreeObject(xpath);

  free(match_min);
  free(not_match_max);
}

void
project_parse_datasource(
    project_t *project,
    xmlXPathContextPtr ctx,
    int id) {
  xmlXPathObjectPtr xpath;
  xmlChar *filename, *sep, *rows;
  database_t *d;
  int i;
  char buffer[100];

  snprintf(buffer, 100, "/project/data-sources/data-source[@id=%d]", id);
  xpath = xmlXPathEvalExpression(BAD_CAST buffer, ctx);

  filename = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "file");

  sep = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "field-separator");

  rows = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "rows");

  xmlXPathFreeObject(xpath);

  snprintf(
      buffer,
      100,
      "/project/data-sources/data-source[@id=%d]/fields/field",
      id);
  xpath = xmlXPathEvalExpression(BAD_CAST buffer, ctx);

  if(!rows) {
    handle_error("Missing attribute 'rows'\n");
  }

  d = database_new(
      xpath->nodesetval->nodeNr,
      strtoull((char *)rows, NULL, 10));

  d->filename = (char *) filename;

  if(!strcmp((char *) sep, "\\t")) {
    d->sep = '\t';
  } else {
    d->sep = *sep;
  }

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    d->fields[i] = xmlGetProp(
        xpath->nodesetval->nodeTab[i],
        BAD_CAST "name");
  }

  if(id == 0) {
    project->d0 = d;
  } else {
    project->d1 = d;
  }

  xmlXPathFreeObject(xpath);
  free(sep);
  free(rows);
}

void
project_parse_datasources(project_t *project, xmlXPathContextPtr ctx) {
  project_parse_datasource(project, ctx, 0);
  if(strcmp((char *)project->task, "deduplication")) {
    project_parse_datasource(project, ctx, 1);
  }
}

void
project_parse_project(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;

  xpath = xmlXPathEvalExpression(BAD_CAST "/project", ctx);

  project->name = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "name");

  project->task = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "task");

  xmlXPathFreeObject(xpath);
}

void
project_parse_output(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;
  xmlChar *filename, *min, *max;

  xpath = xmlXPathEvalExpression(BAD_CAST "/project/output", ctx);

  filename = xmlGetProp(xpath->nodesetval->nodeTab[0], BAD_CAST "file");
  min = xmlGetProp(xpath->nodesetval->nodeTab[0], BAD_CAST "min");
  max = xmlGetProp(xpath->nodesetval->nodeTab[0], BAD_CAST "max");

  assert(xpath != NULL);
  assert(min != NULL);
  assert(max != NULL);

  project->output = output_new(
      (char *) filename,
      atof( (char *) min),
      atof( (char *) max));

  free(min);
  free(max);
  xmlXPathFreeObject(xpath);
}

void
project_parse_conjunctions(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;
  xmlNode *part_node;
  xmlChar *field_name, *transform, *size;
  conjunction_t *conjunction;
  int i, field;

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/blocking/conjunction",
      ctx);

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    conjunction = conjunction_new(1);

    part_node = xpath->nodesetval->nodeTab[i]->children;

    while(part_node) {
      if(!strcmp((char *) part_node->name, "part")) {
        field_name = xmlGetProp(part_node, BAD_CAST "field-name");
        transform  = xmlGetProp(part_node, BAD_CAST "transform");
        size = xmlGetProp(part_node, BAD_CAST "size");

        field = project_get_field_id(project, (char *) field_name);
        assert(field != -1);

        conjunction_add_part(
            conjunction,
            field,
            (char *) transform,
            size ? atoi((char *) size) : 0);

        free(field_name);
        free(transform);
        free(size);
      }
      part_node = part_node->next;
    }
    project_add_conjunction(project, conjunction);
  }

  xmlXPathFreeObject(xpath);
}

void
project_parse(project_t *project, char *file_name) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpath_ctx;

  xmlInitParser();

  if((doc = xmlParseFile(file_name)) == NULL)
    handle_error("Unable to parse XML\n");

  if((xpath_ctx = xmlXPathNewContext(doc)) == NULL)
    handle_error("Unable to create XPath\n");

  project_parse_project(project, xpath_ctx);
  project_parse_datasources(project, xpath_ctx);
  project_parse_conjunctions(project, xpath_ctx);
  project_parse_classifier(project, xpath_ctx);
  project_parse_output(project, xpath_ctx);

  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
  xmlCleanupParser();
}

int
project_get_field_id(project_t *project, char *field_name) {
  unsigned int i;
  int field;

  field = -1;

  for(i = 0; i < project->d0->num_fields; i++) {
    if(!strcmp((char *) field_name, (char *) project->d0->fields[i])) {
      field = i;
      break;
    }
  }

  if(field == -1) {
    handle_error("Field '%s' not found\n", field_name);
  }

  return field;
}
