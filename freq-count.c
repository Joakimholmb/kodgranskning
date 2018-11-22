#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
//#include "list_linked.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  size_t no_buckets;
  entry_t *buckets;  
  size_t size;
  hash_func func; 
};

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
  ioopm_eq_function *func;
};

struct iter 
{
  link_t *prev;
  link_t *current;
  ioopm_list_t *list;
};

//****************************************

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  printf("%s\n", word);
  elem_t key;
  key.c = strdup(word);
  
  if(ioopm_hash_table_has_key(ht, key))
    {
      
     
      entry_t *previous = ioopm_find_previous_entry_for_key(ht, &ht->buckets[abs(ht->func(key) % ht->no_buckets)], key);
      previous->next->value.i += 1;

      return;
    }
  else
    {
      elem_t one = {1};
      ioopm_hash_table_insert(ht, key, one);

    }
}


void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true) 
    {
      char *buf = NULL;
      size_t len = 0;
      getline(&buf, &len, f);

      if (feof(f))
        {
          free(buf);
          break;
        }
      
      for (char *word = strtok(buf, Delimiters);
           word && *word;
           word = strtok(NULL, Delimiters)
           )
        {
          process_word(word, ht);
        }

      free(buf);
    }
  
  fclose(f);
}



int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_string_knr_hash);
  
  if (argc > 1)
    {
      for (int i = 1; i < argc; ++i)
        {
          process_file(argv[i], ht);
        }


      ioopm_list_t *list = ioopm_hash_table_keys(ht);

      link_t *link = list->first;
      char **buf = calloc(ht->size, sizeof(char *));
      for (size_t i = 0; i < list->size; ++i)
        {
          buf[i] = strdup(link->element.c);
          link = link->next;
        }
      sort_keys(buf, list->size);
      for (size_t i = 0; i < list->size; ++i)
        {
          elem_t key = {.c = buf[i]};
          entry_t *previous = ioopm_find_previous_entry_for_key(ht, &ht->buckets[abs(ht->func(key) % ht->no_buckets)], key);

          printf("%s, freq: %d\n", buf[i], previous->next->value.i);
        }
      puts("done");
    }
  else
    {
      puts("Usage: freq-count file1 ... filen");
    }
}
