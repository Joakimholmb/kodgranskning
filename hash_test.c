#include "CUnit/Basic.h"
#include "list_linked.h"
#include "hash_table.h"

const elem_t ONE = {1};
const elem_t TWO = {2};
const elem_t THREE = {3};
const elem_t FIFTY = {50};
const elem_t SHEJ = {.c = "hej"};
const elem_t S10 = {.c = "10"};
const elem_t S333 = {.c = "333"};

struct hash_table
{
  size_t no_buckets;
  entry_t *buckets;  
  size_t size;
  hash_func func; 
};

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

//************ TODO: EGNA TESTFUNKTIONER


void test_lookup1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_key_extract_int);
  for (int i = 0; i < 17; ++i) /// 18 is a bit magical 
    {
      elem_t value = {i};
      CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, value).value.c, NULL);
    }
  ioopm_hash_table_destroy(ht);
}



void test_insert1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_key_extract_int);
  
  ioopm_hash_table_insert(ht, ONE, SHEJ);
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_lookup(ht, ONE).value.c, "hej");
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, TWO).defined, false)
  ioopm_hash_table_insert(ht, TWO, S10);
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_lookup(ht, TWO).value.c, "10");
  ioopm_hash_table_insert(ht, TWO, SHEJ);
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_lookup(ht, TWO).value.c, "hej");

  ioopm_hash_table_destroy(ht);
}

void test_remove1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  
  elem_t str = ioopm_hash_table_remove(ht, ONE);
  
  CU_ASSERT_EQUAL(str.c, "10");
  
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, ONE).defined, false);
  
  ioopm_hash_table_destroy(ht);
}

void test_remove2()
{ 
 ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, ONE).defined, false);

  elem_t str = ioopm_hash_table_remove(ht, ONE);
  
  CU_ASSERT_EQUAL(str.c, NULL);
  
  ioopm_hash_table_destroy(ht);
}

void test_size1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_size2()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  
  ioopm_hash_table_destroy(ht);
}

void test_size3()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  ioopm_hash_table_remove(ht, ONE);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_clear1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  ioopm_hash_table_clear(ht);
  
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_clear2()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_clear(ht);
  
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_clear3()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  ioopm_hash_table_remove(ht, ONE);

  ioopm_hash_table_clear(ht);
  
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_getkeys1()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL); 
  
  ioopm_hash_table_insert(ht, THREE, S10);
  
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);

  
  CU_ASSERT_EQUAL(ioopm_linked_list_get(keys, 0).i, 3);
  
  ioopm_linked_list_destroy(keys);
  ioopm_hash_table_destroy(ht);
  
}

void test_getkeys2()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_list_t *keys = ioopm_hash_table_keys(ht);

   
  CU_ASSERT_EQUAL(ioopm_linked_list_get(keys, 0).i, 0);

  
  ioopm_linked_list_destroy(keys);
  ioopm_hash_table_destroy(ht);
}

void test_getkeys3()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_list_t *keys = ioopm_hash_table_keys(ht);

  ioopm_hash_table_insert(ht, SHEJ, S10);
  ioopm_hash_table_remove(ht, SHEJ);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(keys, 0).i, 0);
  
  ioopm_linked_list_destroy(keys);
  ioopm_hash_table_destroy(ht);
}

void test_getvalues1()
{
  
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  ioopm_hash_table_insert(ht, TWO, S333);

  ioopm_list_t *values = ioopm_hash_table_values(ht);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(values, 0).c, "10");
  CU_ASSERT_EQUAL(ioopm_linked_list_get(values, 1).c, "333");
  
  ioopm_linked_list_destroy(values);
  ioopm_hash_table_destroy(ht);
  
}

void test_getvalues2()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_list_t *values = ioopm_hash_table_values(ht);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(values, 0).c, NULL);
  
  ioopm_linked_list_destroy(values);
  ioopm_hash_table_destroy(ht);
}

void test_getvalues3()
{
  
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_list_t *values = ioopm_hash_table_values(ht);

  ioopm_hash_table_insert(ht, THREE, S10);
  ioopm_hash_table_remove(ht, THREE);
  
  CU_ASSERT_EQUAL(ioopm_linked_list_get(values, 0).c, NULL);
  
  ioopm_linked_list_destroy(values);
  ioopm_hash_table_destroy(ht);
  
}

void test_haskey1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, SHEJ);
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, ONE));
  
  ioopm_hash_table_destroy(ht);
}

void test_haskey2()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);
  
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, THREE));
  
  ioopm_hash_table_destroy(ht);
}

void test_hasvalue1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, ONE, S10);
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, S10));
  
  //ioopm_hash_table_insert(ht, (elem_t)5, (elem_t)"59");
  
  //CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, (elem_t)"59"));
  
  ioopm_hash_table_destroy(ht);
}

void test_hasvalue2()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);
  
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, THREE));
  
  ioopm_hash_table_destroy(ht);
}

void test_hasvalue3()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);

  ioopm_hash_table_insert(ht, THREE, S333);
  ioopm_hash_table_remove(ht, THREE);
  
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, S333));
  
  ioopm_hash_table_destroy(ht);
}

void aux_apply_to_all(elem_t key_ignored, elem_t *value, void *x)
{

  *value = THREE;

  return;
}


void test_apply_to_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);
  
  ioopm_hash_table_insert(ht, ONE, TWO);

  ioopm_hash_table_apply_to_all(ht, aux_apply_to_all, NULL);
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, ONE).value.i, 3);
  
  
  ioopm_hash_table_destroy(ht);
}

