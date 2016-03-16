#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



static int32_t rsx_mem_seg_set_max = 12;

// memory regions settings
static rsx_mem_reg_setting_t set[12] =
{
	{ 1, 0, 15,        0, 0x100000},  // 
	{ 2, 0, 15,  0x10000,   0x8000},  // hash table
	{ 3, 0, 15,  0x18000,    0x200},  // device fifo, run out
	{ 4, 0, 15,  0x20000,    0x200},  // device fifo, fc1
	{ 5, 0, 15,  0x30000,   0x4000},  // device fifo, fc2
	{ 6, 0, 15,  0x60000,  0x80000},  // device graph
	{ 7, 0, 15,  0x40000,   0x1000},  // context dma objects
	{ 8, 0, 15,  0x50000,   0x1000},  // class objects, sw(software) nv(hardware)
	{ 9, 0, 14,  0xE0000,   0x1000},  // rsx device 1, audio
	{10, 0, 14, 0x200000,  0x40000},  // reports
	{11, 0, 14,       -1,   0xC000},
	{12, 0, 14,  0xF0000,   0x1000}   // rsx device 8, gpu, aka global semaphore
};




/***********************************************************************
* 
***********************************************************************/
int32_t rsx_core_memory_get_BAR1_offset_by_address(rsx_core_mem_obj_t *core_mem, int64_t addr)
{
	int32_t offset = 0;
	
	
	offset = addr - core_mem->bar_1_addr;
	if(core_mem->bar_1_size <= offset)
	{
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
	
	return offset;
}


/***********************************************************************
* get RSX memory context by ID
* 
* rsx_core_mem_obj_t *core_mem = RSX device core object
* uint32_t mem_ctx_id          = RSX memory context ID
***********************************************************************/
rsx_mem_ctx_obj_t *rsx_core_memory_get_memory_context_by_id(rsx_core_mem_obj_t *core_mem, uint32_t mem_ctx_id)
{
	rsx_mem_ctx_obj_t *mem_ctx = NULL;
	
	
	// if id out of range
	if((mem_ctx_id ^ 0x5A5A5A5A) > 15)
	  return NULL;
	
	mem_ctx = (void*)core_mem->rsx_mem_ctx[mem_ctx_id ^ 0x5A5A5A5A];
	
	return mem_ctx;
}


/***********************************************************************
* 
***********************************************************************/
rsx_core_mem_obj_t *rsx_core_memory_allocate_memory_context(rsx_core_mem_obj_t *core_mem, int32_t local_size, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4)
{
	int32_t ctx_id, item_count;
	int32_t ret1, ret2, ret3, ret4, ret5; 
	int64_t out1, out2, out3, out4, out5;
	rsx_mem_ctx_obj_t *mem_ctx = NULL;
	
	
	// search a free RSX memory context into core memory object
	for(ctx_id = 0; ctx_id < 16; ctx_id++)
	{
		// if memory context free and local size have the right alignment (1 MB)
		if((core_mem->rsx_mem_ctx[ctx_id] == NULL) && (((core_mem->ddr_page_size - 1) & local_size) == 0))
		{
			// allocate pages
			item_count = local_size / core_mem->ddr_page_size;
			ret1 = rsx_utils_bitmap_allocate((void*)core_mem->bm_local_mem, item_count, &out1);
			
			// ? 8 / 0x400
			item_count = arg1 / core_mem->unk_048;
			ret2 = rsx_utils_bitmap_allocate((void*)core_mem->bm_unk_00, item_count, &out2);
			
			// ? 0x30000 / 0x1000, 48 what, and why is the bitmap ever FF..., nothing is allocated here
			item_count = arg2 / core_mem->unk_0B8;
			ret3 = rsx_utils_bitmap_allocate((void*)core_mem->bm_unk_01, item_count, &out3);
			
			// ? 
			item_count = arg3; // 15
			ret4 = rsx_utils_bitmap_allocate((void*)core_mem->bm_unk_02, item_count, &out4);
			
			// ? 
			item_count = arg4; // 8
			ret5 = rsx_utils_bitmap_allocate((void*)core_mem->bm_unk_03, item_count, &out5);
			
			// in case of any bitmap allocation error, rollback
			if((ret1 & ret2 & ret3 & ret4 & ret5) != 0)
			{
				if(ret1)
				{
					item_count = local_size / core_mem->ddr_page_size;
					rsx_utils_bitmap_dealloc((void*)core_mem->bm_local_mem, out1, item_count);
				}
				
				if(ret2)
				{
					item_count = arg1 / core_mem->unk_048;
					rsx_utils_bitmap_dealloc((void*)core_mem->bm_unk_00, out2, item_count);
				}
				
				if(ret3)
				{
					item_count = arg2 / core_mem->unk_0B8;
					rsx_utils_bitmap_dealloc((void*)core_mem->bm_unk_01, out3, item_count);
				}
				
				if(ret4)
				{
					item_count = arg3;
					rsx_utils_bitmap_dealloc((void*)core_mem->bm_unk_02, out4, item_count);
				}
				
				if(ret5)
				{
					item_count = arg4;
					rsx_utils_bitmap_dealloc((void*)core_mem->bm_unk_03, out5, item_count);
				}
				
				return NULL;
			}
			
			// allocate RSX memory context object
			mem_ctx = lv1_kmalloc(sizeof(rsx_mem_ctx_obj_t));
			if(mem_ctx == NULL)
	    {
		    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		    return 0;
	    }
			
			// set memory context
			mem_ctx->unk_40 = ((1 <<arg3) - 1) <<out4;           // 0x7FFF, a bitmap ?
			mem_ctx->unk_48 = ((1 <<arg4) - 1) <<out5;           // 0xFF, a bitmap ?
			
			mem_ctx->lm_end_lpar = local_size + out1;            // memory context DDR start LPAR + local size
			mem_ctx->ddr_lpar = core_mem->ddr_lpar_addr;         // memory context DDR base LPAR
			mem_ctx->lm_start_lpar = out1;                       // memory context DDR start LPAR
			
			mem_ctx->core_mem_obj = (void*)core_mem;             // core memory object
			mem_ctx->id = ctx_id ^ 0x5A5A5A5A;                   // memory context ID
			
			mem_ctx->unk_30 = core_mem->unk_0B8 * out3;          // ?
			mem_ctx->unk_38 = arg2 + (core_mem->unk_0B8 * out3); // ?
			
			mem_ctx->unk_28 = core_mem->unk_048 * out2;          // ?
			mem_ctx->unk_2C = arg1 + (core_mem->unk_048 * out2); // ?
			
			// store new RSX memory context object into RSX core memory object
			core_mem->rsx_mem_ctx[ctx_id] = (void*)mem_ctx;
			
			return (void*)mem_ctx;
		}
	}
	
	// no free memory context 
	return NULL;
}

/***********************************************************************
* 
***********************************************************************/
int32_t rsx_core_memory_value_div_by_16(rsx_core_mem_obj_t *core_mem, int32_t offset)
{
	return offset >>4;
}

/***********************************************************************
* 
***********************************************************************/
int32_t rsx_core_memory_get_BAR2_offset_by_address(rsx_core_mem_obj_t *core_mem, int64_t addr)
{
	int64_t size;
	
	
	// addr - start address(0x28002000000) = offset
	addr -= rsx_core_memory_get_mem_reg_addr_by_id(core_mem, 1);
	
	// get BAR0 size, 0x100000
	size = rsx_core_memory_get_mem_reg_size_by_id(core_mem, 1);
	
	// offset out of range ?
	if(addr > size)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return 0;
	}
	
	return addr;  // return offset into area 0x28002000000 BAR2
}


