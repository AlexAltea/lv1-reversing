/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX object context DMA object, size 0x18
struct rsx_object_context_dma_t {
    U32 obj_class;              // 0x00: 
    S32 unk_04;                 // 0x04: 
    S64 bar2_offset;            // 0x08: BAR2(PRAMIN) offset
    S64 bar1_offset;            // 0x10: BAR1(VRAM) offset

    // Methods
    S32 get_object_size();
    U32 get_object_class();
    S64 get_bar1_offset();
    S64 get_bar2_offset();

    rsx_object_context_dma_t(U32 type);
    void sub220064(rsx_object_context_dma_t* dma_obj, S32 arg1, S32 arg2, S64 addr, S32 size);
};
