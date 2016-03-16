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
* 
***********************************************************************/
S32 rsx_core_context_iomap(rsx_ctx_obj_t* rsx_ctx, uS64 ea, S64 lm_offset, S32 size, uS64 flags) {
    S32 ret = -1, page_size = rsx_ctx->page_size, done = 0;
    
    
    // check size alignment
    if ((size / page_size) != ((size / page_size) * page_size))
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // check ea alignment
    if ((ea / page_size) != ((ea / page_size) * page_size))
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // check lm_offset alignment
    if ((lm_offset / page_size) != ((lm_offset / page_size) * page_size))
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // check io size range
    if ((size + ea) > rsx_ctx->io_size)
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // check flags
    if (flags == 0)
      flags = 0xE000000000000000;
    
    if (size == 0)
      return LV1_SUCCESS;
    
    // map pages
    while(done <= size) {
        ret = lv1_sub_2D7A14(0, ea + rsx_ctx->io_offset + done, lm_offset + done, 1, flags, 1);
        if (ret == 0)
          return LV1_ILLEGAL_PARAMETER_VALUE;
          
        done += page_size;
    }
    
    return 0;
}








/***********************************************************************
* 
***********************************************************************/
S64 rsx_core_context_get_dma_control_lpar_address(rsx_ctx_obj_t* rsx_ctx) {
    rsx_channel_obj_t* ch_obj = NULL;
    
    
    ch_obj = (void*)rsx_ctx->ch_obj;
    if (ch_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
    }
    
    return rsx_object_channel_get_dma_controll_lpar_address((void*)ch_obj);
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_core_context_get_driver_info_lpar_addr(rsx_ctx_obj_t* rsx_ctx) {
    return rsx_ctx->driver_info_lpar;
}

/***********************************************************************
* 
***********************************************************************/
uS32 rsx_core_context_get_rsx_context_id(rsx_ctx_obj_t* rsx_ctx) {
    return rsx_ctx->id;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_context_get_size_of_reports() {
    return 0x4000;  // 16 KB, report size per context
}

/***********************************************************************
* 
***********************************************************************/
static rsx_ctx_dma_obj_t* rsx_core_context_get_dma_object_by_index(rsx_ctx_obj_t* rsx_ctx, S32 idx) {
    // is index valid? 0 to 15
    if (idx > 15) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return NULL;
  }
    
    if (idx < 8)
        return (void*)rsx_ctx->dma_array_0[idx];
    
    return (void*)rsx_ctx->dma_array_1[idx - 8];
    
}

/***********************************************************************
* 
***********************************************************************/
static void rsx_core_context_213614(rsx_ctx_obj_t* rsx_ctx) {
    S32 ret = -1;
    
    
    // TODO: eic stuff
    ret = rsx_device_eic_219C18(rsx_ctx->idx + 7, &rsx_ctx->unk_020);
    if (ret != 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // ? 
    DRVI_write32(rsx_ctx->idx, rsx_ctx->unk_020, 0x12B8);
    DRVI_write32(rsx_ctx->idx, ret, 0x12BC);
    
    // ? RSX context display buffer flag[0]
    if (rsx_ctx->db_flag[0] == 0)
      DRVI_write32(rsx_ctx->idx, -1, 0x12C0);  // store -1
    DRVI_write32(rsx_ctx->idx, 0, 0x12C0);     // store  0
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
static S32 rsx_core_context_2146F4(rsx_ctx_obj_t* rsx_ctx) {
    S32 i, ret = -1, value;
    uS32 class;
    rsx_dev_core_obj_t* core = NULL;
    rsx_core_mem_obj_t* core_mem = NULL;
    rsx_utils_bm_obj_t* bm_driver_info = NULL;
    rsx_bus_ioif0_obj_t* ioif0 = NULL;
    rsx_mem_ctx_obj_t* mem_ctx = NULL;
    rsx_channel_obj_t* ch_obj = NULL;
    rsx_dev_clock_obj_t* clock = NULL;
    rsx_ctx_dma_obj_t* dma_obj = NULL;
    rsx_nv_class_obj_t* nv_obj = NULL;
    rsx_sw_class_obj_t* sw_obj = NULL;
    
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
    }
    
    // get RSX core memory object
    core_mem = (void*)core->core_mem_obj;
    
    // allocate the driver info for our new RSX context
    bm_driver_info = (void*)core_mem->bm_driver_info;
    ret = rsx_utils_bitmap_allocate(bm_driver_info, 1, &rsx_ctx->driver_info_addr);
    if (ret == 0)  // allocation fails
      return 0;
    
    // compute and set RSX context driver info LPAR address
    rsx_ctx->driver_info_lpar = core_mem->driver_info_lpar + rsx_ctx->driver_info_addr - core_mem->driver_info_addr;
    
    // ? copy driver_info_addr
    rsx_ctx->driver_info_addr_0 = rsx_ctx->driver_info_addr;
    
    // memset usable space to 0 
    for(i = 0; i < 2792; i++)
      DRVI_write32(rsx_ctx->idx, 0, i * 4);
    
    // store driver version(0x211) into driver info 0x0000:
    DRVI_write32(rsx_ctx->idx, 0x211, 0);
    
    // get IOIF0 object from device core
    ioif0 = (void*)core->ioif0;
    
    // store chip revision into driver info 0x0004:
    DRVI_write32(rsx_ctx->idx, (S32)rsx_bus_ioif0_get_chip_revision((void*)ioif0), 4);
    
    // store context local memory size into driver info 0x0008:
    mem_ctx = (void*)rsx_ctx->mem_ctx;
    DRVI_write32(rsx_ctx->idx, mem_ctx->lm_end_lpar - mem_ctx->lm_start_lpar, 8);
    
    
    
    // TODO: channel object not finished yet
    // store context channel ID into driver info 0x000C:
    ch_obj = (void*)rsx_ctx->ch_obj;
    DRVI_write32(rsx_ctx->idx, (S32)rsx_object_channel_get_id((void*)ch_obj), 0xC);
    
    
    
    // store NV clock frequency(0x1DCD6500(500 MHz)) into driver info 0x0010:
    clock = (void*)core->dev_clock_1_obj;
    DRVI_write32(rsx_ctx->idx, (S32)rsx_device_clock_rsx_get_frequency((void*)clock), 0x10);
    
    // store display clock frequency(0x26BE3680(650 MHz)) into driver info 0x0014:
    clock = (void*)core->dev_clock_5_obj;
    DRVI_write32(rsx_ctx->idx, (S32)rsx_device_clock_rsx_get_frequency((void*)clock), 0x14);
    
    
    // ? memory related, TODO: something wrong with stored values!
    DRVI_write32(rsx_ctx->idx, (S32)count_set_bits(mem_ctx->unk_40), 0x18);
    DRVI_write32(rsx_ctx->idx, (S32)count_set_bits(mem_ctx->unk_48), 0x1C);
    DRVI_write32(rsx_ctx->idx, (S32)(mem_ctx->unk_2C - mem_ctx->unk_28), 0x20);
    DRVI_write32(rsx_ctx->idx, (S32)(mem_ctx->unk_38 - mem_ctx->unk_30), 0x24);
    
    
    // store count of GRAPH units(8) into driver info 0x0028:
    value = read_BAR0(0x28000001540);        // NV40_PMC_GRAPH_UNITS           
    DRVI_write08(rsx_ctx->idx, (S8)count_set_bits((value & 0x1FF00) >>8), 0x28);  // value[15:23], GRAPH units
    DRVI_write08(rsx_ctx->idx, (S8)count_set_bits(value & 0xFF) * 4, 0x29);       // value[24:31], ? 6 of 8 are set, but no idea which one
    
    // ?
    value = read_BAR0(0x28000100200);
    value &= 3;                        // value[30:31]
    DRVI_write08(rsx_ctx->idx, (S8)(value + 1), 0x2A); // 2
    
    // ? reports offsets
    DRVI_write32(rsx_ctx->idx, (S32)(rsx_ctx->unk_0E0 - rsx_ctx->reports_addr), 0x2C);  // ctx 0: offset 0x1000
    DRVI_write32(rsx_ctx->idx, (S32)(rsx_ctx->unk_0E8 - rsx_ctx->reports_addr), 0x30);  // ctx 0: offset 0
    DRVI_write32(rsx_ctx->idx, (S32)(rsx_ctx->unk_0F8 - rsx_ctx->reports_addr), 0x34);  // ctx 0: offset 0x1400
    
    // store system mode flags into driver info 0x0054:
    DRVI_write32(rsx_ctx->idx, (S32)(rsx_ctx->sys_mode), 0x54);
    
    
    // dma objects:
    // 0x66604200 to 0x6660420F
    for(i = 0; i < 8; i++) {
        dma_obj = (void*)rsx_ctx->dma_array_0[i];
        class = rsx_object_context_dma_get_object_class((void*)dma_obj);
        DRVI_write32(rsx_ctx->idx, class, 0x858 + (i * 4));
    }
    for(i = 0; i < 8; i++) {
        dma_obj = (void*)rsx_ctx->dma_array_1[i];
        class = rsx_object_context_dma_get_object_class((void*)dma_obj);
        DRVI_write32(rsx_ctx->idx, class, 0x858 + 0x20 + (i * 4));
    }
    
    dma_obj = (void*)rsx_ctx->dma_0A0;  // 0xFEED0000
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x58);
    
    dma_obj = (void*)rsx_ctx->dma_0A8;  // 0xFEED0001
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x5C);
    
    dma_obj = (void*)rsx_ctx->dma_180;  // 0x66606660
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x64);
    
    dma_obj = (void*)rsx_ctx->dma_188;  // 0x66616661
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x68);
    
    dma_obj = (void*)rsx_ctx->dma_1B0;  // 0x56616660
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x70);
    
    dma_obj = (void*)rsx_ctx->dma_1B8;  // 0x56616661
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x74);
    
    dma_obj = (void*)rsx_ctx->dma_190;  // 0x66626660
    class = rsx_object_context_dma_get_object_class((void*)dma_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x6C);
    
    dma_obj = NULL;
    dma_obj = (void*)rsx_ctx->dma_198;  // 0xBAD68000
    if (dma_obj) {
        class = rsx_object_context_dma_get_object_class((void*)dma_obj);
      DRVI_write32(rsx_ctx->idx, class, 0x78);
    }
    
    // nv objects:
    for(i = 0; i < 8; i++) {
      nv_obj = (void*)rsx_ctx->nv_obj[i];
      class = rsx_object_nv_class_get_object_class((void*)nv_obj);
      DRVI_write32(rsx_ctx->idx, class, 0x458 + (i * 4));
    }
    
    // sw object:
    sw_obj = (void*)rsx_ctx->sw_obj;
    class = rsx_object_sw_class_get_object_class((void*)sw_obj);
    DRVI_write32(rsx_ctx->idx, class, 0x478);
    
    // ?  
    DRVI_write32(rsx_ctx->idx, (S32)(rsx_ctx->unk_020), 0x12B8);  // ctx 0(0x38), ctx 1(0x39)
    
    // store context IO offset into driver info 0x2B60:
    DRVI_write32(rsx_ctx->idx, (S32)rsx_ctx->io_offset, 0x2B60);
    
    // ? IO start address?
    value = core_mem->bar_1_addr + (mem_ctx->lm_start_lpar - core_mem->ddr_lpar_addr);
    DRVI_write32(rsx_ctx->idx, (S32)(value - rsx_bus_ioif0_get_BAR1_addr((void*)ioif0)), 0x2B5C);
    
    // BAR0
    value = read_BAR0(0x28000001540);
    value = (value & 0x1FF00) >>8;          // value[15:23], unk
    DRVI_write32(rsx_ctx->idx, (S32)value, 0x2B64);
    
    value = read_BAR0(0x28000001540);
    value &= 0xFF;                          // value[24:31], unk
    DRVI_write32(rsx_ctx->idx, (S8)value, 0x2B66);
    
    return 1;
}

