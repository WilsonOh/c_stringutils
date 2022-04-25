#include "iterator.h"
#include "stringutils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// TODO Make the struct generic using MACROS :)
struct string_iterator_t {
  void *vals;
  size_t curr;
  size_t size;
};

struct string_iterator_t *new_string_iterator(char **vals, size_t size) {
  string_iterator_t *s = malloc(sizeof(string_iterator_t));
  if (s == NULL) {
    perror("new_string_iterator: malloc\n");
    return NULL;
  }
  s->vals = vals;
  s->curr = 0;
  s->size = size;
  return s;
}

struct string_iterator_t *new_string_iterator_v(int num, ...) {
  va_list valist;
  char **s = malloc(sizeof(char **) * (size_t)num);
  va_start(valist, num);
  for (int i = 0; i < num; i++) {
    s[i] = va_arg(valist, char *);
  }
  string_iterator_t *si = new_string_iterator(s, num);
  return si;
}

char *iterate(struct string_iterator_t *si) {
  if (si->curr == si->size) {
    return NULL;
  }
  char **vals = (char **)si->vals;
  return vals[si->curr++];
  // return  si->vals[si->curr++];
}
