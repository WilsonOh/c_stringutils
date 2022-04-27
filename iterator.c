#include "iterator.h"
#include "stringutils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct iterator_t *new_iterator(void *vals, size_t size) {
  struct iterator_t *it = malloc(sizeof(struct iterator_t));
  if (it == NULL) {
    perror("new_iterator: malloc\n");
    return NULL;
  }
  it->curr = 0;
  it->size = size;
  it->vals = vals;
  return it;
}

/* struct iterator_t *new_iterator_v(int num, ...) {
  va_list valist;
  void *s = malloc(sizeof(void *) * (size_t)num);
  va_start(valist, num);
  for (int i = 0; i < num; i++) {
    s[i] = (void *) va_arg(valist, char *);
  }
  struct iterator_t *si = new_iterator(s, num);
  va_end(valist);
  return si;
} */
