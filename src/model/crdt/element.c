#include <stdio.h>
#include "element.h"

unsigned int vkey_from_tokens(unsigned int depth, va_list valist) {
  unsigned int key = 0;
  int base = 1;
  for (int i = 0; i < depth; i++) {
    base <<= i;
    key += va_arg(valist, int) * base;
  }
  return key;
}

/**
 * @brief Given a key comprising k tokens, each token doubling in base size
 * in comparison to the previous, we can represent the key as a single
 * unsigned integer value.
 *
 * The first bit holds the value of the first token with size 2.
 * The next two bits hold the values of the next token with size 4.
 * The next three bits hold the values of the next token with size 8.
 * ... and so on.
 *
 * @param depth The number of tokens in the key.
 * @param ... The integer tokens of the key.
 *
 * @return An unsigned integer representation of the key.
 */
unsigned int key_from_tokens(unsigned int depth, ...) {
  va_list valist;
  va_start(valist, depth);
  int key = vkey_from_tokens(depth, valist);
  va_end(valist);
  return key;
}

unsigned int vuser_ids_from_ids(unsigned int depth, va_list valist) {
  unsigned int user_ids = 0;
  for (int i = 0; i < depth; i++) {
    int v = va_arg(valist, int);
    user_ids += v << (i * 8); // shift by eight bits per user id.
  }
  return user_ids;
}

unsigned int user_ids_from_ids(unsigned int depth, ...) {
  va_list valist;
  va_start(valist, depth);
  int user_ids = vuser_ids_from_ids(depth, valist);
  va_end(valist);
  return user_ids;
}

int key_compare(element* l, element* r) {
  unsigned int min_depth = l->depth < r->depth ? l->depth : r->depth;
  unsigned int l_key = l->key;
  unsigned int r_key = r->key;
  int base = 2;
  for (int i = 1; i <= min_depth; i++) {
    int l_token = l_key % base;
    int r_token = r_key % base;
    int compare = r_token - l_token;
    if (compare != 0) {
      return compare;
    }
    l_key /= base;
    r_key /= base;
    base <<= i;
  }
  return r->depth - l->depth;
}

bool key_equal(element* l, element* r) {
  return key_compare(l, r) == 0;
}

void vset_key_tokens(element* e, unsigned int depth, va_list valist) {
  e->key = vkey_from_tokens(depth, valist);
  e->depth = depth;
}

void set_key_tokens(element* e, unsigned int depth, ...) {
  va_list valist;
  va_start(valist, depth);
  vset_key_tokens(e, depth, valist);
  va_end(valist);
}

