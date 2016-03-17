/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "context_dma.h"

#include "lv1/lv1_misc.h"

S32 rsx_object_context_dma_t::get_object_size() {
    return 0x10;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_object_context_dma_t::sub220064(S32 arg1, S32 arg2, S64 addr, S32 size) {
    S32 value1 = 0, value2 = 0, offset = 0;
    rsx_core_device_t* core = NULL;
    
    
    // ? based on arg1
    if (arg1 == 1)
      value1 = 0x3002;
    else if (arg1 == 2)
      value1 = 0x3003;
    else if (arg1 == 3)
      value1 = 0x303D;
    else {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
  }
  
  // get RSX device core object
  core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  
    // ? based on arg2
    if (arg2 == 4) {
        addr = rsx_core_memory_get_BAR1_offset_by_address((void*)core->core_mem, addr);
        value2 = (addr <<20) | value1;
    }
    else if (arg2 == 8)
        value2 = (value1 | 0x20000) | (addr <<20);
    else {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
  }
    
    // ? alignment check
    if ((addr &= 0xF) != 0) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
  }
    
    // ?
    if (size != 0)
      size--;
    else
      size = 0;
    
    // store address into dma object
    dma_obj->unk_10 = (S32)addr;
    
    // get entry offset
    offset = rsx_core_memory_2120EC((void*)core->core_mem, dma_obj->unk_08);
    
    // write entry
    DDR_write32(  value2, offset + g_rsx_bar2_addr);
    DDR_write32(    size, offset + g_rsx_bar2_addr + 4);
    DDR_write32(addr | 3, offset + g_rsx_bar2_addr + 8);
    DDR_write32(addr | 3, offset + g_rsx_bar2_addr + 0xC);
    
    //printf("addr: 0x%016llX\n", offset + g_rsx_bar2_addr);
    return;
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
    core_mem = (void*)core->core_mem;
  
    // Get context DMA object bitmap
    bm_ctx_dma = (void*)core_mem->bm_ctx_dma;
  
    // allocate 1 of the 256 context DMA objects
    S64 idx;
    S32 ret = rsx_utils_bitmap_allocate(bm_ctx_dma, 1, &idx);
    RSX_ASSERT(ret);
    
    // init dma object
	obj_class    = obj_class;         // store class
	bar2_offset  = addr;              // store BAR2 dma object address
	bar1_offset  = -1;                // init BAR1 offset with -1(invalid)
}
