#!/bin/bash

# -b Конкатенация файлов с нумерацией строк
./s21_cat -b test.txt > s21_cat.txt
cat -b test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "n" - выводить номера строк
./s21_cat -n test.txt > s21_cat.txt
cat -n test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# -e конец строки
./s21_cat -e test.txt > s21_cat.txt
cat -e test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "s" - сжимает несколько смежных пустых строк 
./s21_cat -s test.txt > s21_cat.txt
cat -s test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "t" - выводить символы табуляции в виде "^I"
./s21_cat -t test.txt > s21_cat.txt
cat -t test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "v" - выводить непечатные символы
./s21_cat -v test.txt > s21_cat.txt
cat -v test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "b" - выводить нумерацию только для непустых строк
./s21_cat -b test.txt > s21_cat.txt
cat -b test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# -e конец строки
./s21_cat -e test.txt test2.txt > s21_cat.txt
cat -e test.txt test2.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "n" - выводить номера строк
./s21_cat -n test.txt test2.txt > s21_cat.txt
cat -n test.txt test2.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "s" - сжимает несколько смежных пустых строк
./s21_cat -s test.txt test2.txt > s21_cat.txt
cat -s test.txt test2.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "t" - выводить символы табуляции в виде "^I"
./s21_cat -t test.txt test2.txt > s21_cat.txt
cat -t test.txt test2.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Флаг "v" - выводить непечатные символы
./s21_cat -v test.txt test2.txt > s21_cat.txt
cat -v test.txt test2.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Нет файла
./s21_cat -b nofile.txt > s21_cat.txt
cat -b nofile.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Тест: Конкатенация файлов с номерами строк и подавлением пустых строк (-b, -s)
./s21_cat -b -s test.txt > s21_cat.txt
cat -b -s test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Тест: Вывод номеров строк и символов табуляции (-n, -t)
./s21_cat -n -t test.txt > s21_cat.txt
cat -n -t test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Тест: Вывод номеров строк и непечатных символов (-n, -v)
./s21_cat -n -v test.txt > s21_cat.txt
cat -n -v test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Тест: Поиск текста, вывод номеров строк и символов табуляции (-e, -n, -t)
./s21_cat -e -n -t test.txt > s21_cat.txt
cat -e -n -t test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Тест: Поиск текста, подавление пустых строк и вывод непечатных символов (-e, -s, -v)
./s21_cat -e -s -v test.txt > s21_cat.txt
cat -e -s -v test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

# Тест: Поиск текста, вывод номеров строк и подавление вывода пустых строк (-e, -n, -s)
./s21_cat -e -n -s test.txt > s21_cat.txt
cat -e -n -s test.txt > cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