/***********************************************************************
* 
***********************************************************************/
static int32_t rsx_core_memory_get_mem_reg_addr_and_size_by_id(rsx_core_mem_obj_t *core_mem, int32_t mem_region_id, int64_t *addr, int32_t *size)
{
	int32_t i;
	
	
	if(rsx_mem_seg_set_max == 0)
	  return LV1_ILLEGAL_PARAMETER_VALUE;
	
	for(i = 0; i < rsx_mem_seg_set_max; i++)
	{
		if(set[i].id == mem_region_id)
		{
			if(set[i].type == 0xD)
			{
				*addr = core_mem->bar_1_addr + set[i].offset;
				*size = set[i].size;
				return LV1_SUCCESS;
			}
			
			if(set[i].type == 0xE)
			{
				*addr = (core_mem->bar_1_addr + core_mem->bar_1_size) - set[i].offset;
				*size = set[i].size;
				return LV1_SUCCESS;
			}
			
			if(set[i].type == 0xF)
			{
				*addr = core_mem->unk_080 + set[i].offset;
				*size = set[i].size;
				return LV1_SUCCESS;
			}
		}
	}
	
	return LV1_ILLEGAL_PARAMETER_VALUE;
}

/***********************************************************************
* 
***********************************************************************/
int64_t rsx_core_memory_get_mem_reg_addr_by_id(rsx_core_mem_obj_t *core_mem, int32_t mem_region_id)
{
	int32_t ret = -1;
	int64_t addr;
	int32_t size;
	
  
	ret = rsx_core_memory_get_mem_reg_addr_and_size_by_id(core_mem, mem_region_id, &addr, &size);
	if(ret != 0)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return 0;
	}
	
	return addr;
}

