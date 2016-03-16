/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"




S64 rsx_object_channel_get_dma_controll_lpar_address(rsx_channel_obj_t* ch_obj);
S32 rsx_object_channel_get_id(rsx_channel_obj_t* ch_obj);
void rsx_object_channel_create_sw_object_hash_table_entry(rsx_channel_obj_t* ch_obj, rsx_sw_class_obj_t* sw_obj);
void rsx_object_channel_create_nv_object_hash_table_entry(rsx_channel_obj_t* ch_obj, rsx_nv_class_obj_t* nv_obj);
void rsx_object_channel_create_dma_object_hash_table_entry(rsx_channel_obj_t* ch_obj, rsx_ctx_dma_obj_t* dma_obj);
void rsx_object_channel_220D0C(rsx_channel_obj_t* ch_obj, S32 channel_id, rsx_ctx_dma_obj_t* dma_obj);
rsx_channel_obj_t* rsx_object_channel_create_object(rsx_ctx_dma_obj_t* dma_obj);




#endif // __RSX_OBJECT_CHANNEL_H_
//rsx_object_channel_destroy_sw_object_hash_table_entry
