#ifndef ITERATOR_H
#define ITERATOR_H

typedef struct iterator_t iterator_t;

#include <stdio.h>

struct iterator_t {
  void *vals;
  size_t curr;
  size_t size;
};

#define ITERATE(TYPE, IT)                                                      \
  (IT->curr >= IT->size ? NULL : (TYPE *)&(((TYPE *)IT->vals)[IT->curr++]))

iterator_t *new_iterator(void *vals, size_t size);

// string_iterator_t *new_string_iterator_v(int num, ...);

#endif // !ITERATOR_H
#define ITERATOR_H
