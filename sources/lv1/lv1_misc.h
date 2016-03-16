#include "common/types.h"

#include "rsx_lv1.h"



////////////////////////////////////////////////////////////////////////
// misc LV1 structs

// LV1 unk struct, size 0x28, not spezial RSX related, maybe interrupt stuff ?
struct _lv1_unk_obj_00_t {
	S64 unk_00;                  // 0x00: 
	S64 unk_08;                  // 0x08: 
	S64 *unk_10;                 // 0x10: 
	S64 unk_18;                  // 0x18: 
	S64 unk_20;                  // 0x20: 
} lv1_unk_obj_00_t;







////////////////////////////////////////////////////////////////////////
// misc LV1 functions



S32 lv1_sub_2D7A14(S32 arg1, S64 ea, S32 lm_offset, S32 arg2, U64 flags, S32 arg3);
S32 count_set_bits(S64 value);
S32 allocate_io_segment(S64 arg1, S32 arg2, S32 arg3, S32 arg4, S32 arg5, S32 arg6, S32 arg7, S64 *out);
void lv1_sub_2CA414(lv1_unk_obj_00_t* obj_0, S64 arg1, S64 opd_64, void *obj_1);
void get_param_value(const char *name, U64 *value);
void *lv1_kmalloc(S32 size);
void lv1_kfree(void *addr);
S32 rsx_map_io_to_lpar(S32 core_id, S64 io_addr, S32 arg1, S32 io_size, S64 *lpar_addr);



#endif // __LV1_MISC_H__
