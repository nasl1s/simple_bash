#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define INVALID_INPUT_ERROR -1
#define ILLEGAL_OPTION -2

typedef struct cat_flags_s {
  int b;
  int E;
  int n;
  int s;
  int T;
  int t;
  int v;
} cat_flags_t;

int get_flags_files(const int argc, char* argv[], char*** files,
                    int* files_count, cat_flags_t* flag);
void cat(cat_flags_t* flag, char** files, int files_count);
void free_files(char*** files, int files_count);

int main(int argc, char* argv[]) {
  char** files = NULL;
  int files_count = 0;
  cat_flags_t flags = {0, 0, 0, 0, 0, 0, 0};
  int response = get_flags_files(argc, argv, &files, &files_count, &flags);
  if (response == INVALID_INPUT_ERROR) {
    fputs("cat: invalid input\n", stderr);
  } else {
    cat(&flags, files, files_count);
    free_files(&files, files_count);
  }
}

int process_line_character(const cat_flags_t* flag, const int sym,
                           int* new_line_flag, int* str_num) {
  int result = 0;
  if (flag->s) {
    if (sym == '\n') {
      if (*new_line_flag) {
        result = 1;
      } else {
        *new_line_flag = 1;
      }
    } else {
      *new_line_flag = 0;
    }
  }
  if (!result) {
    if (flag->b) {
      if (!(*new_line_flag) && sym != '\n') {
        printf("%6d\t", ++(*str_num));
      }
    } else if (flag->n) {
      printf("%6d\t", ++(*str_num));
    }
  }
  return result;
}

void cat_file(cat_flags_t* flag, FILE* f) {
  int str_num = 0;
  int sym, prev_sym;
  int new_line_flag = 0;
  for (prev_sym = '\n'; (sym = getc(f)) != EOF; prev_sym = sym) {
    if (prev_sym == '\n') {
      if (process_line_character(flag, sym, &new_line_flag, &str_num)) {
        continue;
      }
    }
    if (sym == '\n') {
      if (flag->E && putchar('$') == EOF) break;
    } else if (sym == '\t') {
      if (flag->T) {
        putchar('^');
        putchar('I');
        continue;
      }
    } else if (flag->v) {
      if (!isascii(sym) && !isprint(sym)) {
        putchar('M');
        putchar('-');
        sym = toascii(sym);
      }
      if (iscntrl(sym)) {
        putchar('^');
        putchar(sym == 127 ? '?' : sym + 64);
        continue;
      }
      if (sym == '\t') {
        putchar('^');
        putchar('I');
        continue;
      }
    }
    putchar(sym);
  }
}

void cat(cat_flags_t* flag, char** files, int files_count) {
  for (int i = 0; i < files_count; ++i) {
    FILE* f = fopen(files[i], "r");
    if (f) {
      cat_file(flag, f);
      fclose(f);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", files[i]);
    }
  }
}

int get_flags(const char* str, cat_flags_t* flag) {
  int err = SUCCESS;
  if (strcmp(str, "--number-nonblank") == 0) {
    flag->b = 1;
  } else if (strcmp(str, "--number") == 0) {
    flag->n = 1;
  } else if (strcmp(str, "--squeeze-blank") == 0) {
    flag->s = 1;
  } else {
    if (strlen(str) == 1) {
      err = ILLEGAL_OPTION;
    } else {
      for (size_t i = 1; i < strlen(str); ++i) {
        switch (str[i]) {
          case 'b':
            flag->b = 1;
            break;
          case 'e':
            flag->E = 1;
            flag->v = 1;
            break;
          case 'E':
            flag->E = 1;
            break;
          case 'T':
            flag->T = 1;
            break;
          case 'n':
            flag->n = 1;
            break;
          case 'v':
            flag->v = 1;
            break;
          case 's':
            flag->s = 1;
            break;
          case 't':
            flag->T = 1;
            flag->v = 1;
            break;
          default:
            printf("cat: illegal option -- %c\n", str[i]);
            err = ILLEGAL_OPTION;
            break;
        }
        if (err) {
          break;
        }
      }
    }
  }
  return err;
}

int get_flags_files(const int argc, char* argv[], char*** files,
                    int* files_count, cat_flags_t* flag) {
  int err = SUCCESS;
  int exit_loop = 0;
  *files_count = 0;
  *files = (char**)malloc(sizeof(char*));
  for (int i = 1; i < argc && !exit_loop; ++i) {
    if (argv[i][0] == '-') {
      if (get_flags(argv[i], flag)) {
        exit_loop = 1;
      }
    } else {
      char* file_name = strdup(argv[i]);
      char** result_ptr =
          (char**)realloc(*files, (*files_count + 1) * sizeof(char*));
      if (result_ptr) {
        *files = result_ptr;
        (*files)[(*files_count)++] = file_name;
      } else {
        free(file_name);
        free_files(files, *files_count);
        err = INVALID_INPUT_ERROR;
        exit_loop = 1;
      }
    }
  }
  return err;
}

void free_files(char*** files, int files_count) {
  for (int i = 0; i < files_count; ++i) {
    free((*files)[i]);
  }
  free(*files);
}