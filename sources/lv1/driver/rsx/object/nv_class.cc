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
* ? maybe wrong
***********************************************************************/
S32 rsx_object_nv_class_get_object_size() {
    return 0x20;
}






/***********************************************************************
* 
***********************************************************************/
void rsx_object_nv_class_create_object(rsx_nv_class_obj_t* nv_obj, uS32 arg1, uS32 arg2) {
    S32 ret = -1, offset, value1, value2;
    rsx_dev_core_obj_t* core = NULL;
    rsx_core_mem_obj_t* core_mem = NULL;
    rsx_utils_bm_obj_t* bm = NULL;
    
    
    nv_obj->unk_00 = arg1;          // ?
    nv_obj->unk_04 = arg2;          // ?
    
    // get RSX device core object
  core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    
    // get core memory object
  core_mem = (void*)core->core_mem_obj;
  
  // get class(sw/nv) object bitmap
  bm = (void*)core_mem->bm_class_nv_sw;
  
  // allocate 1 of the 128 class objects
    ret = rsx_utils_bitmap_allocate(bm, 1, &nv_obj->unk_08);
    if (ret == 0)  // allocation fails {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  
  // get offset
  offset = rsx_core_memory_2120EC((void*)core->core_mem_obj, nv_obj->unk_08);
  
  // ?
  value1 = arg1 & 0xFF;
  
  if (arg1 == 0x4097)
    value2 = 0;
  else if ((arg1 <= 0x308A) && (arg1 >= 0x3089))
        value2 = 0x18000000;
  else if (arg1 == 0x309E)
    value2 = 0;
  else if (arg1 == 0x3062)
    value2 = 0;
  else if (arg1 == 0x39) {
    value1 |= 0x2000000;
    value2 = 0;
    }
  else if (arg1 == 0x30)
    value2 = 0;
  else {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
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


