#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "iterator.h"

struct iter 
{
  link_t *prev;
  link_t *current;
  ioopm_list_t *list;
};

struct list
{
  link_t *first;
  size_t size;
  link_t *last;
  ioopm_eq_function func;
};

struct link
{
  elem_t element;
  link_t *next;
};

/*

FUNCTION DECLARATIONS

*/

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter);
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter);
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);
static link_t *link_new(link_t *link, elem_t elem);

/**************************************

// ITERATOR

**************************************/
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));

  result->list = list;
  result->current = list->first;
  result->prev = list->first;
  return result;  
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  if(iter->current == NULL)
    {
      return false;
    }
  return iter->current->next != NULL; 
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  if(iter->prev == iter->current)
    {
      iter->current = iter->current->next;
      return iter->current->element;
    }
  
  iter->prev = iter->current;
  iter->current = iter->current->next;
  return iter->current->element; 
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
  if (iter->current == iter->list->first)
    {
      iter->list->first = iter->current->next;
    }
  link_t *to_remove = iter->current;
  iter->prev->next = to_remove->next;
  iter->current = to_remove->next;
  elem_t value = to_remove->element;

  free(to_remove);
  return value;
}

#define LIST_EMPTY iter->current==NULL
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t elem)
{
  if (LIST_EMPTY)
    {
      iter->current = link_new(NULL, elem);
      iter->list->first = iter->current;
      iter->prev = iter->current;

      return;
    }
  else if (iter->current == iter->list->first)
    {      
      iter->current = link_new(iter->current, elem);
      iter->list->first = iter->current;
      iter->prev = iter->current;

      return;
    }
  iter->current = link_new(iter->current, elem);
  iter->prev->next = iter->current;
  return;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  iter->current = iter->list->first;
  return;
}
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  elem_t zero = {0};
  
  if(iter->current == NULL)
    {
      errno = EINVAL;
      return zero;
    }
  
  return iter->current->element; 
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter);
}

static link_t *link_new(link_t *next, elem_t value)
{
  link_t *link_new = calloc(1, sizeof(link_t));
  link_new->next = next;
  link_new->element = value;
  
  return link_new;
}
