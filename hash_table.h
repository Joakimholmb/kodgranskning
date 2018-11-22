#pragma once
#include "common.h"
#include "list_linked.h"

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef struct option option_t;

/// @brief Create a new hash table
/// @param func A hash function for your selected key type
/// @return A new empty hash table
void ioopm_dynamic_hash_table_create(ioopm_hash_table_t *ht);

/// @brief resize a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_resize(ioopm_hash_table_t *ht);

/// @brief Create a new hash table
/// @param func A hash function for your selected key type
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(hash_func func);

/// @brief Create a new hash table
/// @param ht a hash table to navigate in
/// @param entry entry navigate through bucket with 
/// @param entry_key an entry to find previous entry for
/// @return A new empty hash table
entry_t *ioopm_find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t entry_key);

/// @brief add key => value entry in hash table ht
/// @param h hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param h hash table operated upon
/// @param key key to lookup
/// @return a pointer to the elem_t mapped to by key.	
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param h hash table operated upon
/// @param key key to remove, only keys existing in the hash table are valid
/// @return the value mapped to by key 
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

///@brief remove a hash table
///@param ht hash table to be destroyed
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return an array of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought 
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought 
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate 
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_apply_function_bool pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate 
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_apply_function_bool pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function_void apply_fun, void *arg);

/// @brief apply hash function on string
/// @param str string operated upon
/// @return hash value
int ioopm_string_knr_hash(elem_t str);

/// @brief return int from type elem_t
/// @param key elem_t operated upon
/// @return int value from elem_t
int ioopm_key_extract_int(elem_t key);
