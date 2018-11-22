#include "list_linked.h"
#include "hash_table.h"
#include "iterator.h"

// GLOBAL DEFINITIONS
#define Init_Buckets 17
#define Load_Size 0.75
const size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};

struct entry
{
  elem_t key;
  elem_t value;
  entry_t *next;
};

struct hash_table
{
  size_t no_buckets;
  entry_t *buckets;  
  size_t size;
  hash_func func; 
};

// FUNCTION DECLARATIONS

void ioopm_dynamic_hash_table_create(ioopm_hash_table_t *ht);
void ioopm_hash_table_resize(ioopm_hash_table_t *ht);
ioopm_hash_table_t *ioopm_hash_table_create(hash_func func);
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);
entry_t *ioopm_find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t entrykey);
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next);
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);
static elem_t destroy_next_entry(entry_t *entry);
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);
static void clear_bucket(entry_t *entry, ioopm_hash_table_t *ht);
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_apply_function_bool pred, void *arg);
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_apply_function_bool pred, void *arg);
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function_void apply_fun, void *arg);
static bool key_equiv(elem_t key, elem_t value_ignored, void *x);
static bool value_equiv(elem_t key_ignored, elem_t value, void *x);
int ioopm_string_knr_hash(elem_t str);
int ioopm_key_extract_int(elem_t key);

// **************************

//        FUNCTIONS

// **************************

void ioopm_dynamic_hash_table_create(ioopm_hash_table_t *ht)
{
  int i = 0;
  for(; primes[i] <=  ht->no_buckets ; ++i);
  ht->no_buckets = primes[i];
  ht->buckets = realloc(ht->buckets, (ht->no_buckets*sizeof(entry_t)));
}

void ioopm_hash_table_resize(ioopm_hash_table_t *ht)
{
  if(ht->size > Load_Size*ht->no_buckets)
    {
      ioopm_list_t *keys = ioopm_hash_table_keys(ht);
      ioopm_list_t *values = ioopm_hash_table_values(ht);
      ioopm_hash_table_clear(ht); 
      ioopm_dynamic_hash_table_create(ht);

      ioopm_list_iterator_t *iter_keys = ioopm_list_iterator(keys);
      ioopm_list_iterator_t *iter_values = ioopm_list_iterator(values);
      
      while(ioopm_iterator_has_next(iter_keys) && ioopm_iterator_has_next(iter_values))
        {
          elem_t key = ioopm_iterator_current(iter_keys);
          elem_t value = ioopm_iterator_current(iter_values);
          ioopm_hash_table_insert(ht, key, value);
           
          ioopm_iterator_next(iter_keys);
          ioopm_iterator_next(iter_values);
        }
      ioopm_iterator_destroy(iter_keys);
      ioopm_iterator_destroy(iter_values);
      ioopm_linked_list_destroy(keys);
      ioopm_linked_list_destroy(values);
      
    }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(0);
  int j = 0;
  for(size_t i = 0; i<ht->no_buckets; ++i)
    {
      entry_t *dummy_entry = &ht->buckets[i];
      entry_t *entry = dummy_entry->next;
      while (entry != NULL)
        {
          ioopm_linked_list_insert(list, j, entry->key);
          ++j;
          entry = entry->next;
        }
    }
  
  return list;
}

ioopm_hash_table_t *ioopm_hash_table_create(hash_func func)
{
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->no_buckets = Init_Buckets;
  result->buckets = calloc(result->no_buckets, sizeof(entry_t));
  result->func = func;
  return result;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{

  ioopm_hash_table_resize(ht);
  int int_key = 0;
  if (ht->func == NULL)
  {
    int_key = key.i;
  }
else
  {
    int_key = ht->func(key); 
  }
  
  int bucket = abs(int_key % ht->no_buckets);

  entry_t *entry = ioopm_find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = entry->next;

  if (next != NULL && next->key.i == key.i)
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
      ht->size += 1;
    }
  
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));

  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;

  return new_entry;
  
}


option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  int int_key = 0;
  
  if (ht->func == NULL)
  {
    int_key = key.i;
  }
else
  {
    int_key = ht->func(key); 
  }
  
  int bucket = abs(int_key % ht->no_buckets);
  key.i = int_key;
  entry_t *prev = ioopm_find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = prev->next;
  if (next && next->value.i && next->key.i == key.i)
    {
      return (option_t) { .defined = true, .value = next->value };
    }
  else
    {
      return (option_t) { .defined = false };
    }
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  entry_t *previous = ioopm_find_previous_entry_for_key(ht, &ht->buckets[key.i % ht->no_buckets], key);
  
  if(previous->next != NULL)
    {
      elem_t value = destroy_next_entry(previous);
      ht->size -= 1;
      return value;
    }

  elem_t error = {0};
  return error;
}



