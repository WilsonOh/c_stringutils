#ifndef ITERATOR_H
#define ITERATOR_H

typedef struct string_iterator_t string_iterator_t;

#include <stdio.h>

string_iterator_t *new_string_iterator(char **vals, size_t size);

string_iterator_t *new_string_iterator_v(int num, ...);

char *iterate(string_iterator_t *si);

#endif // !ITERATOR_H
#define ITERATOR_H
