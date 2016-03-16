#ifndef __RSX_OBJECT_HASH_TABLE_H__
#define __RSX_OBJECT_HASH_TABLE_H__


void rsx_object_hash_table_create_entry(rsx_hash_tbl_obj_t *hash_tbl, int32_t channel_id, uint32_t class_type, int32_t offset, int32_t arg1);
void rsx_object_hash_table_invalidate_entry(rsx_hash_tbl_obj_t *hash_tbl, int32_t channel_id, uint32_t class_type);
void rsx_object_hash_table_init(rsx_hash_tbl_obj_t *hash_tbl);
void rsx_object_hash_table_finalize(rsx_hash_tbl_obj_t *hash_tbl);


#endif // __RSX_OBJECT_HASH_TABLE_H__
