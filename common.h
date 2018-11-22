#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef union 
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char *c;
} elem_t;

struct option
{
  bool defined;
  elem_t value;
};

typedef bool(*ioopm_apply_function_bool)(elem_t key, elem_t value, void *extra);
typedef void(*ioopm_apply_function_void)(elem_t key, elem_t *value, void *extra);
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);
typedef int(*hash_func)(elem_t str);

