#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



/***********************************************************************
* get total item count of a bitmap
***********************************************************************/
int32_t rsx_utils_bitmap_get_item_total(rsx_utils_bm_obj_t *bm_obj)
{
	return bm_obj->item_total;
}

/***********************************************************************
* free the bitmap of a bitmap object
***********************************************************************/
void rsx_utils_bitmap_free_map(rsx_utils_bm_obj_t *bm_obj)
{
	lv1_kfree((void *)bm_obj->bitmap);
	bm_obj->bitmap = NULL;
	
	return;
}

/***********************************************************************
* free a item into a bitmap
* 
* rsx_utils_bm_obj_t *bm_obj = bitmap object
* int64_t start              = start of continuously items to free
* int32_t count              = count of continuously items to free
***********************************************************************/
int32_t rsx_utils_bitmap_dealloc(rsx_utils_bm_obj_t *bm_obj, int64_t start, int32_t count)
{
	int32_t i, bm_idx;
	int64_t *bm = NULL;
	uint64_t seg;
	
	
	// nothing to free
	if(count == 0)
		return 0;  // FAIL
	
	bm_idx = (int32_t)((start - bm_obj->value1) / bm_obj->value2);
	
	bm = bm_obj->bitmap;
	
	// free item, if set
	for(i = bm_idx; i < count; i++)
	{
		seg = bm[i / bm_obj->seg_base];
		if(((seg >>(i - ((i / bm_obj->seg_base) * bm_obj->seg_base))) & (uint64_t)1) == 1)
		  rsx_utils_bitmap_set_bit(bm_obj, i, 0);
	}
	
	return 1;  // OK
}

/***********************************************************************
* allocate a item into a bitmap
* 
* rsx_utils_bm_obj_t *bm_obj = bitmap object
* int32_t count              = count of continuously items to allocate
* int64_t *out               = start of the allocated continuously items
***********************************************************************/
int32_t rsx_utils_bitmap_allocate(rsx_utils_bm_obj_t *bm_obj, int32_t count, int64_t *out)
{
	int32_t i, free = 0, start = 0;
	int64_t *bm = NULL;
	uint64_t seg;
	
	
	// fail, nothing to allocate in bitmap
	if(bm_obj->item_total == 0)
	  return 0;  // FAIL
	
	// count to allocate is 0
	if(count == 0)
	{
		*out = bm_obj->value1;
		return 1;  // OK
	}
	
	bm = bm_obj->bitmap;
	
	// get start and count of free items into bitmap
	for(i = 0; i < bm_obj->item_total; i++)
	{
		seg = bm[i / bm_obj->seg_base];
		
		// item free
		if(((seg >>(i - ((i / bm_obj->seg_base) * bm_obj->seg_base))) & (uint64_t)1) == 0)
			free++;
		else
		  start++;
	}
	
	// not enough continuously free items
	if(free < count)
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
* rsx_utils_bm_obj_t *bm_obj = bitmap object
* int32_t idx                = bit position into bitmap
* int32_t value              = bit value to set, 0(free) or 1(allocated)
***********************************************************************/
void rsx_utils_bitmap_set_bit(rsx_utils_bm_obj_t *bm_obj, int32_t idx, int32_t value)
{
	int64_t *bm = NULL;
	uint64_t segment, tmp;
	int32_t sh;
	
	
	// if idx out of bitmap range, error
	if(bm_obj->seg_count <= (idx /bm_obj->seg_base))
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	bm = bm_obj->bitmap;
	
	sh = idx - ((idx / bm_obj->seg_base) * bm_obj->seg_base);
	segment = bm[idx / bm_obj->seg_base];
	tmp = ((segment & ~((uint64_t)1 <<sh)) | ((value & (uint64_t)1) <<sh));
	bm[idx / bm_obj->seg_base] = tmp;
	
	return;
}

/***********************************************************************
* set a bitmap object
* 
* rsx_utils_bm_obj_t *bm_obj = bitmap object to set
* int64_t val_1              = user value 1, base, e.g. a start address
* int32_t item_total         = total count of items into bitmap
* int64_t val_2              = user value 2, item size, e.g. a page size
***********************************************************************/
void rsx_utils_bitmap_create(rsx_utils_bm_obj_t *bm_obj, int64_t val_1, int32_t item_total, int64_t val_2)
{
	int32_t i, seg_count;
	int64_t *bitmap = NULL;
	
	
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
	bitmap = lv1_kmalloc(seg_count * sizeof(uint64_t));
	if(bitmap == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// store bitmap address into bitmap object
	bm_obj->bitmap = (void*)bitmap;
	
	// if item_count == 0, nothing to do
	if(item_total == 0)
	  return;
	
	// else, init full allocated bitmap with 1
	memset(bitmap, 0xFF, seg_count * sizeof(uint64_t));
	
	// init usable part of bitmap with 0(unset)
	for(i = 0; i < item_total; i++)
		rsx_utils_bitmap_set_bit(bm_obj, i, 0);
		
	return;
}