static elem_t destroy_next_entry(entry_t *entry)
{
  entry_t *next_entry = entry->next;
  elem_t value = next_entry->value;
  entry->next = next_entry->next;
  free(next_entry);
  return value;

  
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for(size_t i = 0; i<ht->no_buckets; ++i)
    {
      entry_t *bucket = &ht->buckets[i];
      entry_t *entry = bucket->next;
      bucket->next = NULL;
      clear_bucket(entry, ht);
    }
  free(ht->buckets);
  free(ht);
  return;
}

static void clear_bucket(entry_t *entry, ioopm_hash_table_t *ht)
{
  if(entry == NULL)
    {
      return;
    }
  else
    {
      while(entry != NULL)
        {
          entry_t *var = entry->next;
          free(entry);
          entry = var;
          ht->size -= 1;
        }
    }
  return;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{  
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for(size_t i = 0; i<ht->no_buckets; ++i)
    {
      entry_t *bucket = &ht->buckets[i];
      entry_t *entry = bucket->next;
      bucket->next = NULL;
      clear_bucket(entry, ht);
    }
  return;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(0);
  int j = 0;
  for(size_t i = 0; i<ht->no_buckets; ++i)
    {
      entry_t *dummy_entry = &ht->buckets[i];
      entry_t *entry = dummy_entry->next;
      while (entry != NULL)
        {
          ioopm_linked_list_insert(list, j, entry->value);
          ++j;
          entry = entry->next;
        }
    }
  return list;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, value_equiv, &value);
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_apply_function_bool pred, void *arg)
{
  for(size_t i; i<ht->no_buckets; ++i)
    {
      entry_t *bucket  = &ht->buckets[i];
      entry_t *entry = bucket->next;
      
      while(entry != NULL)
        {
          if(!(pred(entry->key, (entry->value), arg)))
            {
              return false;
            }
          entry = entry->next;
        }     
    }
  return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_apply_function_bool pred, void *arg)
{
  for(size_t i = 0; i<ht->no_buckets; ++i)
    {
      entry_t *entry  = &ht->buckets[i];
      entry = entry->next;
      while(entry != NULL)
        {
          if(pred(entry->key, (entry->value), arg) == true)
            {
              return true;
            }
          else
            {
              entry = entry->next;
            }
        }     
    }
  return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function_void apply_fun, void *arg)
{
  for(size_t i = 0; i<ht->no_buckets; ++i)
    {
      entry_t *bucket  = &ht->buckets[i];
      entry_t *entry = bucket->next;
      while(entry != NULL)
        {
          apply_fun(entry->key, &(entry->value), arg);
          entry = entry->next;
        }     
    }
  return;
}

static bool key_equiv(elem_t key, elem_t value_ignored, void *x)
{
  
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  if (key.i == other_key.i)
    {
      return (key.i == other_key.i);
    }
  else
    {
      int key_compare1 = ioopm_string_knr_hash(key);
      int key_compare2 = ioopm_string_knr_hash(other_key);
      return (key_compare1 == key_compare2);
    }
  return false;
}

static bool value_equiv(elem_t key_ignored, elem_t value, void *x)
{
  elem_t *other_value_ptr = x;
  elem_t *other_value = other_value_ptr;
  return value.c == (*other_value).c;
}


entry_t *ioopm_find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t entry_key)
{

  entry_t *cursor = entry;
  entry_t *entry_next = cursor->next;
 
  if(entry_next == NULL || entry_next->key.i == entry_key.i || entry_key.i < entry_next->key.i)
    {
      return cursor;
    }
  else if (ht->func(entry->next->key) == ht->func(entry_key))
    {
      return cursor;
    }

  return ioopm_find_previous_entry_for_key(ht, entry_next, entry_key);
}

// ***** HASH_TABLE FUNCS *****

int ioopm_string_knr_hash(elem_t str)
{
  int result = 0;
  int ascii = 32;
  char *s = str.c;
  
  do
    {
      result = result * ascii + *s;
    }
  while (*++s != '\0');
  return result;
}

int ioopm_key_extract_int(elem_t key)
{
  return key.i;
}
