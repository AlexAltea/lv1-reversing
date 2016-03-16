/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

S32 rsx_utils_bitmap_get_item_total(rsx_utils_bm_obj_t* bm_obj);
void rsx_utils_bitmap_free_map(rsx_utils_bm_obj_t* bm_obj);
S32 rsx_utils_bitmap_dealloc(rsx_utils_bm_obj_t* bm_obj, S64 start, S32 count);
S32 rsx_utils_bitmap_allocate(rsx_utils_bm_obj_t* bm_obj, S32 count, S64 *out);
void rsx_utils_bitmap_set_bit(rsx_utils_bm_obj_t* bm_obj, S32 idx, S32 value);
void rsx_utils_bitmap_create(rsx_utils_bm_obj_t* bm_obj, S64 val_1, S32 item_total, S64 val_2);


#endif // __RSX_UTILS_BITMAP_H__
