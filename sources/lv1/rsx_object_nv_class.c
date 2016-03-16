#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



/***********************************************************************
* 
***********************************************************************/
int32_t rsx_object_nv_class_get_object_size()
{
	return 0x20;
}

/***********************************************************************
* 
***********************************************************************/
int64_t rsx_object_nv_class_get_BAR2_address(rsx_nv_class_obj_t *nv_obj)
{
	return nv_obj->bar2_addr;
}

/***********************************************************************
* 
***********************************************************************/
uint32_t rsx_object_nv_class_get_object_class(rsx_nv_class_obj_t *nv_obj)
{
	return nv_obj->class;
}




/***********************************************************************
* 
***********************************************************************/
void rsx_object_nv_class_create_object(rsx_nv_class_obj_t *nv_obj, uint32_t arg1, uint32_t class)
{
	int32_t ret = -1, offset, value1, value2;
	rsx_dev_core_obj_t *core = NULL;
	rsx_core_mem_obj_t *core_mem = NULL;
	rsx_utils_bm_obj_t *bm = NULL;
	
	
	nv_obj->unk_00 = arg1;          // ?
	nv_obj->class  = class;         // set nv object class
	
	// get RSX device core object
  core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if(core == NULL)
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
	
	// get core memory object
  core_mem = (void*)core->core_mem_obj;
  
  // get class(sw/nv) object bitmap
  bm = (void*)core_mem->bm_class_nv_sw;
  
  // allocate 1 of the 128 class objects
	ret = rsx_utils_bitmap_allocate(bm, 1, &nv_obj->bar2_addr);
	if(ret == 0)  // allocation fails
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  
  // get offset
  offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, nv_obj->bar2_addr);
  
  // ?
  value1 = arg1 & 0xFF;
  
  if(arg1 == 0x4097)
    value2 = 0;
  else if((arg1 <= 0x308A) && (arg1 >= 0x3089))
		value2 = 0x18000000;
  else if(arg1 == 0x309E)
    value2 = 0;
  else if(arg1 == 0x3062)
    value2 = 0;
  else if(arg1 == 0x39)
  {
    value1 |= 0x2000000;
    value2 = 0;
	}
  else if(arg1 == 0x30)
    value2 = 0;
  else
  {
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
	
	return;
}


