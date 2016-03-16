#ifndef __LV1_MISC_H__
#define __LV1_MISC_H__


#include "rsx_lv1.h"



////////////////////////////////////////////////////////////////////////
// misc LV1 structs

// LV1 unk struct, size 0x28, not spezial RSX related, maybe interrupt stuff ?
typedef struct _lv1_unk_obj_00_t
{
	int64_t unk_00;                  // 0x00: 
	int64_t unk_08;                  // 0x08: 
	int64_t *unk_10;                 // 0x10: 
	int64_t unk_18;                  // 0x18: 
	int64_t unk_20;                  // 0x20: 
} __attribute__((packed)) lv1_unk_obj_00_t;







////////////////////////////////////////////////////////////////////////
// misc LV1 functions



int32_t lv1_sub_2D7A14(int32_t arg1, int64_t ea, int32_t lm_offset, int32_t arg2, uint64_t flags, int32_t arg3);
int32_t count_set_bits(int64_t value);
int32_t allocate_io_segment(int64_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7, int64_t *out);
void lv1_sub_2CA414(lv1_unk_obj_00_t *obj_0, int64_t arg1, int64_t opd_64, void *obj_1);
void get_param_value(const char *name, uint64_t *value);
void *lv1_kmalloc(int32_t size);
void lv1_kfree(void *addr);
int32_t rsx_map_io_to_lpar(int32_t core_id, int64_t io_addr, int32_t arg1, int32_t io_size, int64_t *lpar_addr);



#endif // __LV1_MISC_H__