/***********************************************************************
* 
***********************************************************************/
int32_t rsx_core_memory_get_mem_reg_size_by_id(rsx_core_mem_obj_t *core_mem, int32_t mem_region_id)
{
	int32_t ret = -1;
	int64_t addr;
	int32_t size;
	
	ret = rsx_core_memory_get_mem_reg_addr_and_size_by_id(core_mem, mem_region_id, &addr, &size);
	if(ret != 0)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return 0;
	}
	
	return size;
}


/***********************************************************************
* 
***********************************************************************/
static void rsx_core_memory_build_core_mem_obj(rsx_core_mem_obj_t *core_mem, int32_t core_id, int64_t BAR1_addr,
                                               int32_t BAR1_size, int64_t BAR2_addr, int32_t arg1, int32_t arg2,
                                               int32_t arg3, int32_t arg4, int32_t arg5, int32_t DDR_MB_CTags, int64_t *status)
{
	int32_t i, ret = -1, size;
	int32_t item_total;
	int64_t addr;
	rsx_utils_bm_obj_t *bm_local_mem = NULL;
	rsx_utils_bm_obj_t *bm_reports = NULL;
	rsx_utils_bm_obj_t *bm_unk_00 = NULL;
	rsx_utils_bm_obj_t *bm_unk_01 = NULL;
	rsx_utils_bm_obj_t *bm_unk_02 = NULL;
	rsx_utils_bm_obj_t *bm_unk_03 = NULL;
	rsx_utils_bm_obj_t *bm_ctx_dma = NULL;
	rsx_utils_bm_obj_t *bm_class_nv_sw = NULL;
	rsx_utils_bm_obj_t *bm_driver_info = NULL;
	
  
	// store values
	core_mem->unk_030        = arg1;            // ?
	core_mem->unk_034        = arg2;            // ?
	core_mem->unk_038        = arg3;            // ?
	core_mem->core_id        = core_id;         // device core ID
	core_mem->unk_040        = arg5;            // ?
	core_mem->bar_1_addr     = BAR1_addr;       // 0x28080000000
	core_mem->bar_1_size     = BAR1_size;       // 0x10000000
	core_mem->unk_080        = BAR2_addr;       // ? 0x28002000000
	core_mem->ddr_sys_reserv = 0x200000;        // 2 MB system reserved DDR memory
	core_mem->unk_03C        = arg4;            // ? 8
	core_mem->DDR_MB_CTags   = DDR_MB_CTags;    // ?
	core_mem->unk_188        = NULL;            // HSPRG0 context value, I set it to NULL
	
	// store new global core memory object
	g_core_mem_obj = (void*)core_mem;
	
	// driver info
  // I use a extern buffer with BE read/write methodes
	core_mem->driver_info_addr = (int32_t)driver_info;
	if(core_mem->driver_info_addr == 0)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	core_mem->driver_info_total_size = rsx_core_memory_get_mem_reg_size_by_id(core_mem, 11);
	if(core_mem->driver_info_total_size < 0x9000)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// reports
	core_mem->reports_size_total = rsx_core_memory_get_mem_reg_size_by_id(core_mem, 10);
	core_mem->reports_addr = rsx_core_memory_get_mem_reg_addr_by_id(core_mem, 10);
	core_mem->reports_size = 0x10000;
	
	
	// map local memory
	ret = rsx_map_io_to_lpar(core_mem->core_id,                 // (IN)  memory core ID
	                         core_mem->bar_1_addr,              // (IN)  IO address to map
	                         20,                                // (IN)  ?
	                         core_mem->bar_1_size - 0x200000,   // (IN)  IO size, - 2 MB system reserved
	                         &core_mem->ddr_lpar_addr);         // (OUT) mapped LPAR address 
	if(ret != 0)
	  return;
	
	// map driver info
	ret = rsx_map_io_to_lpar(core_mem->core_id,                 // (IN)  memory core ID
	                         core_mem->driver_info_addr,        // (IN)  IO address to map
	                         12,                                // (IN)  ?
	                         core_mem->driver_info_total_size,  // (IN)  IO size
	                         &core_mem->driver_info_lpar);      // (OUT) mapped LPAR address 
	if(ret != 0)
	  return;
	
	// map reports
	ret = rsx_map_io_to_lpar(core_mem->core_id,                 // (IN)  memory core ID
	                         core_mem->reports_addr,            // (IN)  IO address to map
	                         12,                                // (IN)  ?
	                         core_mem->reports_size_total,      // (IN)  IO size
	                         &core_mem->reports_lpar_addr);     // (OUT) mapped LPAR address 
	if(ret != 0)
	  return;
	
	core_mem->ddr_page_size = 0x100000;             // set page size
	core_mem->unk_048       = 0x400;                // ?
	
	
	// local memory
	// allocate bitmap, to handle the 254 local memory pages
	bm_local_mem = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_local_mem != NULL)
	{
		item_total = (core_mem->bar_1_size - core_mem->ddr_sys_reserv) / core_mem->ddr_page_size;
		
		rsx_utils_bitmap_create(bm_local_mem,              // (IN) bitmap object
		                        core_mem->ddr_lpar_addr,   // (IN) user value 1, DDR base LPAR address
		                        item_total,                // (IN) bitmap total item count, 245 pages of 1MB
		                        core_mem->ddr_page_size);  // (IN) user value 2, DDR page size (1 MB)
	  
	}
	
	// store bitmap into core memory object
	core_mem->bm_local_mem = (void*)bm_local_mem;
	
	// reports
	// allocate bitmap, to handle the 4 report areas
	bm_reports = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_reports != NULL)
	{
		item_total = core_mem->reports_size_total / core_mem->reports_size;
		
		rsx_utils_bitmap_create(bm_reports,                   // (IN) bitmap object
		                        core_mem->reports_lpar_addr,  // (IN) user value 1, reports base LPAR address
		                        item_total,                   // (IN) bitmap total item count, 4 report areas
		                        core_mem->reports_size);      // (IN) user value 2, size of one reports
		
	}
	
	// store bitmap into core memory object
	core_mem->bm_reports = (void*)bm_reports;
	
	// allocate bitmap, DDR_MB_CTags related ?, 512 items 
	core_mem->unk_048 = 0x400;
	
	bm_unk_00 = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_unk_00 != NULL)
	{
		item_total = core_mem->DDR_MB_CTags / core_mem->unk_048;
		
		rsx_utils_bitmap_create(bm_unk_00,                    // (IN) bitmap object
		                        0,                            // (IN) user value 1
		                        item_total,                   // (IN) bitmap total item count, 512 ?
		                        1);                           // (IN) user value 2
		
	}
	
	// store bitmap into core memory object
	core_mem->bm_unk_00 = (void*)bm_unk_00;
	
	// allocate bitmap, ?, 768 items
	core_mem->unk_0A8 = 0;
	core_mem->unk_0B0 = 0x300000;
	core_mem->unk_0B8 = 0x1000;
  
	bm_unk_01 = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_unk_01 != NULL)
	{
		item_total = core_mem->unk_0B0 / core_mem->unk_0B8;
		
		
		rsx_utils_bitmap_create(bm_unk_01,                    // (IN) bitmap object
		                        core_mem->unk_0A8,            // (IN) user value 1
		                        item_total,                   // (IN) bitmap total item count, 768 ?
		                        1);                           // (IN) user value 2
		
	}
	
	// store bitmap into core memory object
	core_mem->bm_unk_01 = (void*)bm_unk_01;
	
	// allocate bitmap, ?, 15 items
	bm_unk_02 = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_unk_02 != NULL)
	{
		rsx_utils_bitmap_create(bm_unk_02,                    // (IN) bitmap object
		                        core_mem->unk_030,            // (IN) user value 1
		                        core_mem->unk_034,            // (IN) bitmap total item count, 15 ?
		                        1);                           // (IN) user value 2
		
	}
	
	// store bitmap into core memory object
	core_mem->bm_unk_02 = (void*)bm_unk_02;
	
	// allocate bitmap, ?, 8 items
	bm_unk_03 = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_unk_03 != NULL)
	{
		rsx_utils_bitmap_create(bm_unk_03,                    // (IN) bitmap object
		                        core_mem->unk_038,            // (IN) user value 1
		                        core_mem->unk_03C,            // (IN) bitmap total item count, 8 ?
		                        1);                           // (IN) user value 2
		
	}
	
	// store bitmap into core memory object
	core_mem->bm_unk_03 = (void*)bm_unk_03;
	
	// check some bitmap objects again...
	if(core_mem->bm_local_mem == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	if(core_mem->bm_reports == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	if(core_mem->bm_unk_00 == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	if(core_mem->bm_unk_01 == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	if(core_mem->bm_unk_02 == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// context DMA
	// allocate bitmap, to handle the 256 context dma objects
	bm_ctx_dma = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_ctx_dma != NULL)
	{
		addr = rsx_core_memory_get_mem_reg_addr_by_id(core_mem, 7);
		size = rsx_object_context_dma_get_object_size();                          // 0x1000(4 KB)
		item_total = rsx_core_memory_get_mem_reg_size_by_id(core_mem, 7) / size;
		
		rsx_utils_bitmap_create(bm_ctx_dma,                // (IN) bitmap object
		                        addr,                      // (IN) user value 1, context DMA address, 0x28002040000
		                        item_total,                // (IN) bitmap total item count, 256 context dma objects
		                        size);                     // (IN) user value 2, size of a DMA object, 16 byte
	  
	}
	
	// store bitmap into core memory object
	core_mem->bm_ctx_dma = (void*)bm_ctx_dma;
	
	
	
	// class nv sw
	// allocate bitmap, to handle the 128 nv/sw class objects
	bm_class_nv_sw = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_class_nv_sw != NULL)
	{
		addr = rsx_core_memory_get_mem_reg_addr_by_id(core_mem, 8);
		size = rsx_object_nv_class_get_object_size();
		item_total = rsx_core_memory_get_mem_reg_size_by_id(core_mem, 8) / size;
		
		rsx_utils_bitmap_create(bm_class_nv_sw,            // (IN) bitmap object
		                        addr,                      // (IN) user value 1, address 0x28002050000
		                        item_total,                // (IN) bitmap total item count, 256 context dma objects
		                        size);                     // (IN) user value 2, size of a nv/sw class object, 32 byte
	  
	}
	
	// store bitmap into core memory object
	core_mem->bm_class_nv_sw = (void*)bm_class_nv_sw;
	
	// driver info
	// allocate bitmap, to handle the 3 driver info areas
	size = rsx_core_context_get_size_of_reports();
	core_mem->driver_info_size = size;
	
	bm_driver_info = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
	if(bm_driver_info != NULL)
	{
		item_total = core_mem->driver_info_total_size / core_mem->driver_info_size;
    
		rsx_utils_bitmap_create(bm_driver_info,                         // (IN) bitmap object
		                        (int32_t)core_mem->driver_info_addr,    // (IN) user value 1, LV1 driver info address
		                        item_total,                             // (IN) bitmap total item count, 3 driver info areas
		                        size);                                  // (IN) user value 2, size of one driver info area, 0x4000(16 KB)
	  
	}
	
	// store bitmap into core memory object
	core_mem->bm_driver_info = (void*)bm_driver_info;
	
	// init core memory rsx_mem_ctx array with 0
	for(i = 0; i < 16; i++)
	  core_mem->rsx_mem_ctx[i] = NULL;
	
  return;
}


/***********************************************************************
* 
***********************************************************************/
rsx_core_mem_obj_t *rsx_core_memory_ctor(int64_t rsx_core_id)
{
	int32_t DDR_MB_CTags;
	int64_t BAR1_addr;
	int32_t BAR1_size;
	int64_t BAR2_addr;
	rsx_dev_core_obj_t *core = NULL;
	rsx_bus_ioif0_obj_t *ioif0 = NULL;
	rsx_core_mem_obj_t *core_mem = NULL;
	
	
	// get device core
	core = rsx_core_device_get_core_object_by_id(rsx_core_id);
	if(core == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	
	// get IOIF0
	ioif0 = (void*)core->ioif0;
	if(ioif0 == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	
	// check: is there alraedy a global memory core object?
	if(g_core_mem_obj != NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	
	// get needed values
	DDR_MB_CTags = read_BAR0(0x28000100320);
	BAR1_addr    = rsx_bus_ioif0_get_BAR1_addr(ioif0);
	BAR1_size    = rsx_bus_ioif0_get_BAR1_size(ioif0);
	BAR2_addr    = rsx_bus_ioif0_get_BAR2_addr(ioif0);
	
	// allocate RSX core memory object
	core_mem = lv1_kmalloc(sizeof(rsx_core_mem_obj_t));
	if(core_mem == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	
	// set new core memory object
	rsx_core_memory_build_core_mem_obj(core_mem,         // memory core object
	                                   rsx_core_id,      // core ID
	                                   BAR1_addr,        // 0x28080000000
	                                   BAR1_size,        // 0x10000000(256 MB)
	                                   BAR2_addr,        // 0x28002000000
	                                   0,                // ?
	                                   0xF,              // ?
	                                   0,                // ?
	                                   8,                // ?
	                                   0,                // ?
                                     DDR_MB_CTags,     // 0x80000
                                     NULL);            // value to hold error status, ignored
	
	return core_mem;
}

