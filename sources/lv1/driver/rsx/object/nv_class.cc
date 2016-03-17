/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "nv_class.h"

#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/core/device.h"

S32 rsx_object_nv_class_t::get_object_size() {
    return 0x20; // TODO:  Maybe wrong?
}

U32 rsx_object_nv_class_t::get_object_handle() {
    return handle;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_object_nv_class_create_object(rsx_object_nv_class_t* nv_obj, U32 type, U32 handle) {
    S32 ret, offset;
    rsx_core_device_t* core = nullptr;
    rsx_core_memory_t* core_mem = nullptr;
    rsx_utils_bitmap_t* bm = nullptr;

    nv_obj->type = type;
    nv_obj->handle = handle;
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // Allocate 1 of the 128 class objects in the NV/SW object bitmap
    bm = core_mem->bm_class_nv_sw;
    ret = bm->allocate(1, &nv_obj->bar2_offset);
    RSX_ASSERT(ret);
  
    // Get offset from core memory object
    core_mem = core->core_mem;
    offset = core_mem->get_bar2_offset_by_address(nv_obj->bar2_offset);
  
    // ?
    S32 value2;
    S32 value1 = type & 0xFF;
    switch (type) {
    case 0x0030: value1 |= 0x00000000; value2 = 0x00000000; break;
    case 0x3062: value1 |= 0x00000000; value2 = 0x00000000; break;
    case 0x309E: value1 |= 0x00000000; value2 = 0x00000000; break;
    case 0x4097: value1 |= 0x00000000; value2 = 0x00000000; break;
    case 0x0039: value1 |= 0x02000000; value2 = 0x00000000; break;
    case 0x3089: value1 |= 0x00000000; value2 = 0x18000000; break;
    case 0x308A: value1 |= 0x00000000; value2 = 0x18000000; break;
    default:
        RSX_ASSERT(false);
    }
    
    // ?
    DDR_read32(offset + g_rsx_bar2_addr);
    DDR_write32(value1, offset + g_rsx_bar2_addr);
    
    DDR_read32(offset + g_rsx_bar2_addr + 4);
    DDR_write32(value2, offset + g_rsx_bar2_addr + 4);
    
    DDR_read32(offset + g_rsx_bar2_addr + 8);
    DDR_write32(0x1000000, offset + g_rsx_bar2_addr + 8);
    
    DDR_read32(offset + g_rsx_bar2_addr + 0xC);
    DDR_write32(0, offset + g_rsx_bar2_addr + 0xC);
    
    DDR_read32(offset + g_rsx_bar2_addr + 0x10);
    DDR_write32(0, offset + g_rsx_bar2_addr + 0x10);
    
    //printf("addr: 0x%016llX\n", offset + g_rsx_bar2_addr);
    return;
}