bool aux_all(elem_t key_ignored, elem_t value, void *x)
{
  elem_t compare = S10;
  return (value.i == compare.i);
  
}

void test_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL);
  
  ioopm_hash_table_insert(ht, ONE, S10);
  ioopm_hash_table_insert(ht, TWO, S10);
  ioopm_hash_table_insert(ht, THREE, S10);

  
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, aux_all, NULL));
  
  ioopm_hash_table_destroy(ht);
}

void test_resize()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_key_extract_int);

  for(int i=10; i<60; ++i)
    {
      elem_t j = {i};
      ioopm_hash_table_insert(ht, j, j);
    }

  
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, FIFTY).value.i, 50);
  
    ioopm_hash_table_destroy(ht);
}


//******************

//      MAIN

//******************
int main()
{
  CU_pSuite pSuiteLookup = NULL;
  CU_pSuite pSuiteInsert = NULL;
  CU_pSuite pSuiteRemove = NULL;
  CU_pSuite pSuiteSize = NULL;
  CU_pSuite pSuiteClear = NULL;
  CU_pSuite pSuiteGetKeys = NULL;
  CU_pSuite pSuiteGetValues = NULL;
  CU_pSuite pSuiteHasKey = NULL;
  CU_pSuite pSuiteHasValue = NULL;
  CU_pSuite pSuiteApplyToAll = NULL;
  CU_pSuite pSuiteAll = NULL;
  CU_pSuite pSuiteResize = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // LOOKUP SUITE
  pSuiteLookup = CU_add_suite("lookup Suite", init_suite, clean_suite);
  if(NULL == pSuiteLookup)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if(NULL == CU_add_test(pSuiteLookup, "test of lookup()", test_lookup1))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // INSERT SUITE
  pSuiteInsert = CU_add_suite("Insert Suite", init_suite, clean_suite);
  if(NULL == pSuiteInsert)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if(NULL == CU_add_test(pSuiteInsert, "test of insert()", test_insert1))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // REMOVE SUITE
  pSuiteRemove = CU_add_suite("Remove Suite", init_suite, clean_suite);
  if(NULL == pSuiteRemove)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteRemove, "test of remove()", test_remove1) || (NULL == CU_add_test(pSuiteRemove, "test of remove()", test_remove2))))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // SIZE SUITE

  pSuiteSize = CU_add_suite("Size Suite", init_suite, clean_suite);
  if(NULL == pSuiteSize)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteSize, "test of size()", test_size1)) || (NULL == CU_add_test(pSuiteSize, "test of size()", test_size2)) || (NULL == CU_add_test(pSuiteSize, "test of size()", test_size3)))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // CLEAR SUITE

  pSuiteClear = CU_add_suite("Clear Suite", init_suite, clean_suite);
  if(NULL == pSuiteClear)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteClear, "test of clear()", test_clear1)) || (NULL == CU_add_test(pSuiteClear, "test of clear()", test_clear2)) || (NULL == CU_add_test(pSuiteClear, "test of clear()", test_clear3)))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // GETKEYS SUITE

  pSuiteGetKeys = CU_add_suite("GetKeys Suite", init_suite, clean_suite);
  if(NULL == pSuiteGetKeys)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteGetKeys, "test of getkeys1()", test_getkeys1)) || (NULL == CU_add_test(pSuiteGetKeys, "test of getkeys2()", test_getkeys2)) || (NULL == CU_add_test(pSuiteGetKeys, "test of getkeys3()", test_getkeys3)))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // GETVALUES SUITE

  pSuiteGetValues = CU_add_suite("Getvalues Suite", init_suite, clean_suite);
  if(NULL == pSuiteGetValues)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteGetValues, "test of getvalues1()", test_getvalues1)) || (NULL == CU_add_test(pSuiteGetValues, "test of getvalues2()", test_getvalues2)) || (NULL == CU_add_test(pSuiteGetValues, "test of getvalues3()", test_getvalues3)))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // HASKEY SUITE
  
  pSuiteHasKey = CU_add_suite("HasKey Suite", init_suite, clean_suite);
  if(NULL == pSuiteHasKey)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteHasKey, "test of haskey1()", test_haskey1) || (NULL == CU_add_test(pSuiteHasKey, "test of haskey2()", test_haskey2))))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }
  
  // HASVALUE SUITE

  pSuiteHasValue = CU_add_suite("Hasvalue Suite", init_suite, clean_suite);
  if(NULL == pSuiteHasValue)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if((NULL == CU_add_test(pSuiteHasValue, "test of hasvalue1()", test_hasvalue1) || (NULL == CU_add_test(pSuiteHasValue, "test of hasvalue2()", test_hasvalue2)) || (NULL == CU_add_test(pSuiteHasValue, "test of hasvalue3()", test_hasvalue3))))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // APPLTOALL SUITE

  pSuiteApplyToAll = CU_add_suite("ApplyToAll Suite", init_suite, clean_suite);
  if(NULL == pSuiteApplyToAll)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if(NULL == CU_add_test(pSuiteApplyToAll, "test of ApplyToAll()", test_apply_to_all))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // APPLTOALL SUITE

  pSuiteAll = CU_add_suite("All Suite", init_suite, clean_suite);
  if(NULL == pSuiteAll)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if(NULL == CU_add_test(pSuiteAll, "test of All()", test_all))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  // RESIZE SUITE

  pSuiteResize = CU_add_suite("test of Resize", init_suite, clean_suite);
  if(NULL == pSuiteResize)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if(NULL == CU_add_test(pSuiteResize, "test of resize()", test_resize))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }
  
  // RUN TESTS

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
