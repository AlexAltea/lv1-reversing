/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "fifo.h"

#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/mmio.h"
#include "lv1/driver/rsx/core/device.h"


inline void rsx_device_fifo_t::enable_interrupt(U32 bits) {
    U32 reg = rsx_rd32(RSX_PFIFO_INTR_EN_0) | bits;
    rsx_wr32(RSX_PFIFO_INTR_EN_0, reg);
}

inline void rsx_device_fifo_t::disable_interrupt(U32 bits) {
}

S64 rsx_device_fifo_t::get_channel_dma_control_lpar_address(S32 channel_id) {
    return (fifo->dma_control_lpar + (channel_id << 12));
}

S64 rsx_device_fifo_t::sub21B7DC(S32 index) {
    RSX_ASSERT(index < 4);
    return fifo->unk_50[index];
}

void rsx_device_fifo_t::sub21B668(S32 channel_id, S32 arg1) {
    S32 value;
    RSX_ASSERT(channel_id < 32);

    // TODO: ???

    return;
}

void rsx_device_fifo_t::sub21C234(S32 channel_id) {
    //S32 value;
    rsx_core_device_t* core = NULL;
    rsx_device_fifo_t* fifo = NULL;
    
    RSX_ASSERT(channel_id < 32);
    
    // Get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // Get FIFO object again and pause it
    fifo = core->fifo;
    fifo->pause();
    
    // TODO: ?
    fifo->sub21B668(channel_id, 1);
    
    // TODO: ???

    return;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_t::sub21CCC0(S32 channel_id, S32 offset) {
    S32 tmp, value, fc1_offset;
    S64 fc1_addr;
    rsx_core_device_t* core = NULL;
    rsx_core_memory_t* core_mem = NULL;

    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    tmp = rsx_core_memory_value_div_by_16((void*)core, offset);
    
    // read value from BAR0:0x28000003204
    value = rsx_rd32(0x28000003204);     // unknow value! not inited, here 0
    value &= 0x1FF;
    
    /*if (value == channel_id) {
      rsx_wr32(tmp, 0x280000032E0);
      return;
    }*/
    
    // get channel fc1 address
    fc1_addr = rsx_device_fifo_get_fc1_channel_address_by_id(fifo, channel_id);
    
    // get channel fc1 offset
    core_mem = (void*)core->core_mem;
    fc1_offset = rsx_core_memory_get_BAR2_offset_by_address(core_mem, fc1_addr);
    
    // 
    value = DDR_read32(g_rsx_bar2_addr + fc1_offset + 0x38);
    value &= 0xFF;
    value |= (tmp & 0xFFFFFFF);
    DDR_write32(value, g_rsx_bar2_addr + fc1_offset + 0x38);
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_device_fifo_t::get_fc2_channel_address_by_id(S32 channel_id) {
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    
    
    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // get device core memory object channels bitmap
    bm_channels = core->bm_obj_channels;
    if (bm_channels == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // check: is channel ID valid? not out of range(0 to 3)
    if ((rsx_utils_bitmap_get_item_total(bm_channels)) <= channel_id) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // 0x28002030000 + 0(0), 1(0x1000), 2(0x2000), 3(0x3000)
    return (fifo->fc2_addr + (channel_id <<12));
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_device_fifo_t::get_fc1_channel_address_by_id(S32 channel_id) {
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    
    
    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // get device core memory object channels bitmap
    bm_channels = (void*)core->bm_obj_channels;
    if (bm_channels == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // check: is channel ID valid? not out of range(0 to 3)
    if ((rsx_utils_bitmap_get_item_total(bm_channels)) <= channel_id) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    // 0x28002020000 + 0(0), 1(0x80), 2(0x100), 3(0x180)
    return (fifo->fc1_addr + (channel_id <<7));
}


S32 rsx_device_fifo_t::pause() {
    S32 i, value;
    
    
    fifo->pause++;
    
    value = rsx_rd32(RSX_PFIFO_CACHES);
    value &= 0xFFFFFFFE;                     // unset value[31:31]
    rsx_wr32(value, RSX_PFIFO_CACHES);
    
    value = rsx_rd32(RSX_PFIFO_CACHES);
    
    // if value[28:28] set, wait and try again
    if ((value &= 0xFFFFFFEF) != 0) {
    for (i = 0; i < 1000000; i++) {
            // db16cyc
      // db16cyc
      // db16cyc
      // db16cyc
      // eieio
            value = rsx_rd32(RSX_PFIFO_CACHES);
            
            // if value[28:28] unset, ok
            if ((value &= 0xFFFFFFEF) == 0)
              break;
        }
    }
    
    value = rsx_rd32(0x28000003200);
    
    // if value[32:32] unset...
    if ((value & 0xFFFFFFFE) == 0)
      return 0;
    
    value = rsx_rd32(0x28000003200);
    value &= 0xFFFFFFFE;                     // unset value[31:31]
    rsx_wr32(value, 0x28000003200);
    
    value = rsx_rd32(0x28000003220);
    value &= 0xFFFFFFFE;                     // unset value[31:31]
    rsx_wr32(value, 0x28000003220);
    
    
    /**/
    for (i = 0; i < 1000001; i++) {
        value = rsx_rd32(0x28000003250);
        if ((value & 0xFFFFEFFF) == 0)         // if value[19:19] unset... {
            // db16cyc                          // wait and try again
      // db16cyc
      // db16cyc
      // db16cyc
            continue;
        }
        
        value = rsx_rd32(0x28000003304);
        if ((value & 0xFFFFFFFE) == 0)         // if value[31:31] unset... {
            // db16cyc                          // wait and try again
      // db16cyc
      // db16cyc
      // db16cyc
            continue;
        }
        
        value = rsx_rd32(0x28000003250);
        value &= 0xFFFFFFFE;                  // unset value[31:31]
        rsx_wr32(value, 0x28000003250);
        
        value = rsx_rd32(0x28000003258);
        value &= 0xFEFFFFFF;                  // unset value[07:07]
        rsx_wr32(value, 0x28000003258);
        
        value = rsx_rd32(0x28000003250);
        if ((value & 0xFFFFFFEF) == 0)         // if value[27:27] unset... {
            
            
            
        }
        //TODO: fucking loops...
    }
    
    
    
    return 0;
}


void rsx_device_fifo_t::resume() {
    S32 value;
    
    
    if (fifo->pause == 0) {
        value = rsx_rd32(0x28000003250);
        value |= 1;                         // set value[31:31]
        rsx_wr32(value, 0x28000003250);
        
        value = rsx_rd32(0x28000003200);
        value |= 1;                         // set value[31:31]
        rsx_wr32(value, 0x28000003200);
        
        value = rsx_rd32(0x28000003220);
        value |= 1;                         // set value[31:31]
        rsx_wr32(value, 0x28000003220);
        
        if (((rsx_core_device_get_rsx_enable_flag()) != 0) && (fifo->init != 1))
          return;
        
        value = rsx_rd32(RSX_PFIFO_CACHES);
        value |= 1;                         // set value[31:31]
        rsx_wr32(value, RSX_PFIFO_CACHES);
    }
    
    fifo->pause--;
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_t::sub21B0B0() {
    S32 value;

    // update BAR0 registers
    value = rsx_rd32(RSX_PFIFO_UNK2230);
    value &= 0xFFFFFE00;                 // unset value[23:31]
    value |= 0xFF;                       // set value[24:31]
    rsx_wr32(RSX_PFIFO_UNK2230, value);
    
    // set 32bit values to 0
    rsx_wr32(RSX_PFIFO_CACHE1_PUT, 0);
    rsx_wr32(RSX_PFIFO_CACHE1_GET, 0);
    rsx_wr32(RSX_PFIFO_CACHE1_DMA_PUT, 0);
    rsx_wr32(RSX_PFIFO_CACHE1_DMA_GET, 0);
    rsx_wr32(RSX_PFIFO_CACHE1_HASH, 0);
    
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x00, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x04, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x08, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x0C, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x10, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x14, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x18, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x1C, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x20, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x24, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x28, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x2C, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x30, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x34, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x38, 0);
    rsx_wr32(RSX_PFIFO_UNK2510 + 0x3C, 0);

    // set flags
    value = rsx_rd32(0x2800000250C);
    value |= 1;                          // set [31:31]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 2;                          // set [30:30]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 4;                          // set [29:29]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 8;                          // set [28:28]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x10;                       // set [27:27]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x20;                       // set [26:26]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x40;                       // set [25:25]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x80;                       // set [24:24]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x100;                      // set [23:23]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x200;                      // set [22:22]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x400;                      // set [21:21]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x800;                      // set [20:20]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x1000;                     // set [19:19]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x2000;                     // set [18:18]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x4000;                     // set [17:17]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x2800000250C);
    value |= 0x8000;                     // set [16:16]
    rsx_wr32(value, 0x2800000250C);
    
    value = rsx_rd32(0x28000002048);
    value &= 0xFEFFFFFF;                 // unset [07:07]
    rsx_wr32(value, 0x28000002048);
    
    rsx_wr32(RSX_PFIFO_CACHE1_DMA_STATE, 0);
    rsx_wr32(RSX_PFIFO_UNK32E8, 0);
    rsx_wr32(RSX_PFIFO_UNK2410, 0);
    rsx_wr32(RSX_PFIFO_UNK2420, 0);
    
    value = rsx_rd32(0x2800000221C);
    value &= 0xFFFFFFFE;                 // unset [31:31]
    rsx_wr32(value, 0x2800000221C);
    
    rsx_wr32(1, 0x28000002058);        // set to 1
    
    value = rsx_rd32(RSX_PFIFO_CACHES);
    value &= 0xFFFFFFFE;                 // unset [31:31]
    rsx_wr32(value, RSX_PFIFO_CACHES);
    
    value = rsx_rd32(0x28000003250);
    value &= 0xFFFFFFFE;                 // unset [31:31]
    rsx_wr32(value, 0x28000003250);
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_t::21B5FC(S32 arg1, S32 arg2) {
    if ((arg1 - 1) > 2) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    fifo->unk_50[arg1] = arg2;
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_t::sub21BA08() {
    S32 value, fc1_offset, fc2_offset, run_out_offset, ctr = 0;
    rsx_core_device_t* core = NULL;
    
    
    // get device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // get fc1 offset, if offset is valid(not out of address range)
    fc1_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem, fifo->fc1_addr);
    
    // get fc2 offset, if offset is valid(not out of address range)
    fc2_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem, fifo->fc2_addr);
    
    // update BAR0 registers
    value = rsx_rd32(0x28000002230);
    value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
    value |= 1;                          // set value[31:31]
    rsx_wr32(value, 0x28000002230);
    
    // BAR0:0x28000002220 = NV40_PFIFO_RAMFC, RAM FIFO cash?
    // (the offsets for fc1 and fc2 are stored here, FIFO cash 1 and 2?)
    value = rsx_rd32(0x28000002220);
    value &= 0xFFFFE000;                           // unset value[19:31]
    value |= (fc1_offset & 0x1FFF0000) >> 16;
    rsx_wr32(value, 0x28000002220);
    
    value = rsx_rd32(0x28000002220);
    value &= 0xFF00FFFF;                           // unset value[08:15]
    value |= (fc2_offset & 0xFF0000);
    rsx_wr32(value, 0x28000002220);              // value now 0x????30002
    
    // get run_out address, 0x28002018000 and size, 0x200(512 byte)
    fifo->run_out_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem, 3);
    fifo->run_out_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem, 3);
    
    if (fifo->run_out_size <= 0x1FF) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    //////////////////////////////////////////////////////////////////////
    // update BAR0 registers
    value = rsx_rd32(0x28000002218);
    value &= 0xFFFEFFFF;                           // unset value[15:15]
    rsx_wr32(value, 0x28000002218);
    //////////////////////////////////////////////////////////////////////
    
    // get run_out offset, if offset is valid(not out of address range)
    run_out_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem, fifo->run_out_addr);
    
    //////////////////////////////////////////////////////////////////////
    // update BAR0 registers
    value = rsx_rd32(0x28000002218);
    value &= 0xFFFFFE01;                           // unset value[23:30]
    value |= (run_out_offset & 0x1FE00) >>9;
    rsx_wr32(value, 0x28000002218);
    //////////////////////////////////////////////////////////////////////
    
    // get run_out offset, if offset is valid(not out of address range)
    run_out_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem, fifo->run_out_addr);
    
    // init FIFO run_out memory, with 0xFADEFADE
    while (fifo->run_out_size > ctr) {
      DDR_write32(0xFADEFADE, (ctr + run_out_offset + g_rsx_bar2_addr));
      ctr+=4;
  }
    
    // ???
    fifo->sub21B5FC(1, 0x80);
    fifo->sub21B5FC(2, 0x80);
    fifo->sub21B5FC(3, 8);
    
    
    //////////////////////////////////////////////////////////////////////
    // update BAR0 registers
    value = 0;
    rsx_wr32(RSX_PFIFO_INTR_EN_0, value);
    
    enable_interrupt(RSX_PFIFO_INTR_CACHE_ERROR);
    enable_interrupt(RSX_PFIFO_INTR_CACHE_ERROR); // Again?
    enable_interrupt(RSX_PFIFO_INTR_RUNOUT);
    enable_interrupt(RSX_PFIFO_INTR_RUNOUT_OVERFLOW);
    enable_interrupt(RSX_PFIFO_INTR_DMA_PUSHER);
    enable_interrupt(RSX_PFIFO_INTR_DMA_PT);
    enable_interrupt(RSX_PFIFO_INTR_BIT18);
    enable_interrupt(RSX_PFIFO_INTR_BIT19);
    enable_interrupt(RSX_PFIFO_INTR_SEMAPHORE);
    enable_interrupt(RSX_PFIFO_INTR_BIT21);
    enable_interrupt(RSX_PFIFO_INTR_BIT22);
    enable_interrupt(RSX_PFIFO_INTR_ACQUIRE_TIMEOUT);

    fifo->sub21B0B0();     // init/update BAR0 registers
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_t::init() {
    S32 ret = -1, fc1_offset, fc2_offset, ctr = 0;
    rsx_core_device_t* core = NULL;
    
    
    // ?
    fifo->pause = 0;
    fifo->unk_2C = 0;
    
    // get device core object, we need IOIF0 next
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    // get fc1 address, 0x28002020000
    fifo->fc1_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem, 4);
    
    // get fc2 address, 0x28002030000
    fifo->fc2_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem, 5);
    
    // get fc1 size, 0x200(512 byte)
    fifo->fc1_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem, 4);
    
    // get fc2 size, 0x4000(16 KB)
    fifo->fc2_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem, 5);
    
    // get fc1 offset, if offset is valid(not out of address range)
    fc1_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem, fifo->fc1_addr);
    
    // get fc2 offset, if offset is valid(not out of address range)
    fc2_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem, fifo->fc2_addr);
    
    // init fc1 memory, with 0xDEADBEEF
    if (fifo->fc1_size != 0) {
      while (fifo->fc1_size > ctr) {
          DDR_write32(0xDEADBEEF, (ctr + fc1_offset + g_rsx_bar2_addr));
          ctr+=4;
      }
    }
    
    // init fc2 memory, with 0x12345678
    if (fifo->fc2_size != 0) {
        ctr = 0;
      while (fifo->fc2_size > ctr) {
          DDR_write32(0x12345678, (ctr + fc2_offset + g_rsx_bar2_addr));
          ctr+=4;
      }
    }
    
    // build and store DMA control address 0x28000C00000 and size 0x20000
    fifo->dma_control_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0) + 0xC00000;
    fifo->dma_control_size = 0x20000;
    
    // map DMA memory
    ret = rsx_map_io_to_lpar((S32)core->core_id,       // (IN) device core ID, 2
                             fifo->dma_control_addr,       // (IN) DMA control address
                             12,                           // (IN) page size lshift count
                             fifo->dma_control_size,       // (IN) DMA control size
                             &fifo->dma_control_lpar);     // (OUT) mapped LPAR address
    
    if (ret != 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // set FIFO object init flag to 1
    fifo->init = 1;
    
    // init/update BAR0 registers, e.g. offsets for fc1, fc2 and run_out
    fifo->sub21BA08();
    
    return;
}
