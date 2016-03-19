/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "context_dma.h"

#include "lv1/lv1_misc.h"
#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/vram.h"
#include "lv1/driver/rsx/core/device.h"

S32 rsx_object_context_dma_t::get_object_size() const {
    return 0x10;
}
U32 rsx_object_context_dma_t::get_object_handle() const {
    return handle;
}
S64 rsx_object_context_dma_t::get_bar1_offset() const {
    return bar1_offset;
}
S64 rsx_object_context_dma_t::get_bar2_offset() const {
    return bar2_offset;
}


void rsx_object_context_dma_t::sub220064(S32 type, S32 arg2, S64 addr, S32 size) {
    U32 dma_object;
    U32 dma_offset;
    U32 dma_size = size ? size - 1 : 0;

    switch (type) {
    case L1GPU_CONTEXT_DMA_TYPE1:  dma_object = 0x3002; break;
    case L1GPU_CONTEXT_DMA_TYPE2:  dma_object = 0x3003; break;
    case L1GPU_CONTEXT_DMA_TYPE3:  dma_object = 0x303D; break;
    default:
        RSX_ASSERT(false);
    }
  
    // Get RSX device core object
    rsx_core_device_t* core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
  
    rsx_core_memory_t* mem = core->core_mem;
    if (arg2 == 4) {
        addr = mem->get_bar1_offset_by_address(addr);
        dma_object = (addr << 20) | dma_object;
    }
    else if (arg2 == 8)
        dma_object = (type | 0x20000) | (addr << 20);
    else {
        RSX_ASSERT(false);
    }
    
    // ? alignment check
    RSX_ASSERT(addr & 0xF == 0);
    
    // store address into dma object
    bar1_offset = (S32)addr;
    
    // get entry offset
    U32 ramin_offset = mem->sub2120EC(dma_obj->bar2_offset);
    
    // Write DMA object
    vram_wr32(dma_object,     ramin_offset + 0x0);
    vram_wr32(dma_size,       ramin_offset + 0x4);
    vram_wr32(dma_offset | 3, ramin_offset + 0x8);
    vram_wr32(dma_offset | 3, ramin_offset + 0xC);
}

/***********************************************************************
* 
***********************************************************************/
rsx_object_context_dma_t::rsx_object_context_dma_t(U32 handle) : handle(handle), bar1_offset(~0ULL) {
    rsx_core_device_t* core = NULL;
    rsx_core_memory_t* core_mem = NULL;
    rsx_utils_bitmap_t* bm_ctx_dma = NULL;

    // Get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
  
    // Get core memory object
    core_mem = core->core_mem;
  
    // Get context DMA object bitmap
    bm_ctx_dma = core_mem->bm_ctx_dma;
  
    // allocate 1 of the 256 context DMA objects
    S64 idx;
    S32 ret = bm_ctx_dma->allocate(1, &idx);
    RSX_ASSERT(ret);
    
    // init dma object
	obj_class    = obj_class;         // store class
	bar2_offset  = addr;              // store BAR2 dma object address
	bar1_offset  = -1;                // init BAR1 offset with -1(invalid)
}
