CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: s21_grep

s21_cat: 
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

test: 
	sh tests.sh

clean:
	rm -f s21_grep