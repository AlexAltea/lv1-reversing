/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#ifndef __RSX_OBJECT_HASH_TABLE_H__
#define __RSX_OBJECT_HASH_TABLE_H__


void rsx_object_hash_table_create_entry(rsx_hash_tbl_obj_t* hash_tbl, S32 channel_id, uS32 class_type, S32 offset, S32 arg1);
void rsx_object_hash_table_invalidate_entry(rsx_hash_tbl_obj_t* hash_tbl, S32 channel_id, uS32 class_type);
void rsx_object_hash_table_init(rsx_hash_tbl_obj_t* hash_tbl);
void rsx_object_hash_table_finalize(rsx_hash_tbl_obj_t* hash_tbl);


#endif // __RSX_OBJECT_HASH_TABLE_H__
