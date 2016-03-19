/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "context.h"

#include "lv1/lv1.h"
#include "lv1/lv1_misc.h"
#include "lv1/driver/rsx/assert.h"


S32 rsx_core_context_t::iomap(U64 ea, S64 lm_offset, S32 size, U64 flags) {
    // Check size alignment
    if ((size / page_size) != ((size / page_size) * page_size))
        return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // Check EA alignment
    if ((ea / page_size) != ((ea / page_size) * page_size))
        return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // Check lm_offset alignment
    if ((lm_offset / page_size) != ((lm_offset / page_size) * page_size))
        return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // Check IO size range
    if ((size + ea) > io_size)
        return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // Check flags
    if (flags == 0)
        flags = 0xE000000000000000;
    
    if (size == 0)
        return LV1_SUCCESS;
    
    // Map pages
    for (S32 done = 0; done <= size; done += page_size) {
        S32 ret = lv1_sub_2D7A14(0, ea + io_offset + done, lm_offset + done, 1, flags, 1);
        if (ret == 0) {
            return LV1_ILLEGAL_PARAMETER_VALUE;
        } 
    }
    return 0;
}

S64 rsx_core_context_t::get_dma_control_lpar_address() {
    RSX_ASSERT(ch_obj);
    return ch_obj->get_dma_control_lpar_address();
}

S64 rsx_core_context_t::get_driver_info_lpar_addr() {
    return driver_info_lpar;
}

U32 rsx_core_context_t::get_rsx_context_id() {
    return id;
}

S32 rsx_core_context_t::get_size_of_reports() {
    return 0x4000;  // 16 KB. Report size per context
}

rsx_object_context_dma_t* rsx_core_context_t::get_dma_object_by_index(S32 index) {
    RSX_ASSERT(index < 16);
    if (index < 8) {
        return dma_array_0[index];
    } else {
        return dma_array_1[index - 8];
    }
}

