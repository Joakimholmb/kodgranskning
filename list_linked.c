//#include <stdlib.h>
//#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "list_linked.h"
#include <errno.h>

extern int errno;

struct link
{
  elem_t element;
  link_t *next;
};

struct list
{
  link_t *first;
  size_t size;
  link_t *last;
  ioopm_eq_function func;
};

// ************** LIST_LINKED FUNC DECLARATIONS ************

static link_t *link_new(link_t *next, elem_t value);
static link_t *find_previous_link(ioopm_list_t *list, size_t index);
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function compare);
void ioopm_linked_list_destroy(ioopm_list_t *list);
void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value);
elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index);
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value);
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);
void ioopm_linked_list_clear(ioopm_list_t *list);
elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index);
int ioopm_linked_list_size(ioopm_list_t *list);
bool ioopm_linked_list_is_empty(ioopm_list_t *list);
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element);
bool ioopm_linked_list_all(ioopm_list_t *list, void *x);
bool ioopm_linked_list_any(ioopm_list_t *list, void *x);
void ioopm_linked_apply_to_all(ioopm_list_t *list, void (*fun)(elem_t *, elem_t *), void *x);

/*

  FUNCTIONS

*/

static link_t *link_new(link_t *next, elem_t value)
{
  link_t *link_new = calloc(1, sizeof(link_t));
  link_new->next = next;
  link_new->element = value;
  
  return link_new;
}

static link_t *find_previous_link(ioopm_list_t *list, size_t index)
{
  link_t *cursor = list->first;
  if (cursor == NULL)
    {
      return NULL;
    }
  else if (index <= 1)
    {
      return cursor;
    }
  else
    {
      for(size_t i = 0; i < (index-1); ++i)
        {
          cursor = cursor->next;
        }
    }
  return cursor;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function compare)
{
  ioopm_list_t *result = calloc(1, sizeof(ioopm_list_t));
  result->func = compare;
  return result;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  ioopm_linked_list_clear(list);
  free(list);
  return;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  link_t *link = list->first;

  if(link == NULL)
    {
      return;
    }
  while(link->next != NULL)
    {
      link_t *var = link->next;
      free(link);
      link = var;
      list->size -= 1;
    }
  free(link);
  return;
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
  if (list->first == NULL)
    {
      
      list->first = link_new(NULL, value);
      list->last = list->first;
    }
  else
    {
      if (index == list->size)
        {
          ioopm_linked_list_append(list, value);
          return;
        }
      else
        {
          link_t *prev_link = find_previous_link(list, index);
          link_t *next_link = prev_link->next;
          link_t *new_link = link_new(next_link,value);
          prev_link->next = new_link;
        }
    }
  list->size += 1;
  return;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
  elem_t zero = {0};
  if(list->size < index)
    {
      errno = EINVAL;
      return zero;
    }
  else
    {
      link_t *link = find_previous_link(list, (index+1));
      if(link == NULL)
        {
          errno = EINVAL;
          return zero;
        }
      return link->element;
    }
  
  
  return zero;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  if (list->last == NULL)
    {
      ioopm_linked_list_insert(list, 0, value);
    }
  else
    {
      link_t *new_link = link_new(NULL, value);
      list->last->next = new_link;
      list->last = new_link;
    }
  list->size +=1;
  return;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
  list->first = link_new(list->first, value);
  list->size += 1;
  return;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
  elem_t zero = {0};
  link_t *prev_link = find_previous_link(list, (index));
  if(prev_link == NULL)
    {
      errno = EINVAL;
      return zero;
    }

  if(prev_link == list->first && index == 0)
    {
      link_t *next_link = prev_link->next->next;
      link_t *link = prev_link->next;
      prev_link->next = next_link;
      elem_t value = link->element;
      free(prev_link);
      
      list->first = link;
      list->size -= 1;
      return value;
    }
  else
    {
      link_t *next_link = prev_link->next->next;
      link_t *link = prev_link->next;
      prev_link->next = next_link;
      elem_t value = link->element;
      free(link);
      
      list->size -= 1;
      return value;
    }
}

int ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  return (ioopm_linked_list_size(list) == 0);
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
  link_t *cursor = list->first;
  while(cursor != NULL)
    {
      if ((list->func(cursor->element, element)))
        {
          return true;
        }
      cursor = cursor->next;
    }
  
  return false;  
}

bool ioopm_linked_list_all(ioopm_list_t *list, void *x)

{
  link_t *cursor = list->first;
  elem_t *elem_compare = x;
  
  if(cursor == NULL)
    {
      return false;
    }
  else
    {
      while(cursor != NULL)
        {
          if (list->func(cursor->element, *elem_compare))
            {
              cursor = cursor->next;
            }
          else
            {
              return false;
            }
        }
    }
  
  return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, void *x)

{
  link_t *cursor = list->first;
  elem_t *elem_compare = x;
  while(cursor != NULL)
    {
      if (list->func(cursor->element, *elem_compare))
        {
          return true;
        }
      else
        {
          cursor = cursor->next;
        }
    }
  return false;
}

void ioopm_linked_apply_to_all(ioopm_list_t *list, void (*fun)(elem_t *, elem_t *), void *x)
{
  link_t *cursor = list->first;
  elem_t *elem = x;
  while(cursor != NULL)
    {
      fun(&(cursor->element), elem);
      cursor = cursor->next;
    }
  return;
}
