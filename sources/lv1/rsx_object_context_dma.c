#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"




/***********************************************************************
* get size of a context dma object
***********************************************************************/
int32_t rsx_object_context_dma_get_object_size()
{
	return 0x10;
}

/***********************************************************************
* 
***********************************************************************/
int64_t rsx_object_context_dma_object_get_BAR2_address(rsx_ctx_dma_obj_t *dma_obj)
{
	return dma_obj->bar2_addr;
}

/***********************************************************************
* 
***********************************************************************/
uint32_t rsx_object_context_dma_get_object_class(rsx_ctx_dma_obj_t *dma_obj)
{
	return dma_obj->class;
}



/***********************************************************************
* 
* rsx_ctx_dma_obj_t *dma_obj = DMA object to work with
* int32_t arg1               = ? 1(?), 2(?) or 3(?)
* int32_t arg2               = ? 4(?) or 8(?)
* int64_t bar1_addr          = BAR1(VRAM) address
* int32_t size               = BAR1(VRAM) size
***********************************************************************/
void rsx_object_context_dma_220064(rsx_ctx_dma_obj_t *dma_obj, int32_t arg1, int32_t arg2, int64_t bar1_addr, int32_t size)
{
	int32_t value1 = 0, value2 = 0, offset = 0;
	rsx_dev_core_obj_t *core = NULL;
	
	
	// ? based on arg1
	if(arg1 == 1)
	  value1 = 0x3002;
	else if(arg1 == 2)
	  value1 = 0x3003;
	else if(arg1 == 3)
	  value1 = 0x303D;
	else
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
	  return;
	}
  
  // get RSX device core object
  core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if(core == NULL)
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  
	// ? based on arg2
	if(arg2 == 4)
	{
		bar1_addr = rsx_core_memory_get_BAR1_offset_by_address((void*)core->core_mem_obj, bar1_addr);
		value2 = value1 | (bar1_addr <<20);
	}
	else if(arg2 == 8)
	{
		value2 = (value1 | 0x20000) | (bar1_addr <<20);
	}
	else
	{
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
	  return;
  }
	
	// address alignment check
	if((bar1_addr & 0xF) != 0)
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
	  return;
  }
	
	// store BAR1 offset
	dma_obj->bar1_offset = (int32_t)bar1_addr;
	
	
	// 
	if(size != 0)
	  size--;
	
	// get entry offset in BAR2(RAMIN)
	offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, dma_obj->bar2_addr);
  
	// write entry
	DDR_write32(value2,        offset + g_rsx_bar2_addr);
	DDR_write32(size,          offset + g_rsx_bar2_addr + 4);
	DDR_write32(bar1_addr | 3, offset + g_rsx_bar2_addr + 8);
	DDR_write32(bar1_addr | 3, offset + g_rsx_bar2_addr + 0xC);
	
	return;
}

/***********************************************************************
* 
***********************************************************************/
rsx_ctx_dma_obj_t *rsx_object_context_dma_create_obj(uint32_t class)
{
	int32_t ret = -1;
	int64_t addr;
	rsx_dev_core_obj_t *core = NULL;
	rsx_core_mem_obj_t *core_mem = NULL;
	rsx_utils_bm_obj_t *bm_ctx_dma = NULL;
	rsx_ctx_dma_obj_t *dma_obj = NULL;
	
	
	// get RSX device core object
	core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if(core == NULL)
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
  
  // get core memory object
  core_mem = (void*)core->core_mem_obj;
  
  // get context DMA object bitmap
  bm_ctx_dma = (void*)core_mem->bm_ctx_dma;
  
  // allocate 1 of the 256 context DMA objects, addr = BAR2 address of object
	ret = rsx_utils_bitmap_allocate(bm_ctx_dma, 1, &addr);
	if(ret == 0)  // allocation fails
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
	
	// allocate object
	dma_obj = lv1_kmalloc(sizeof(rsx_ctx_dma_obj_t));
	if(dma_obj == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return 0;
	}
	
	// init dma object
	dma_obj->class       = class;             // store class
	dma_obj->bar2_addr   = addr;              // store BAR2 dma object address
	dma_obj->bar1_offset = -1;                // init BAR1 offset with -1(invalid)
	
	return dma_obj;
}