/***********************************************************************
* 
***********************************************************************/
static void rsx_core_context_2136CC(rsx_ctx_obj_t* rsx_ctx) {
    S32 i;
    rsx_channel_obj_t* ch_obj = NULL;
    rsx_ctx_dma_obj_t* dma_obj = NULL;
    rsx_nv_class_obj_t* nv_obj = NULL;
    rsx_sw_class_obj_t* sw_obj = NULL;
    
    
    // if system mode flag [27:27]("flag local pb") is set, get...
    if (rsx_ctx->sys_mode & 0x10)
      dma_obj = (void*)rsx_ctx->dma_0A0;      // dma class object, 0xFEED0000, else get...
    dma_obj = (void*)rsx_ctx->dma_0A8;        // dma class object, 0xFEED0001
    
    if (dma_obj == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    
    // store dma class object
    rsx_ctx->dma_098 = (void*)dma_obj;
    
    
    
    
    // create channel object
    // TODO: not finished
    ch_obj = (void*)rsx_object_channel_create_object((void*)dma_obj);
    if (ch_obj == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  rsx_ctx->ch_obj = (void*)ch_obj;
    
    
    
    
    //////////////////////////////////////////////////////////////////////
    // create the hash table entries for the class objects: dma, hv and sw
    dma_obj = (void*)rsx_ctx->dma_0A0;  // 0xFEED0000
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    
    dma_obj = (void*)rsx_ctx->dma_0A8;  // 0xFEED0001
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    
    // if system mode[29:29]("flag gsemu ctx") is set
    if (rsx_ctx->sys_mode & 4) {
        dma_obj = (void*)rsx_ctx->dma_0B8;  // 0xFEED0003
      rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
      
      dma_obj = (void*)rsx_ctx->dma_0C0;  // 0xFEED0004
      rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    }
    
    // if system mode flag [28:28]("flag system semaphore") is set
    if (rsx_ctx->sys_mode & 8) {
        dma_obj = (void*)rsx_ctx->dma_1A8;  // 0x13378080
      rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
      
      dma_obj = (void*)rsx_ctx->dma_1A0;  // 0x13378086
      rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    }
    
    // if system mode flag [25:25](?) is set
    if (rsx_ctx->sys_mode & 0x40) {
        dma_obj = (void*)rsx_ctx->dma_1A0;  // 0x13378086
      rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    }
    
    // if system mode flag [26:26](?) is set
    if (rsx_ctx->sys_mode & 0x20) {
        dma_obj = (void*)rsx_ctx->dma_198;  // 0xBAD68000
      rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    }
    
    dma_obj = (void*)rsx_ctx->dma_180;  // 0x66606660
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    
    dma_obj = (void*)rsx_ctx->dma_188;  // 0x66616661
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
    
    dma_obj = (void*)rsx_ctx->dma_1B8;  // 0x56616661
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
  
  dma_obj = (void*)rsx_ctx->dma_1B0;  // 0x56616660
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
  
  dma_obj = (void*)rsx_ctx->dma_190;  // 0x66626660
    rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);
  
  
  nv_obj = (void*)rsx_ctx->nv_obj[0];  // 0x00000000
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  nv_obj = (void*)rsx_ctx->nv_obj[1];  // 0x31337000
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  nv_obj = (void*)rsx_ctx->nv_obj[2];  // 0x31337303
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
    
    nv_obj = (void*)rsx_ctx->nv_obj[3];  // 0x3137C0DE
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  nv_obj = (void*)rsx_ctx->nv_obj[4];  // 0x31337A73
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  nv_obj = (void*)rsx_ctx->nv_obj[5];  // 0x313371C3
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  nv_obj = (void*)rsx_ctx->nv_obj[6];  // 0x3137AF00
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  nv_obj = (void*)rsx_ctx->nv_obj[7];  // 0x31337808
  rsx_object_channel_create_nv_object_hash_table_entry((void*)ch_obj, (void*)nv_obj);
  
  
  // TODO: sw object not complete yet
  sw_obj = (void*)rsx_ctx->sw_obj;  // 0xCAFEBABE
  rsx_object_channel_create_sw_object_hash_table_entry((void*)ch_obj, (void*)sw_obj);
  
  
  // the remaining 16 dma objects, 0x66604200 to 0x6660420F
  for(i = 0; i < 16; i++) {
        dma_obj = (void*)rsx_core_context_get_dma_object_by_index((void*)rsx_ctx, i);
        rsx_object_channel_create_dma_object_hash_table_entry((void*)ch_obj, (void*)dma_obj);    
    }
  
    return;
}

/***********************************************************************
* 
***********************************************************************/
static void rsx_core_context_212F78(rsx_ctx_obj_t* rsx_ctx) {
    S64 addr;
    S32 i, size;
    rsx_ctx_dma_obj_t* dma_obj = NULL;
    rsx_mem_ctx_obj_t* mem_ctx = NULL;
    rsx_core_mem_obj_t* core_mem = NULL;
    
  
    //////////////////////////////////////////////////////////////////////
    // create and store dma object, 0xFEED0000
    dma_obj = rsx_object_context_dma_create_obj(0xFEED0000);
    if (dma_obj == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    rsx_ctx->dma_0A0 = (void*)dma_obj;
    
    // get memory context
    mem_ctx = (void*)rsx_ctx->mem_ctx;
    
    // get RSX core memory object
    core_mem = (void*)mem_ctx->core_mem_obj;
    
    // size = local memory size of our RSX memory context
    size = mem_ctx->lm_end_lpar - mem_ctx->lm_start_lpar;
    
    // address = local memory start LPAR of our RSX memory context + BAR1(0x28080000000)
    addr = mem_ctx->lm_start_lpar - core_mem->ddr_lpar_addr + core_mem->bar_1_addr;
    
    rsx_object_context_dma_220064(dma_obj, 3, 4, addr, size);
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // create and store dma object, 0xFEED0001
    dma_obj = rsx_object_context_dma_create_obj(0xFEED0001);
    if (dma_obj == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    rsx_ctx->dma_0A8 = (void*)dma_obj;
    
    // size = IO size, 256 MB
    size = (S32)rsx_ctx->io_size;
    
    // address = IO offset, e.g. ctx 0(0x80000000), ctx 1(0x90000000)
    addr = rsx_ctx->io_offset;
    
    rsx_object_context_dma_220064(dma_obj, 3, 8, addr, size);
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [29:29]("flag gsemu ctx") is set, create dma objects 0xFEED0003 and 0xFEED0004 too
    // ? GS PS2 GPU ?
    if (rsx_ctx->sys_mode & 4) {
        // create and store dma object, 0xFEED0003
        dma_obj = rsx_object_context_dma_create_obj(0xFEED0003);
        if (dma_obj == NULL) {
            printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
            return;
        }
        rsx_ctx->dma_0B8 = (void*)dma_obj;
        
        // size =  0x1000 (4 KB)
        size = 0x1000;
        
        // address = local memory start LPAR of our RSX memory context + BAR1(0x28080000000) + 0x4000000
        addr = (mem_ctx->lm_start_lpar - core_mem->ddr_lpar_addr) + core_mem->bar_1_addr + 0x4000000;
        
        rsx_object_context_dma_220064(dma_obj, 1, 4, addr, size);
        
        // create and store dma object, 0xFEED0004
        dma_obj = rsx_object_context_dma_create_obj(0xFEED0004);
        if (dma_obj == NULL) {
            printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
            return;
        }
        rsx_ctx->dma_0C0 = (void*)dma_obj;
        
        // size =  0x1000 (4 KB)
        size = 0x1000;
            
        // address = local memory start LPAR of our RSX memory context + BAR1(0x28080000000) + 0x4000000
        addr = (mem_ctx->lm_start_lpar - core_mem->ddr_lpar_addr) + core_mem->bar_1_addr + 0x4000000;
        
        rsx_object_context_dma_220064(dma_obj, 3, 4, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [28:28]("flag system semaphore") is set, ...
    if (rsx_ctx->sys_mode & 8) {
        // create and store dma object, 0x13378086
        dma_obj = rsx_object_context_dma_create_obj(0x13378086);
        if (dma_obj == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
    }
      rsx_ctx->dma_1A0 = (void*)dma_obj;
        
        // size =  0x1000 (4 KB)
      size = 0x1000;
        
        // address = e.g. ctx 0: 0x8F03F000, ctx 1:0x9F03F000
      addr = (rsx_ctx->io_offset + 0xF000000) | 0x3F000;
      
        rsx_object_context_dma_220064(dma_obj, 3, 8, addr, size);
        
        // create and store dma object, 0x13378080
        dma_obj = rsx_object_context_dma_create_obj(0x13378080);
        rsx_ctx->dma_1A8 = (void*)dma_obj;
        
        rsx_object_context_dma_220064(dma_obj, 1, 8, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [25:25](?) is set, ...
    if (rsx_ctx->sys_mode & 0x40) {
      // create and store dma object, 0x13378086
      dma_obj = rsx_object_context_dma_create_obj(0x13378086);
      rsx_ctx->dma_1A0 = (void*)dma_obj;
      
      // size =  0x1000 (4 KB)
      size = 0x1000;
      
      // address = e.g. ctx 0: 0x8F000000, ctx 1:0x9F000000
      addr = rsx_ctx->io_offset + 0xF000000;
      
      rsx_object_context_dma_220064(dma_obj, 3, 8, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [26:26](?) is set, ...
    if (rsx_ctx->sys_mode & 0x20) {
        // create and store dma object, 0xBAD68000
      dma_obj = rsx_object_context_dma_create_obj(0xBAD68000);
      rsx_ctx->dma_198 = (void*)dma_obj;
        
        // size =  0x1000000 (16 MB)
      size = 0x1000000;
        
        // address = e.g. ctx 0: 0x8E000000, ctx 1:0x9E000000
      addr = rsx_ctx->io_offset + 0xE000000;
        
        rsx_object_context_dma_220064(dma_obj, 2, 8, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    // 8 more dma objects...
    for(i = 0; i < 8; i++) {
        dma_obj = rsx_object_context_dma_create_obj(0x6660420F - i);
        rsx_object_context_dma_220064(dma_obj, 2, 8, rsx_ctx->io_offset + 0xF100000 + (i * 0x40), 0x40);
        rsx_ctx->dma_array_1[7 - i] = (void*)dma_obj;
    }
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
static S32 rsx_core_context_214C58(rsx_ctx_obj_t* rsx_ctx) {
    S32 ret = -1, sh = 0;
    S64 channel_id;
    rsx_dev_core_obj_t* core = NULL;
    rsx_utils_bm_obj_t* bm_channels = NULL;
    
    
    rsx_ctx->io_size = 0x10000000;              // set IO memory size (256 MB)
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
    }
    
    // get device core memory object channels bitmap
    bm_channels = (void*)core->bm_obj_channels;
    
    // allocate and deallocate a new channel, we need the ID of the next one
    ret = rsx_utils_bitmap_allocate((void*)bm_channels, 1, &channel_id);
    if (ret == 0)   // allocation fails {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    rsx_utils_bitmap_dealloc((void*)bm_channels, channel_id, 1);
    
    
    // ? unk, HSPRG0, any hande/context, the first arg for allocate_io_segment()
    // addi      r11, r11, -0x67E8 # HSRPG0(-0x67E8)
    // ld        r9, 0(r11)
    // ld        r31, 0x50(r9)
    
    
    for(sh = 63; sh > -1; sh--)
        if (rsx_ctx->page_size & ((uS64)1<<sh))
          break;
    
    // if system_mode[21:21](?) not set
    if (!(rsx_ctx->sys_mode & 0x400)) {
        ret = allocate_io_segment(0,                                // (IN)  HSPRG0...
                                  (channel_id <<28) + 0x80000000,   // (IN)  
                                  rsx_ctx->io_size,                 // (IN)  
                                  0,                                // (IN)  ?
                                  rsx_ctx->io_size,                 // (IN) 
                                  sh,                               // (IN)  
                                  1,                                // (IN)  ?
                                  &rsx_ctx->io_offset);             // (OUT) 
        
        if (ret == 0) {
            rsx_ctx->unk_038 = ret;
            return 1;
        }
        
        return 0;
    }
    
    
    //////////////////////////////////////////////////////////////////////
    // else, system_mode[21:21](?) is set
    ret = allocate_io_segment(0,                                // (IN)  HSPRG0...
                                (channel_id <<29) + 0x80000000,   // (IN)  
                                0x20000000,                       // (IN)  
                                0,                                // (IN)  ?
                                0x10000000,                       // (IN) 
                                sh,                               // (IN)  
                                1,                                // (IN)  ?
                                &rsx_ctx->io_offset);             // (OUT) 
    
    if (ret == 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
  }
    
    // 
    ret = allocate_io_segment(0,                                // (IN)  HSPRG0...
                                0x10000000,                       // (IN)  
                                rsx_ctx->io_size,                 // (IN)  
                                0,                                // (IN)  ?
                                0x10000000,                       // (IN) 
                                sh,                               // (IN)  
                                1,                                // (IN)  ?
                                &rsx_ctx->unk_038);               // (OUT) 
    
    if (ret == 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
  }
    //////////////////////////////////////////////////////////////////////
    
    
    if (rsx_ctx->io_offset)
      return 1;  // OK
    
    if (rsx_ctx->unk_038 != 0x10000000) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
  }
    
    return 1;
}

/***********************************************************************
* 
***********************************************************************/
static void rsx_core_context_214040(rsx_ctx_obj_t* rsx_ctx) {
    rsx_nv_class_obj_t* nv_obj = NULL;
    rsx_sw_class_obj_t* sw_obj = NULL;
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x00000030, 0x00000000, 0x28002050000)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    rsx_object_nv_class_create_object((void*)nv_obj, 0x30, 0);
    rsx_ctx->nv_obj[0] = (void*)nv_obj;
    
    
    // allocate and create nv object, ctx 0(0x00004097, 0x31337000, 0x28002050020)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    rsx_object_nv_class_create_object((void*)nv_obj, 0x4097, 0x31337000);
    rsx_ctx->nv_obj[1] = (void*)nv_obj;
    nv_obj = NULL;
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x00000039, 0x31337303, 0x28002050040)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x39, 0x31337303);
        rsx_ctx->nv_obj[2] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // allocate and create nv object, ctx 0(0x00000039, 0x3137C0DE, 0x28002050060)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x39, 0x3137C0DE);
        rsx_ctx->nv_obj[3] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // check only the first one
    if (rsx_ctx->nv_obj[2] == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x0000309E, 0x31337A73, 0x28002050080)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x309E, 0x31337A73);
        rsx_ctx->nv_obj[4] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // allocate and create nv object, ctx 0(0x00003062, 0x313371C3, 0x280020500A0)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x3062, 0x313371C3);
        rsx_ctx->nv_obj[5] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // check only the first one
    if (rsx_ctx->nv_obj[4] == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x00003089, 0x3137AF00, 0x280020500C0)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x3089, 0x3137AF00);
        rsx_ctx->nv_obj[6] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // allocate and create nv object, ctx 0(0x0000308A, 0x31337808, 0x280020500E0)
    nv_obj = lv1_kmalloc(sizeof(rsx_nv_class_obj_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x308A, 0x31337808);
        rsx_ctx->nv_obj[7] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // check only the first one
    if (rsx_ctx->nv_obj[6] == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    //////////////////////////////////////////////////////////////////////
    
    
    // allocate and create the sw class object
    sw_obj = lv1_kmalloc(sizeof(rsx_sw_class_obj_t));
    if (sw_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // the 3. param is the OPD of rsx_object_sw_driver()
    // TODO:
    rsx_object_sw_class_create_object((void*)sw_obj, 0xCAFEBABE, NULL);
    
    // store sw class object into context
    rsx_ctx->sw_obj = (void*)sw_obj;
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
static S32 rsx_core_context_2143E0(rsx_ctx_obj_t* rsx_ctx) {
    S32 i, k, ret = -1;
    rsx_dev_core_obj_t* core = NULL;
    rsx_utils_bm_obj_t* bm_reports = NULL;
    rsx_mem_ctx_obj_t* mem_ctx = NULL;
    rsx_core_mem_obj_t* core_mem = NULL;
    rsx_ctx_dma_obj_t* dma_obj = NULL;
    
    
    // get RSX core memory object bitmap-reports
    mem_ctx = (void*)rsx_ctx->mem_ctx;
    core_mem = (void*)mem_ctx->core_mem_obj;
    bm_reports = (void*)core_mem->bm_reports;
    
    // allocate a reports area for our new RSX context
    ret = rsx_utils_bitmap_allocate((void*)bm_reports, 1, &rsx_ctx->reports_lpar_addr);
    if (ret == 0)  // allocation fails
      return LV1_RESOURCE_SHORTAGE;
    
    
    // compute and store BAR1 reports start address
    rsx_ctx->reports_addr = rsx_ctx->reports_lpar_addr - core_mem->reports_lpar_addr + core_mem->reports_addr;
    
    // get and store reports size
    rsx_ctx->reports_size = bm_reports->value2;
    
    // ? reports:labels/channel, ctx 0(0x2808FE01000)
    rsx_ctx->unk_0E0 = rsx_ctx->reports_addr + 0x1000;
    
    // ? reports:reports, ctx 0(0x2808FE01400)
    rsx_ctx->unk_0F8 = rsx_ctx->reports_addr + 0x1400;
    
    // ? copy
    rsx_ctx->unk_0E8 = rsx_ctx->reports_addr;
    
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // get memory region 12 start address, 0x2808FF10000
    rsx_ctx->unk_0F0 = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem_obj, 12);
    
    
    
    // ?
    rsx_ctx->unk_1C8 = rsx_ctx->unk_0E0;
    
    for(i = 0; i < 8; i++) {
        for(k = 0; k < 4; k++) {
            DDR_write64(-1, rsx_ctx->unk_1C8 + ((k + (i * 4)) * 0x10));
            DDR_write32( 0, rsx_ctx->unk_1C8 + ((k + (i * 4)) * 0x10) + 8);
            DDR_write16( 0, rsx_ctx->unk_1C8 + ((k + (i * 4)) * 0x10) + 0xC);
            DDR_write16( 0, rsx_ctx->unk_1C8 + ((k + (i * 4)) * 0x10) + 0xE);
        }
        
        dma_obj = rsx_object_context_dma_create_obj(0x66604200 + i);
        rsx_ctx->dma_array_0[i] = (void*)dma_obj;
        
        // ? register/bind/init ? rsx_ctx->unk_1C8: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
        rsx_object_context_dma_220064(dma_obj, 2, 4, (i * 0x40) + rsx_ctx->unk_1C8, 0x40);
    }
    
    
    
    // address alignment check
    if ((rsx_ctx->unk_0E8 & 0xFFF) != 0) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    
    
    // ? 
  rsx_ctx->unk_1D0 = rsx_ctx->unk_0E8;
    
    for(i = 0; i < 256; i++) {
        DDR_write32(0x1337C0D3, (i * 0x10) + rsx_ctx->unk_1D0);
        DDR_write32(0x1337BABE, (i * 0x10) + rsx_ctx->unk_1D0 + 4);
        DDR_write32(0x1337BEEF, (i * 0x10) + rsx_ctx->unk_1D0 + 8);
        DDR_write32(0x1337F001, (i * 0x10) + rsx_ctx->unk_1D0 + 0xC);
    }
    
    
    
    // ?
    dma_obj = rsx_object_context_dma_create_obj(0x66606660);
    rsx_object_context_dma_220064(dma_obj, 3, 4, rsx_ctx->unk_0E8, 0x1000);
    rsx_ctx->dma_180 = (void*)dma_obj;
    
    dma_obj = rsx_object_context_dma_create_obj(0x66616661);
    rsx_object_context_dma_220064(dma_obj, 1, 4, rsx_ctx->unk_0E8, 0x1000);
    rsx_ctx->dma_188 = (void*)dma_obj;
    
    dma_obj = rsx_object_context_dma_create_obj(0x56616660);
    rsx_object_context_dma_220064(dma_obj, 3, 4, rsx_ctx->unk_0F0, 0x1000);
    rsx_ctx->dma_1B0 = (void*)dma_obj;
    
    dma_obj = rsx_object_context_dma_create_obj(0x56616661);
    rsx_object_context_dma_220064(dma_obj, 1, 4, rsx_ctx->unk_0F0, 0x1000);
    rsx_ctx->dma_1B8 = (void*)dma_obj;
    
    
    
    // ?
    rsx_ctx->unk_1D8 = rsx_ctx->unk_0F8;
    
    for(i = 0; i < 2048; i++) {
        DDR_write64(-1, (i * 0x10) + rsx_ctx->unk_1D8);
        DDR_write32( 0, (i * 0x10) + rsx_ctx->unk_1D8 + 8);
        DDR_write32( 0, (i * 0x10) + rsx_ctx->unk_1D8 + 0xC);
    }
    
    
    
    // ?
    dma_obj = rsx_object_context_dma_create_obj(0x66626660);
    rsx_object_context_dma_220064(dma_obj, 3, 4, rsx_ctx->unk_0F8, 0x8000);
    rsx_ctx->dma_190 = (void*)dma_obj;
    
    
    
    return 0;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_core_context_init(rsx_ctx_obj_t* rsx_ctx, uS64 *out, S64 core_id, rsx_mem_ctx_obj_t* mem_ctx, uS64 system_mode, S32 idx) {
    S32 i, value;
    rsx_dev_core_obj_t* core = NULL;
    
    
    rsx_ctx->id = idx ^ 0x55555555;    // set RSX context ID
    rsx_ctx->core_id = core_id;        // set RSX device core ID
    rsx_ctx->idx = idx;                // set RSX context index, 0, 1 or 2
    rsx_ctx->mem_ctx = (void*)mem_ctx; // set RSX memory context
    rsx_ctx->page_size = 0x100000;     // set default page size, 1 MB
    rsx_ctx->sys_mode = system_mode;   // system mode flags
    rsx_ctx->db_flag[0] = 0;           // display buffer[0] flag
    rsx_ctx->dma_0B8 = 0;              // (0xFEED0003) init 0
    rsx_ctx->dma_0C0 = 0;              // (0xFEED0004) init 0
    rsx_ctx->dma_198 = 0;              // (0xBAD68000) init 0
    rsx_ctx->dma_1A0 = 0;              // (0x13378086) init 0
    rsx_ctx->dma_1A8 = 0;              // (0x13378080) init 0
    
    // if system_mode[30:30]("small io page size") set, page size = 64 KB
    if (system_mode & 2)
      rsx_ctx->page_size = 0x10000;
    
    // if system_mode[28:28]("flag system semaphore") set
    if (system_mode & 8) {
      system_mode &= 0xFFFFFFFFFFFFFFBF;    // unset system_mode[25:25](?)
      rsx_ctx->sys_mode = system_mode;
  }  
    
    // if system_mode[24:24](?) set
    if (system_mode & 0x80) {
        rsx_core_device_get_core_object_by_id(g_rsx_core_id);
        if (core == NULL) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
        
        // pause FIFO
        rsx_device_fifo_pause((void*)core->dev_fifo_obj);
        
        // ? do something in GRAPH
        rsx_device_graph_21D054((void*)core->dev_graph_obj);
        rsx_device_graph_21D224((void*)core->dev_graph_obj, 0);
        
        // continue FIFO
        rsx_device_fifo_continue((void*)core->dev_fifo_obj);
    }
    
    // if system_mode[22:22](?) set
    if (system_mode & 0x200) {
        rsx_core_device_get_core_object_by_id(g_rsx_core_id);
        if (core == NULL) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
      
      // pause FIFO
        rsx_device_fifo_pause((void*)core->dev_fifo_obj);
        
        // ? do something in GRAPH
        rsx_device_graph_21D224((void*)core->dev_graph_obj, 1);
        
        // continue FIFO
        rsx_device_fifo_continue((void*)core->dev_fifo_obj);
    }
    
    // if system_mode[23:23](?) set
    if (system_mode & 0x100) {
        rsx_core_device_get_core_object_by_id(g_rsx_core_id);
        if (core == NULL) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
        
        // pause FIFO
        rsx_device_fifo_pause((void*)core->dev_fifo_obj);
        
        // ? do something in GRAPH
        rsx_device_graph_21D054((void*)core->dev_graph_obj);
        
        // BAR0/GRAPH: set 0x28000405018[17:17]
        value = read_BAR0(0x28000405018);
        value |= 0x4000;
        write_BAR0(0, 0x28000405018);
        
        // continue FIFO
        rsx_device_fifo_continue((void*)core->dev_fifo_obj);
    }
    
    // if system_mode[20:20](?) set
    if (system_mode & 0x800)
      rsx_ctx->db_flag[0] = 1;       // set display buffer[0] flag to 1
    
    // flag based settings end
    
    
    
    //  reports, labels, dma
    rsx_core_context_2143E0((void*)rsx_ctx);
    
    //  nv/sw class 
    rsx_core_context_214040((void*)rsx_ctx);
    
    // allocate IO segment
    rsx_core_context_214C58((void*)rsx_ctx);
    
    // dma
    rsx_core_context_212F78((void*)rsx_ctx);
    
    // channel, hash table(sw/hv/dma)
    // TODO: channel init not finished yet
    rsx_core_context_2136CC((void*)rsx_ctx);
    
    // create/init driver info
    rsx_core_context_2146F4((void*)rsx_ctx);
    
    // eic, driver info, display buffers
    // TODO: eic stuff
    rsx_core_context_213614((void*)rsx_ctx);
    
    
    
    // init RSX context displaybuffer objects
    for(i = 0; i < 8; i++) {
        rsx_ctx->d_buf[i].unk_00 = 0;
        rsx_ctx->d_buf[i].dma_obj = 0;
    }
    
    // increase global RSX context count by 1
    g_rsx_ctx_count++;
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
rsx_ctx_obj_t* rsx_core_context_allocate(S64 core_id, rsx_mem_ctx_obj_t* mem_ctx, uS64 system_mode) {
    S32 idx;
  uS64 out = 0;
    rsx_mem_ctx_obj_t* rsx_ctx = NULL;
    
    
    for(idx = 0; idx < 16; idx++) {
        if (g_rsx_ctx_tbl[idx] == NULL) {
            rsx_ctx = lv1_kmalloc(sizeof(rsx_ctx_obj_t));
            if (rsx_ctx == NULL) {
            printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
            return NULL;
        }
            
            rsx_core_context_init((void*)rsx_ctx, &out, core_id, mem_ctx, system_mode, idx);
            
            // store new RSX context object global
            g_rsx_ctx_tbl[idx] = (void*)rsx_ctx;
            
            return (void*)rsx_ctx;  // OK
        }
    }
    
    return NULL;  // FAIL
}