void rsx_core_context_t::sub213614() {
    // TODO: eic stuff
    S32 ret = rsx_device_eic_219C18(index + 7, &unk_020);
    if (ret != 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // ? 
    DRVI_write32(index, unk_020, 0x12B8);
    DRVI_write32(index, ret, 0x12BC);
    
    // ? RSX context display buffer flag[0]
    if (db_flag[0] == 0) {
        DRVI_write32(index, -1, 0x12C0);  // store -1
    } else {
        DRVI_write32(index,  0, 0x12C0);  // store  0
    }
    return;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_context_t::sub2146F4() {
    S32 i, ret = -1, value;
    U32 handle;
    rsx_core_device_t* core = NULL;
    rsx_core_memory_t* core_mem = NULL;
    rsx_utils_bitmap_t* bm_driver_info = NULL;
    rsx_bus_ioif0_t* ioif0 = NULL;
    rsx_memory_context_t* mem_ctx = NULL;
    rsx_object_channel_t* ch_obj = NULL;
    rsx_device_clock_t* clock = NULL;
    
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // get RSX core memory object
    core_mem = (void*)core->core_mem;
    
    // allocate the driver info for our new RSX context
    bm_driver_info = (void*)core_mem->bm_driver_info;
    ret = rsx_utils_bitmap_allocate(bm_driver_info, 1, &driver_info_addr);
    if (ret == 0)  // allocation fails
      return 0;
    
    // compute and set RSX context driver info LPAR address
    driver_info_lpar = core_mem->driver_info_lpar + driver_info_addr - core_mem->driver_info_addr;
    
    // ? copy driver_info_addr
    driver_info_addr_0 = driver_info_addr;
    
    // memset usable space to 0 
    for (i = 0; i < 2792; i++)
      DRVI_write32(index, 0, i * 4);
    
    // store driver version(0x211) into driver info 0x0000:
    DRVI_write32(index, 0x211, 0);
    
    // get IOIF0 object from device core
    ioif0 = (void*)core->ioif0;
    
    // store chip revision into driver info 0x0004:
    DRVI_write32(index, (S32)rsx_bus_ioif0_get_chip_revision((void*)ioif0), 4);
    
    // store context local memory size into driver info 0x0008:
    mem_ctx = (void*)mem_ctx;
    DRVI_write32(index, mem_ctx->lm_end_lpar - mem_ctx->lm_start_lpar, 8);
    
    
    
    // TODO: channel object not finished yet
    // store context channel ID into driver info 0x000C:
    ch_obj = (void*)ch_obj;
    DRVI_write32(index, (S32)rsx_object_channel_get_id((void*)ch_obj), 0xC);
    
    
    
    // store NV clock frequency(0x1DCD6500(500 MHz)) into driver info 0x0010:
    clock = (void*)core->dev_clock1;
    DRVI_write32(index, (S32)rsx_device_clock_rsx_get_frequency((void*)clock), 0x10);
    
    // store display clock frequency(0x26BE3680(650 MHz)) into driver info 0x0014:
    clock = (void*)core->dev_clock5;
    DRVI_write32(index, (S32)rsx_device_clock_rsx_get_frequency((void*)clock), 0x14);
    
    
    // ? memory related, TODO: something wrong with stored values!
    DRVI_write32(index, (S32)count_set_bits(mem_ctx->unk_40), 0x18);
    DRVI_write32(index, (S32)count_set_bits(mem_ctx->unk_48), 0x1C);
    DRVI_write32(index, (S32)(mem_ctx->unk_2C - mem_ctx->unk_28), 0x20);
    DRVI_write32(index, (S32)(mem_ctx->unk_38 - mem_ctx->unk_30), 0x24);
    
    
    // store count of GRAPH units(8) into driver info 0x0028:
    value = rsx_rd32(0x00001540);        // NV40_PMC_GRAPH_UNITS           
    DRVI_write08(index, (S8)count_set_bits((value & 0x1FF00) >>8), 0x28);  // value[15:23], GRAPH units
    DRVI_write08(index, (S8)count_set_bits(value & 0xFF) * 4, 0x29);       // value[24:31], ? 6 of 8 are set, but no idea which one
    
    // ?
    value = rsx_rd32(0x00100200);
    value &= 3;                        // value[30:31]
    DRVI_write08(index, (S8)(value + 1), 0x2A); // 2
    
    // ? reports offsets
    DRVI_write32(index, (S32)(unk_0E0 - reports_addr), 0x2C);  // ctx 0: offset 0x1000
    DRVI_write32(index, (S32)(unk_0E8 - reports_addr), 0x30);  // ctx 0: offset 0
    DRVI_write32(index, (S32)(unk_0F8 - reports_addr), 0x34);  // ctx 0: offset 0x1400
    
    // store system mode flags into driver info 0x0054:
    DRVI_write32(index, (S32)(system_mode), 0x54);
    
    
    // DMA objects: 0x66604200 to 0x6660420F
    for (i = 0; i < 8; i++) {
        DRVI_write32(index, dma_array_0[i]->get_object_handle(), 0x858 + 0x00 + (i * 4));
    }
    for (i = 0; i < 8; i++) {
        DRVI_write32(index, dma_array_1[i]->get_object_handle(), 0x858 + 0x20 + (i * 4));
    }
    DRVI_write32(index, dma_0A0->get_object_handle(), 0x58);  // 0xFEED0000
    DRVI_write32(index, dma_0A8->get_object_handle(), 0x5C);  // 0xFEED0001
    DRVI_write32(index, dma_180->get_object_handle(), 0x64);  // 0x66606660
    DRVI_write32(index, dma_188->get_object_handle(), 0x68);  // 0x66616661
    DRVI_write32(index, dma_1B0->get_object_handle(), 0x70);  // 0x56616660
    DRVI_write32(index, dma_1B8->get_object_handle(), 0x74);  // 0x56616661
    DRVI_write32(index, dma_190->get_object_handle(), 0x6C);  // 0x66626660
    
    if (dma_198) {
        DRVI_write32(index, dma_198->get_object_handle(); 0x78); // 0xBAD68000
    }
    
    // NV objects
    for (i = 0; i < 8; i++) {
        DRVI_write32(index, nv_obj[i]->get_object_handle(), 0x458 + (i * 4));
    }
    // SW object
    DRVI_write32(index, sw_obj->get_object_handle(), 0x478);
    
    // ?  
    DRVI_write32(index, (S32)(unk_020), 0x12B8);  // ctx 0(0x38), ctx 1(0x39)
    
    // store context IO offset into driver info 0x2B60:
    DRVI_write32(index, (S32)io_offset, 0x2B60);
    
    // ? IO start address?
    value = core_mem->bar1_addr + (mem_ctx->lm_start_lpar - core_mem->ddr_lpar_addr);
    DRVI_write32(index, (S32)(value - ioif0->get_bar1_addr()), 0x2B5C);
    
    // BAR0
    value = rsx_rd32(0x00001540);
    value = (value & 0x1FF00) >>8;          // value[15:23], unk
    DRVI_write32(index, (S32)value, 0x2B64);
    
    value = rsx_rd32(0x00001540);
    value &= 0xFF;                          // value[24:31], unk
    DRVI_write32(index, (S8)value, 0x2B66);
    
    return 1;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_core_context_t::sub2136CC() {
    rsx_object_channel_t* ch_obj = NULL;
    rsx_object_sw_class_t* sw_obj = NULL;
    
    rsx_object_context_dma_t* dma_obj = nullptr;
    if (system_mode & L1GPU_SYSTEM_MODE_LOCAL_PB) {
        dma_obj = dma_0A0;  // DMA class object: 0xFEED0000
    } else {
        dma_obj = dma_0A8;  // DMA class object: 0xFEED0001
    }
    RSX_ASSERT(dma_obj);
    dma_098 = dma_obj;
    
    
    
    
    // create channel object
    // TODO: not finished
    ch_obj = (void*)rsx_object_channel_create_object((void*)dma_obj);
    if (ch_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    ch_obj = (void*)ch_obj;
    
    
    
    
    //////////////////////////////////////////////////////////////////////
    // create the hash table entries for the class objects: dma, hv and sw
    ch_obj->create_dma_object_hash_table_entry(dma_0A0);  // 0xFEED0000
    ch_obj->create_dma_object_hash_table_entry(dma_0A8);  // 0xFEED0001
    
    if (system_mode & L1GPU_SYSTEM_MODE_GSEMU) {
        ch_obj->create_dma_object_hash_table_entry(dma_0B8);  // 0xFEED0003
        ch_obj->create_dma_object_hash_table_entry(dma_0C0);  // 0xFEED0004
    }
    if (system_mode & L1GPU_SYSTEM_MODE_SYSTEM_SEMA) {
        ch_obj->create_dma_object_hash_table_entry(dma_1A8);  // 0x13378080
        ch_obj->create_dma_object_hash_table_entry(dma_1A0);  // 0x13378086
    }
    
    // If system mode flag [25:25](?) is set
    if (system_mode & 0x40) {
        ch_obj->create_dma_object_hash_table_entry(dma_1A0);  // 0x13378086
    }
    // If system mode flag [26:26](?) is set
    if (system_mode & 0x20) {
        ch_obj->create_dma_object_hash_table_entry(dma_198);  // 0xBAD68000
    }
    
    ch_obj->create_dma_object_hash_table_entry(dma_180);   // 0x66606660
    ch_obj->create_dma_object_hash_table_entry(dma_188);   // 0x66616661
    ch_obj->create_dma_object_hash_table_entry(dma_1B8);   // 0x56616661
    ch_obj->create_dma_object_hash_table_entry(dma_1B0);   // 0x56616660
    ch_obj->create_dma_object_hash_table_entry(dma_190);   // 0x66626660

    ch_obj->create_nv_object_hash_table_entry(nv_obj[0]);  // 0x00000000
    ch_obj->create_nv_object_hash_table_entry(nv_obj[1]);  // 0x31337000
    ch_obj->create_nv_object_hash_table_entry(nv_obj[2]);  // 0x31337303
    ch_obj->create_nv_object_hash_table_entry(nv_obj[3]);  // 0x3137C0DE
    ch_obj->create_nv_object_hash_table_entry(nv_obj[4]);  // 0x31337A73
    ch_obj->create_nv_object_hash_table_entry(nv_obj[5]);  // 0x313371C3
    ch_obj->create_nv_object_hash_table_entry(nv_obj[6]);  // 0x3137AF00
    ch_obj->create_nv_object_hash_table_entry(nv_obj[7]);  // 0x31337808
  
    // TODO: sw object not complete yet
    ch_obj->create_sw_object_hash_table_entry(sw_obj);     // 0xCAFEBABE
  
    // The 16 remaining DMA objects: 0x66604200 to 0x6660420F
    for (S32 i = 0; i < 16; i++) {
        dma_obj = get_dma_object_by_index(i);
        ch_obj->create_dma_object_hash_table_entry(dma_obj);    
    }
}

/***********************************************************************
* 
***********************************************************************/
void rsx_core_context_t::sub212F78() {
    S64 addr;
    S32 i, size;
    rsx_object_context_dma_t* dma_obj = NULL;
    rsx_memory_context_t* mem_ctx = NULL;
    rsx_core_memory_t* core_mem = NULL;
    
  
    //////////////////////////////////////////////////////////////////////
    // create and store dma object, 0xFEED0000
    dma_obj = rsx_object_context_dma_create_obj(0xFEED0000);
    if (dma_obj == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    dma_0A0 = (void*)dma_obj;
    
    // get memory context
    mem_ctx = (void*)mem_ctx;
    
    // get RSX core memory object
    core_mem = (void*)mem_ctx->core_mem;
    
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
    dma_0A8 = (void*)dma_obj;
    
    // size = IO size, 256 MB
    size = (S32)io_size;
    
    // address = IO offset, e.g. ctx 0(0x80000000), ctx 1(0x90000000)
    addr = io_offset;
    
    rsx_object_context_dma_220064(dma_obj, 3, 8, addr, size);
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [29:29]("flag gsemu ctx") is set, create dma objects 0xFEED0003 and 0xFEED0004 too
    // ? GS PS2 GPU ?
    if (system_mode & 4) {
        // create and store dma object, 0xFEED0003
        dma_obj = rsx_object_context_dma_create_obj(0xFEED0003);
        if (dma_obj == NULL) {
            printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
            return;
        }
        dma_0B8 = (void*)dma_obj;
        
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
        dma_0C0 = (void*)dma_obj;
        
        // size =  0x1000 (4 KB)
        size = 0x1000;
            
        // address = local memory start LPAR of our RSX memory context + BAR1(0x28080000000) + 0x4000000
        addr = (mem_ctx->lm_start_lpar - core_mem->ddr_lpar_addr) + core_mem->bar_1_addr + 0x4000000;
        
        rsx_object_context_dma_220064(dma_obj, 3, 4, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [28:28]("flag system semaphore") is set, ...
    if (system_mode & 8) {
        // create and store dma object, 0x13378086
        dma_obj = rsx_object_context_dma_create_obj(0x13378086);
        if (dma_obj == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
      return;
    }
      dma_1A0 = (void*)dma_obj;
        
        // size =  0x1000 (4 KB)
      size = 0x1000;
        
        // address = e.g. ctx 0: 0x8F03F000, ctx 1:0x9F03F000
      addr = (io_offset + 0xF000000) | 0x3F000;
      
        rsx_object_context_dma_220064(dma_obj, 3, 8, addr, size);
        
        // create and store dma object, 0x13378080
        dma_obj = rsx_object_context_dma_create_obj(0x13378080);
        dma_1A8 = (void*)dma_obj;
        
        rsx_object_context_dma_220064(dma_obj, 1, 8, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [25:25](?) is set, ...
    if (system_mode & 0x40) {
      // create and store dma object, 0x13378086
      dma_obj = rsx_object_context_dma_create_obj(0x13378086);
      dma_1A0 = (void*)dma_obj;
      
      // size =  0x1000 (4 KB)
      size = 0x1000;
      
      // address = e.g. ctx 0: 0x8F000000, ctx 1:0x9F000000
      addr = io_offset + 0xF000000;
      
      rsx_object_context_dma_220064(dma_obj, 3, 8, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // if system mode flag [26:26](?) is set, ...
    if (system_mode & 0x20) {
        // create and store dma object, 0xBAD68000
      dma_obj = rsx_object_context_dma_create_obj(0xBAD68000);
      dma_198 = (void*)dma_obj;
        
        // size =  0x1000000 (16 MB)
      size = 0x1000000;
        
        // address = e.g. ctx 0: 0x8E000000, ctx 1:0x9E000000
      addr = io_offset + 0xE000000;
        
        rsx_object_context_dma_220064(dma_obj, 2, 8, addr, size);
    }
    //////////////////////////////////////////////////////////////////////
    
    
    // 8 more dma objects...
    for (i = 0; i < 8; i++) {
        dma_obj = rsx_object_context_dma_create_obj(0x6660420F - i);
        rsx_object_context_dma_220064(dma_obj, 2, 8, io_offset + 0xF100000 + (i * 0x40), 0x40);
        dma_array_1[7 - i] = (void*)dma_obj;
    }
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_context_t::sub214C58() {
    S32 ret = -1, sh = 0;
    S64 channel_id;
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    
    
    io_size = 0x10000000;              // set IO memory size (256 MB)
    
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
    
    
    for (sh = 63; sh > -1; sh--)
        if (page_size & ((U64)1<<sh))
          break;
    
    // if system_mode[21:21](?) not set
    if (!(system_mode & 0x400)) {
        ret = allocate_io_segment(0,                                // (IN)  HSPRG0...
                                  (channel_id <<28) + 0x80000000,   // (IN)  
                                  io_size,                 // (IN)  
                                  0,                                // (IN)  ?
                                  io_size,                 // (IN) 
                                  sh,                               // (IN)  
                                  1,                                // (IN)  ?
                                  &io_offset);             // (OUT) 
        
        if (ret == 0) {
            unk_038 = ret;
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
                                &io_offset);             // (OUT) 
    
    if (ret == 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
  }
    
    // 
    ret = allocate_io_segment(0,                                // (IN)  HSPRG0...
                                0x10000000,                       // (IN)  
                                io_size,                 // (IN)  
                                0,                                // (IN)  ?
                                0x10000000,                       // (IN) 
                                sh,                               // (IN)  
                                1,                                // (IN)  ?
                                &unk_038);               // (OUT) 
    
    if (ret == 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
  }
    //////////////////////////////////////////////////////////////////////
    
    
    if (io_offset)
      return 1;  // OK
    
    if (unk_038 != 0x10000000) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
  }
    
    return 1;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_core_context_t::sub214040() {
    rsx_object_nv_class_t* nv_obj = NULL;
    rsx_object_sw_class_t* sw_obj = NULL;
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x00000030, 0x00000000, 0x28002050000)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    rsx_object_nv_class_create_object((void*)nv_obj, 0x30, 0);
    nv_obj[0] = (void*)nv_obj;
    
    
    // allocate and create nv object, ctx 0(0x00004097, 0x31337000, 0x28002050020)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    rsx_object_nv_class_create_object((void*)nv_obj, 0x4097, 0x31337000);
    nv_obj[1] = (void*)nv_obj;
    nv_obj = NULL;
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x00000039, 0x31337303, 0x28002050040)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x39, 0x31337303);
        nv_obj[2] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // allocate and create nv object, ctx 0(0x00000039, 0x3137C0DE, 0x28002050060)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x39, 0x3137C0DE);
        nv_obj[3] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // check only the first one
    if (nv_obj[2] == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x0000309E, 0x31337A73, 0x28002050080)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x309E, 0x31337A73);
        nv_obj[4] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // allocate and create nv object, ctx 0(0x00003062, 0x313371C3, 0x280020500A0)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x3062, 0x313371C3);
        nv_obj[5] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // check only the first one
    if (nv_obj[4] == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    //////////////////////////////////////////////////////////////////////
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate and create nv object, ctx 0(0x00003089, 0x3137AF00, 0x280020500C0)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x3089, 0x3137AF00);
        nv_obj[6] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // allocate and create nv object, ctx 0(0x0000308A, 0x31337808, 0x280020500E0)
    nv_obj = lv1_kmalloc(sizeof(rsx_object_nv_class_t));
    if (nv_obj) {
        rsx_object_nv_class_create_object((void*)nv_obj, 0x308A, 0x31337808);
        nv_obj[7] = (void*)nv_obj;
        nv_obj = NULL;
    }
    
    // check only the first one
    if (nv_obj[6] == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    //////////////////////////////////////////////////////////////////////
    
    
    // allocate and create the sw class object
    sw_obj = lv1_kmalloc(sizeof(rsx_object_sw_class_t));
    if (sw_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // the 3. param is the OPD of rsx_object_sw_driver()
    // TODO:
    rsx_object_sw_class_create_object((void*)sw_obj, 0xCAFEBABE, NULL);
    
    // store sw class object into context
    sw_obj = (void*)sw_obj;
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_context_t::sub2143E0() {
    S32 i, k, ret = -1;
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_reports = NULL;
    rsx_memory_context_t* mem_ctx = NULL;
    rsx_core_memory_t* core_mem = NULL;
    rsx_object_context_dma_t* dma_obj = NULL;
    
    
    // get RSX core memory object bitmap-reports
    mem_ctx = (void*)mem_ctx;
    core_mem = (void*)mem_ctx->core_mem;
    bm_reports = (void*)core_mem->bm_reports;
    
    // allocate a reports area for our new RSX context
    ret = rsx_utils_bitmap_allocate((void*)bm_reports, 1, &reports_lpar_addr);
    if (ret == 0)  // allocation fails
      return LV1_RESOURCE_SHORTAGE;
    
    
    // compute and store BAR1 reports start address
    reports_addr = reports_lpar_addr - core_mem->reports_lpar_addr + core_mem->reports_addr;
    
    // get and store reports size
    reports_size = bm_reports->value2;
    
    // ? reports:labels/channel, ctx 0(0x2808FE01000)
    unk_0E0 = reports_addr + 0x1000;
    
    // ? reports:reports, ctx 0(0x2808FE01400)
    unk_0F8 = reports_addr + 0x1400;
    
    // ? copy
    unk_0E8 = reports_addr;
    
    
    // get RSX device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // get memory region 12 start address, 0x2808FF10000
    unk_0F0 = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem, 12);
    
    
    
    // ?
    unk_1C8 = unk_0E0;
    
    for (i = 0; i < 8; i++) {
        for (k = 0; k < 4; k++) {
            DDR_write64(-1, unk_1C8 + ((k + (i * 4)) * 0x10));
            DDR_write32( 0, unk_1C8 + ((k + (i * 4)) * 0x10) + 8);
            DDR_write16( 0, unk_1C8 + ((k + (i * 4)) * 0x10) + 0xC);
            DDR_write16( 0, unk_1C8 + ((k + (i * 4)) * 0x10) + 0xE);
        }
        
        dma_obj = rsx_object_context_dma_create_obj(0x66604200 + i);
        dma_array_0[i] = (void*)dma_obj;
        
        // ? register/bind/init ? unk_1C8: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
        rsx_object_context_dma_220064(dma_obj, 2, 4, (i * 0x40) + unk_1C8, 0x40);
    }
    
    
    
    // address alignment check
    if ((unk_0E8 & 0xFFF) != 0) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    
    
    // ? 
  unk_1D0 = unk_0E8;
    
    for (i = 0; i < 256; i++) {
        DDR_write32(0x1337C0D3, (i * 0x10) + unk_1D0);
        DDR_write32(0x1337BABE, (i * 0x10) + unk_1D0 + 4);
        DDR_write32(0x1337BEEF, (i * 0x10) + unk_1D0 + 8);
        DDR_write32(0x1337F001, (i * 0x10) + unk_1D0 + 0xC);
    }
    
    
    
    // ?
    dma_obj = rsx_object_context_dma_create_obj(0x66606660);
    rsx_object_context_dma_220064(dma_obj, 3, 4, unk_0E8, 0x1000);
    dma_180 = (void*)dma_obj;
    
    dma_obj = rsx_object_context_dma_create_obj(0x66616661);
    rsx_object_context_dma_220064(dma_obj, 1, 4, unk_0E8, 0x1000);
    dma_188 = (void*)dma_obj;
    
    dma_obj = rsx_object_context_dma_create_obj(0x56616660);
    rsx_object_context_dma_220064(dma_obj, 3, 4, unk_0F0, 0x1000);
    dma_1B0 = (void*)dma_obj;
    
    dma_obj = rsx_object_context_dma_create_obj(0x56616661);
    rsx_object_context_dma_220064(dma_obj, 1, 4, unk_0F0, 0x1000);
    dma_1B8 = (void*)dma_obj;
    
    
    
    // ?
    unk_1D8 = unk_0F8;
    
    for (i = 0; i < 2048; i++) {
        DDR_write64(-1, (i * 0x10) + unk_1D8);
        DDR_write32( 0, (i * 0x10) + unk_1D8 + 8);
        DDR_write32( 0, (i * 0x10) + unk_1D8 + 0xC);
    }
    
    
    
    // ?
    dma_obj = rsx_object_context_dma_create_obj(0x66626660);
    rsx_object_context_dma_220064(dma_obj, 3, 4, unk_0F8, 0x8000);
    dma_190 = (void*)dma_obj;
    
    
    
    return 0;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_core_context_t::init(U64 *out, S64 core_id, rsx_memory_context_t* mem_ctx, U64 system_mode, S32 idx) {
    S32 value;
    rsx_core_device_t* core = NULL;
    
    
    id = idx ^ 0x55555555;    // set RSX context ID
    core_id = core_id;        // set RSX device core ID
    index = idx;                // set RSX context index, 0, 1 or 2
    mem_ctx = (void*)mem_ctx; // set RSX memory context
    page_size = 0x100000;     // set default page size, 1 MB
    system_mode = system_mode;   // system mode flags
    db_flag[0] = 0;           // display buffer[0] flag
    dma_0B8 = 0;              // (0xFEED0003) init 0
    dma_0C0 = 0;              // (0xFEED0004) init 0
    dma_198 = 0;              // (0xBAD68000) init 0
    dma_1A0 = 0;              // (0x13378086) init 0
    dma_1A8 = 0;              // (0x13378080) init 0
    
    // if system_mode[30:30]("small io page size") set, page size = 64 KB
    if (system_mode & 2)
      page_size = 0x10000;
    
    // if system_mode[28:28]("flag system semaphore") set
    if (system_mode & 8) {
      system_mode &= 0xFFFFFFFFFFFFFFBF;    // unset system_mode[25:25](?)
      system_mode = system_mode;
    }  
    
    // if system_mode[24:24](?) set
    if (system_mode & 0x80) {
        rsx_core_device_get_core_object_by_id(g_rsx_core_id);
        if (core == NULL) {
            printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
            return;
        }
        
        // pause FIFO
        rsx_device_fifo_pause((void*)core->fifo);
        
        // ? do something in GRAPH
        rsx_device_graph_21D054((void*)core->dev_graph_obj);
        rsx_device_graph_21D224((void*)core->dev_graph_obj, 0);
        
        // continue FIFO
        rsx_device_fifo_continue((void*)core->fifo);
    }
    
    // if system_mode[22:22](?) set
    if (system_mode & 0x200) {
        rsx_core_device_get_core_object_by_id(g_rsx_core_id);
        if (core == NULL) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
      
      // pause FIFO
        rsx_device_fifo_pause((void*)core->fifo);
        
        // ? do something in GRAPH
        rsx_device_graph_21D224((void*)core->dev_graph_obj, 1);
        
        // continue FIFO
        rsx_device_fifo_continue((void*)core->fifo);
    }
    
    // if system_mode[23:23](?) set
    if (system_mode & 0x100) {
        rsx_core_device_get_core_object_by_id(g_rsx_core_id);
        if (core == NULL) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
        
        // pause FIFO
        rsx_device_fifo_pause((void*)core->fifo);
        
        // ? do something in GRAPH
        rsx_device_graph_21D054((void*)core->dev_graph_obj);
        
        // BAR0/GRAPH: set 0x28000405018[17:17]
        value = rsx_rd32(0x00405018);
        value |= 0x4000;
        rsx_wr32(0, 0x28000405018);
        
        // continue FIFO
        rsx_device_fifo_continue((void*)core->fifo);
    }
    
    // if system_mode[20:20](?) set
    if (system_mode & 0x800)
      db_flag[0] = 1;       // set display buffer[0] flag to 1
    
    // flag based settings end
    
    
    
    //  reports, labels, dma
    sub2143E0();
    
    //  nv/sw class 
    sub214040();
    
    // allocate IO segment
    sub214C58();
    
    // dma
    sub212F78();
    
    // channel, hash table(sw/hv/dma)
    // TODO: channel init not finished yet
    sub2136CC();
    
    // create/init driver info
    sub2146F4();
    
    // eic, driver info, display buffers
    // TODO: eic stuff
    sub213614();
    
    // Init RSX context displaybuffer objects
    for (S32 i = 0; i < 8; i++) {
        d_buf[i].unk_00 = 0;
        d_buf[i].dma_obj = 0;
    }
    
    // increase global RSX context count by 1
    g_rsx_ctx_count++;
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
rsx_core_context_t* rsx_core_context_allocate(S64 core_id, rsx_memory_context_t* mem_ctx, U64 system_mode) {
    S32 idx;
  U64 out = 0;
    rsx_memory_context_t* rsx_ctx = NULL;
    
    
    for (idx = 0; idx < 16; idx++) {
        if (g_rsx_ctx_tbl[idx] == NULL) {
            rsx_ctx = lv1_kmalloc(sizeof(rsx_core_context_t));
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
