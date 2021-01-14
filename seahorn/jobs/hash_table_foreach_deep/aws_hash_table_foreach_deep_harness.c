#include <aws/common/hash_table.h>
#include <aws/common/private/hash_table_impl.h>
#include <config.h>
#include <hash_table_helper.h>
#include <seahorn/seahorn.h>
#include <utils.h>

/* pre-condition that entry_count matches actual number of empty slot
    is too expensive to check, so this job will use a stub of
    hash_table_state_is_valid that ignores entry_count
 */

int hash_table_foreach_proof_callback(void *context,
                                      struct aws_hash_element *pElement) {
  AWS_PRECONDITION(AWS_OBJECT_PTR_IS_WRITABLE(pElement),
                   "Input pointer [pElement] must be writable.");
  return nd_int();
}

int main(void) {
  struct aws_hash_table map;
  initialize_bounded_aws_hash_table(&map, MAX_TABLE_SIZE);
  ensure_hash_table_has_valid_destroy_functions(&map);
  assume(aws_hash_table_entry_count_is_valid(&map));
  map.p_impl->equals_fn = nondet_equals;
  map.p_impl->hash_fn = uninterpreted_hasher;
  assume(aws_hash_table_is_valid(&map));

  void *context = nd_voidp();
  aws_hash_table_foreach(&map, hash_table_foreach_proof_callback, context);
  /* No obvious postconditions, other than that the map remains valid. But the
   * iterator could have modified the table
   */
  sassert(aws_hash_table_is_valid(&map));
  return 0;
}