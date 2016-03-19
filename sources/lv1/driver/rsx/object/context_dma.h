/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

enum {
    L1GPU_CONTEXT_DMA_TYPE1 = 1,
    L1GPU_CONTEXT_DMA_TYPE2 = 2,
    L1GPU_CONTEXT_DMA_TYPE3 = 3,
};

// LV1 RSX object context DMA object, size 0x18
struct rsx_object_context_dma_t {
    U32 handle;        // 0x00: 
    S32 unk_04;        // 0x04: 
    S64 bar2_offset;   // 0x08: PRAMIN offset
    S64 bar1_offset;   // 0x10: VRAM offset

    // Methods
    S32 get_object_size();
    U32 get_object_handle();
    S64 get_bar1_offset();
    S64 get_bar2_offset();

    rsx_object_context_dma_t(U32 type);
    void sub220064(S32 type, S32 arg2, S64 addr, S32 size);
};
