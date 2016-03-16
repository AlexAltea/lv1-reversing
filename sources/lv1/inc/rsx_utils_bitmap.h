#ifndef __RSX_UTILS_BITMAP_H__
#define __RSX_UTILS_BITMAP_H__


int32_t rsx_utils_bitmap_get_item_total(rsx_utils_bm_obj_t *bm_obj);
void rsx_utils_bitmap_free_map(rsx_utils_bm_obj_t *bm_obj);
int32_t rsx_utils_bitmap_dealloc(rsx_utils_bm_obj_t *bm_obj, int64_t start, int32_t count);
int32_t rsx_utils_bitmap_allocate(rsx_utils_bm_obj_t *bm_obj, int32_t count, int64_t *out);
void rsx_utils_bitmap_set_bit(rsx_utils_bm_obj_t *bm_obj, int32_t idx, int32_t value);
void rsx_utils_bitmap_create(rsx_utils_bm_obj_t *bm_obj, int64_t val_1, int32_t item_total, int64_t val_2);


#endif // __RSX_UTILS_BITMAP_H__
