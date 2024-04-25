#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grep_flags_s {
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int e;
  int f;
} grep_flags_t;

typedef struct data_s {
  char** pattern;
  int pattern_size;
  char** file;
  int file_size;
} data_t;

int get_input(const int argc, char* argv[], data_t* data, grep_flags_t* flag);
void grep(data_t data, grep_flags_t flag);
void free_data(data_t* data);

int main(int argc, char* argv[]) {
  grep_flags_t flags = {0};
  data_t data = {0};
  int err = get_input(argc, argv, &data, &flags);
  if (!err) {
    grep(data, flags);
    free_data(&data);
  }
}

int get_flags(const char option, grep_flags_t* flag) {
  int response = 0;
  switch (option) {
    case 'i':
      flag->i = 1;
      break;
    case 'v':
      flag->v = 1;
      break;
    case 'c':
      flag->c = 1;
      break;
    case 'l':
      flag->l = 1;
      break;
    case 'n':
      flag->n = 1;
      break;
    case 'h':
      flag->h = 1;
      break;
    case 's':
      flag->s = 1;
      break;
    case 'o':
      flag->o = 1;
      break;
    case 'e':
      break;
    case 'f':
      break;
    default:
      printf("grep: illegal option -- %c\n", option);
      response = -1;
      break;
  }
  return response;
}

int get_patterns(data_t* data, const char* pattern) {
  int response = 0;
  char** new_data =
      (char**)realloc(data->pattern, ++data->pattern_size * sizeof(char*));
  if (new_data) {
    data->pattern = new_data;
    data->pattern[data->pattern_size - 1] = strdup(pattern);
  } else {
    printf("Realloc error");
    response = -1;
  }
  return response;
}

int get_f_patterns(data_t* data, char* file_name) {
  int response = 0;
  FILE* file = fopen(file_name, "r");
  if (file) {
    char* buff = NULL;
    size_t str_len = 0;
    ssize_t nread;
    while ((nread = getline(&buff, &str_len, file)) != -1) {
      if (buff[strlen(buff) - 1] == '\n' && strlen(buff) > 1) {
        buff[strlen(buff) - 1] = '\0';
      }
      if (get_patterns(data, buff)) {
        break;
      }
    }
    free(buff);
    fclose(file);
  } else {
    response = -1;
    fprintf(stderr, "grep: %s: No such file or directory\n", file_name);
  }

  return response;
}

int get_files(data_t* data, const char* file_name) {
  int response = 0;
  char** new_data =
      (char**)realloc(data->file, ++data->file_size * sizeof(char*));
  if (new_data) {
    data->file = new_data;
    data->file[data->file_size - 1] = strdup(file_name);
  } else {
    printf("Realloc error");
    response = -1;
  }
  return response;
}

void summer_vibe(const int argc, char* argv[], int* i, grep_flags_t* flag,
                 int* exit_loop, data_t* data) {
  if (argv[(*i)][1] == 'e' || argv[(*i)][1] == 'f') {
    ++(*i);
    if (*i < argc) {
      if (argv[(*i) - 1][1] == 'e') {
        if (get_patterns(data, argv[(*i)])) {
          *exit_loop = 1;
        }
      } else if (argv[(*i) - 1][1] == 'f') {
        if (get_f_patterns(data, argv[(*i)])) {
          *exit_loop = 1;
        }
      }
    }
  } else {
    for (size_t j = 1; j < strlen(argv[(*i)]); ++j) {
      if (get_flags(argv[(*i)][j], flag)) {
        *exit_loop = 1;
      }
    }
  }
}

int get_input(const int argc, char* argv[], data_t* data, grep_flags_t* flag) {
  int response = 0;
  int exit_loop = 0;
  for (int i = 1; i < argc && !exit_loop; ++i) {
    if (argv[i][0] == '-') {
      summer_vibe(argc, argv, &i, flag, &exit_loop, data);
    } else if (data->pattern_size == 0) {
      if (get_patterns(data, argv[i])) {
        exit_loop = 1;
      }
    } else {
      if (get_files(data, argv[i])) {
        exit_loop = 1;
      }
    }
  }
  return response;
}

void flag_o(data_t data, regex_t* regex, char* buff, FILE* file) {
  const char* shift = buff;
  regmatch_t match;
  for (int i = 0; i < data.pattern_size; ++i) {
    while (regexec(&regex[i], shift, 1, &match, 0) == 0) {
      printf("%.*s", (int)(match.rm_eo - match.rm_so), shift + match.rm_so);
      shift += match.rm_eo;
      if (!feof(file)) {
        printf("\n");
      }
    }
  }
}

void print_line(char* file_name, int line_count, char* buff, grep_flags_t flag,
                data_t data, FILE* file) {
  if (data.file_size > 1 && !flag.h) {
    printf("%s:", file_name);
  }
  if (flag.n) {
    printf("%d:", line_count);
  }
  printf("%s", buff);
  if (feof(file) && (!flag.l)) {
    printf("\n");
  }
}

void grep_file(char* file_name, data_t data, grep_flags_t flag,
               regex_t* regex) {
  FILE* file = fopen(file_name, "r");
  if (file) {
    char* buff = NULL;
    size_t str_len = 0;
    ssize_t nread;
    int f_find = 0;
    int match_count = 0;
    int line_count = 0;
    while ((nread = getline(&buff, &str_len, file)) != -1) {
      ++line_count;
      f_find = 0;
      for (int i = 0; i < data.pattern_size; ++i) {
        if (regexec(&regex[i], buff, 0, NULL, 0) == 0) {
          f_find = 1;
          break;
        }
      }
      if ((!f_find && flag.v) || (f_find && !flag.v)) ++match_count;
      if (flag.v) f_find = !f_find;
      if (f_find && !flag.l && !flag.c) {
        if (!flag.o) {
          print_line(file_name, line_count, buff, flag, data, file);
        } else {
          flag_o(data, regex, buff, file);
        }
      }
    }
    if (flag.c) {
      if (data.file_size > 1 && !flag.h) {
        printf("%s:", file_name);
        printf("%d\n", match_count);
      }
      if (data.file_size == 1) printf("%d\n", match_count);
    }
    if (flag.l && match_count > 0) printf("%s\n", file_name);
    free(buff);
    fclose(file);
  } else {
    if (!flag.s)
      fprintf(stderr, "grep: %s: No such file or directory\n", file_name);
  }
}

void grep(data_t data, grep_flags_t flag) {
  regex_t* compiled_regex =
      (regex_t*)malloc(data.pattern_size * sizeof(regex_t));
  for (int i = 0; i < data.pattern_size; ++i) {
    int reg_flag = flag.i ? REG_ICASE : 0;
    regcomp(&compiled_regex[i], data.pattern[i], reg_flag);
  }
  for (int i = 0; i < data.file_size; ++i) {
    grep_file(data.file[i], data, flag, compiled_regex);
  }
  for (int i = 0; i < data.pattern_size; ++i) {
    regfree(&compiled_regex[i]);
  }
  free(compiled_regex);
}

void free_data(data_t* data) {
  for (int i = 0; i < data->file_size; ++i) {
    free(data->file[i]);
  }
  free(data->file);
  for (int i = 0; i < data->pattern_size; ++i) {
    free(data->pattern[i]);
  }
  free(data->pattern);
}
