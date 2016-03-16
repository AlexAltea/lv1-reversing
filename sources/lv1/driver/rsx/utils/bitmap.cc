/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



/***********************************************************************
* get total item count of a bitmap
***********************************************************************/
S32 rsx_utils_bitmap_get_item_total(rsx_utils_bm_obj_t* bm_obj) {
    return bm_obj->item_total;
}

/***********************************************************************
* free the bitmap of a bitmap object
***********************************************************************/
void rsx_utils_bitmap_free_map(rsx_utils_bm_obj_t* bm_obj) {
    lv1_kfree((void *)bm_obj->bitmap);
    bm_obj->bitmap = NULL;
    
    return;
}

/***********************************************************************
* free a item into a bitmap
* 
* rsx_utils_bm_obj_t* bm_obj = bitmap object
* S64 start              = start of continuously items to free
* S32 count              = count of continuously items to free
***********************************************************************/
S32 rsx_utils_bitmap_dealloc(rsx_utils_bm_obj_t* bm_obj, S64 start, S32 count) {
    S32 i, bm_idx;
    S64 *bm = NULL;
    uS64 seg;
    
    
    // nothing to free
    if (count == 0)
        return 0;  // FAIL
    
    bm_idx = (S32)((start - bm_obj->value1) / bm_obj->value2);
    
    bm = bm_obj->bitmap;
    
    // free item, if set
    for(i = bm_idx; i < count; i++) {
        seg = bm[i / bm_obj->seg_base];
        if (((seg >>(i - ((i / bm_obj->seg_base) * bm_obj->seg_base))) & (uS64)1) == 1)
          rsx_utils_bitmap_set_bit(bm_obj, i, 0);
    }
    
    return 1;  // OK
}

/***********************************************************************
* allocate a item into a bitmap
* 
* rsx_utils_bm_obj_t* bm_obj = bitmap object
* S32 count              = count of continuously items to allocate
* S64 *out               = start of the allocated continuously items
***********************************************************************/
S32 rsx_utils_bitmap_allocate(rsx_utils_bm_obj_t* bm_obj, S32 count, S64 *out) {
    S32 i, free = 0, start = 0;
    S64 *bm = NULL;
    uS64 seg;
    
    
    // fail, nothing to allocate in bitmap
    if (bm_obj->item_total == 0)
      return 0;  // FAIL
    
    // count to allocate is 0
    if (count == 0) {
        *out = bm_obj->value1;
        return 1;  // OK
    }
    
    bm = bm_obj->bitmap;
    
    // get start and count of free items into bitmap
    for(i = 0; i < bm_obj->item_total; i++) {
        seg = bm[i / bm_obj->seg_base];
        
        // item free
        if (((seg >>(i - ((i / bm_obj->seg_base) * bm_obj->seg_base))) & (uS64)1) == 0)
            free++;
        else
          start++;
    }
    
    // not enough continuously free items
    if (free < count)
      return 0;  // FAIL
    
    // allocate items into bitmap
    for(i = start; i < (count + start); i++)
      rsx_utils_bitmap_set_bit(bm_obj, i, 1);
    
    // e.g. in case of memory, new address
    *out = bm_obj->value1 + (start * bm_obj->value2);
    
    return 1;  // OK
}

/***********************************************************************
* set a item into a bitmap
* 
* rsx_utils_bm_obj_t* bm_obj = bitmap object
* S32 idx                = bit position into bitmap
* S32 value              = bit value to set, 0(free) or 1(allocated)
***********************************************************************/
void rsx_utils_bitmap_set_bit(rsx_utils_bm_obj_t* bm_obj, S32 idx, S32 value) {
    S64 *bm = NULL;
    uS64 segment, tmp;
    S32 sh;
    
    
    // if idx out of bitmap range, error
    if (bm_obj->seg_count <= (idx /bm_obj->seg_base)) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    bm = bm_obj->bitmap;
    
    sh = idx - ((idx / bm_obj->seg_base) * bm_obj->seg_base);
    segment = bm[idx / bm_obj->seg_base];
    tmp = ((segment & ~((uS64)1 <<sh)) | ((value & (uS64)1) <<sh));
    bm[idx / bm_obj->seg_base] = tmp;
    
    return;
}

/***********************************************************************
* set a bitmap object
* 
* rsx_utils_bm_obj_t* bm_obj = bitmap object to set
* S64 val_1              = user value 1, base, e.g. a start address
* S32 item_total         = total count of items into bitmap
* S64 val_2              = user value 2, item size, e.g. a page size
***********************************************************************/
void rsx_utils_bitmap_create(rsx_utils_bm_obj_t* bm_obj, S64 val_1, S32 item_total, S64 val_2) {
    S32 i, seg_count;
    S64 *bitmap = NULL;
    
    
    // compute count of needed 64 bit segments for our map
    seg_count = (item_total >>6) + 1;
    
    // init members
    bm_obj->value1     = val_1;       // user value 1, e.g. a memory start address
    bm_obj->value2     = val_2;       // user value 2, e.g. a page size for one item of bitmap
    bm_obj->unk_20     = 0;           // ?
    bm_obj->seg_base   = 64;          // count of bits per segment
    bm_obj->item_total = item_total;  // count of items to handle
    bm_obj->seg_count  = seg_count;
    
    // allocate bitmap
    bitmap = lv1_kmalloc(seg_count * sizeof(uS64));
    if (bitmap == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // store bitmap address into bitmap object
    bm_obj->bitmap = (void*)bitmap;
    
    // if item_count == 0, nothing to do
    if (item_total == 0)
      return;
    
    // else, init full allocated bitmap with 1
    memset(bitmap, 0xFF, seg_count * sizeof(uS64));
    
    // init usable part of bitmap with 0(unset)
    for(i = 0; i < item_total; i++)
        rsx_utils_bitmap_set_bit(bm_obj, i, 0);
        
    return;
}
