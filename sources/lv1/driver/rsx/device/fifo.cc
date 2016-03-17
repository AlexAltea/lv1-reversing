/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>




/***********************************************************************
* 
***********************************************************************/
S64 rsx_device_fifo_get_channel_dma_control_lpar_address(rsx_device_fifo_t* fifo, S32 channel_id) {
    return (fifo->dma_control_lpar + (channel_id <<12));
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_device_fifo_21B7DC(rsx_device_fifo_t* fifo, S32 idx) {
    if ((idx - 1) > 2) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
    return fifo->unk_50[idx];
}

/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_21B668(rsx_device_fifo_t* fifo, S32 channel_id, S32 arg1) {
    //S32 value;
    
    // check channel ID, ??? 32 max channels ???
    if (channel_id > 31) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
  
  // TODO:
  
  
  
  
  
  
  
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_21C234(rsx_device_fifo_t* fifo, S32 channel_id) {
    //S32 value;
    rsx_core_device_t* core = NULL;
    rsx_device_fifo_t* _fifo = NULL;
    
    
    
    
    // check channel ID, ??? 32 max channels ???
    if (channel_id > 31) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    
    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    
    // get FIFO object again and pause FIFO
    _fifo = (void*)core->dev_fifo_obj;
    rsx_device_fifo_pause(_fifo);
    
    
    
    // ? TODO:
    rsx_device_fifo_21B668(fifo, channel_id, 1);
    
    
    
    // TODO:
    
    
    
    
    return;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_21CCC0(rsx_device_fifo_t* fifo, S32 channel_id, S32 offset) {
    S32 tmp, value, fc1_offset;
    S64 fc1_addr;
    rsx_core_device_t* core = NULL;
    rsx_core_memory_t* core_mem = NULL;
    
    
    
    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    
    tmp = rsx_core_memory_value_div_by_16((void*)core, offset);
    
    // read value from BAR0:0x28000003204
    value = read_BAR0(0x28000003204);     // unknow value! not inited, here 0
    value &= 0x1FF;
    
    /*if (value == channel_id) {
      write_BAR0(tmp, 0x280000032E0);
      return;
    }*/
    
    // get channel fc1 address
    fc1_addr = rsx_device_fifo_get_fc1_channel_address_by_id(fifo, channel_id);
    
    // get channel fc1 offset
    core_mem = (void*)core->core_mem_obj;
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
S64 rsx_device_fifo_get_fc2_channel_address_by_id(rsx_device_fifo_t* fifo, S32 channel_id) {
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    
    
    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
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
    
    // 0x28002030000 + 0(0), 1(0x1000), 2(0x2000), 3(0x3000)
    return (fifo->fc2_addr + (channel_id <<12));
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_device_fifo_get_fc1_channel_address_by_id(rsx_device_fifo_t* fifo, S32 channel_id) {
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    
    
    // get RSX device core
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
  if (core == NULL) {
      printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }
    
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




/***********************************************************************
* 
***********************************************************************/
S32 rsx_device_fifo_pause(rsx_device_fifo_t* fifo) {
    S32 i, value;
    
    
    fifo->pause++;
    
    value = read_BAR0(0x28000002500);
    value &= 0xFFFFFFFE;                     // unset value[31:31]
    write_BAR0(value, 0x28000002500);
    
    value = read_BAR0(0x28000002500);
    
    // if value[28:28] set, wait and try again
    if ((value &= 0xFFFFFFEF) != 0) {
    for (i = 0; i < 1000000; i++) {
            // db16cyc
      // db16cyc
      // db16cyc
      // db16cyc
      // eieio
            value = read_BAR0(0x28000002500);
            
            // if value[28:28] unset, ok
            if ((value &= 0xFFFFFFEF) == 0)
              break;
        }
    }
    
    value = read_BAR0(0x28000003200);
    
    // if value[32:32] unset...
    if ((value & 0xFFFFFFFE) == 0)
      return 0;
    
    value = read_BAR0(0x28000003200);
    value &= 0xFFFFFFFE;                     // unset value[31:31]
    write_BAR0(value, 0x28000003200);
    
    value = read_BAR0(0x28000003220);
    value &= 0xFFFFFFFE;                     // unset value[31:31]
    write_BAR0(value, 0x28000003220);
    
    
    /**/
    for (i = 0; i < 1000001; i++) {
        value = read_BAR0(0x28000003250);
        if ((value & 0xFFFFEFFF) == 0)         // if value[19:19] unset... {
            // db16cyc                          // wait and try again
      // db16cyc
      // db16cyc
      // db16cyc
            continue;
        }
        
        value = read_BAR0(0x28000003304);
        if ((value & 0xFFFFFFFE) == 0)         // if value[31:31] unset... {
            // db16cyc                          // wait and try again
      // db16cyc
      // db16cyc
      // db16cyc
            continue;
        }
        
        value = read_BAR0(0x28000003250);
        value &= 0xFFFFFFFE;                  // unset value[31:31]
        write_BAR0(value, 0x28000003250);
        
        value = read_BAR0(0x28000003258);
        value &= 0xFEFFFFFF;                  // unset value[07:07]
        write_BAR0(value, 0x28000003258);
        
        value = read_BAR0(0x28000003250);
        if ((value & 0xFFFFFFEF) == 0)         // if value[27:27] unset... {
            
            
            
        }
        //TODO: fucking loops...
    }
    
    
    
    return 0;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_continue(rsx_device_fifo_t* fifo) {
    S32 value;
    
    
    if (fifo->pause == 0) {
        value = read_BAR0(0x28000003250);
        value |= 1;                         // set value[31:31]
        write_BAR0(value, 0x28000003250);
        
        value = read_BAR0(0x28000003200);
        value |= 1;                         // set value[31:31]
        write_BAR0(value, 0x28000003200);
        
        value = read_BAR0(0x28000003220);
        value |= 1;                         // set value[31:31]
        write_BAR0(value, 0x28000003220);
        
        if (((rsx_core_device_get_rsx_enable_flag()) != 0) && (fifo->init != 1))
          return;
        
        value = read_BAR0(0x28000002500);
        value |= 1;                         // set value[31:31]
        write_BAR0(value, 0x28000002500);
    }
    
    fifo->pause--;
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_21B0B0(rsx_device_fifo_t* fifo) {
    S32 value;
    
    
    // update BAR0 registers
    value = read_BAR0(0x28000002230);
    value &= 0xFFFFFE00;                 // unset value[23:31]
    value |= 0xFF;                       // set value[24:31]
    write_BAR0(value, 0x28000002230);
    
    // set 32bit values to 0
    write_BAR0(0, 0x28000003210);
    write_BAR0(0, 0x28000003270);
    write_BAR0(0, 0x28000003240);        // reset DMA Put register
    write_BAR0(0, 0x28000003244);        // reset DMA Get register
    write_BAR0(0, 0x28000003258);
    
    write_BAR0(0, 0x28000002510);
    write_BAR0(0, 0x28000002514);
    write_BAR0(0, 0x28000002518);
    write_BAR0(0, 0x2800000251C);
    write_BAR0(0, 0x28000002520);
    write_BAR0(0, 0x28000002524);
    write_BAR0(0, 0x28000002528);
    write_BAR0(0, 0x2800000252C);
    write_BAR0(0, 0x28000002530);
    write_BAR0(0, 0x28000002534);
    write_BAR0(0, 0x28000002538);
    write_BAR0(0, 0x2800000253C);
    write_BAR0(0, 0x28000002540);
    write_BAR0(0, 0x28000002544);
    write_BAR0(0, 0x28000002548);
    write_BAR0(0, 0x2800000254C);
    
    // set flags
    value = read_BAR0(0x2800000250C);
    value |= 1;                          // set [31:31]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 2;                          // set [30:30]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 4;                          // set [29:29]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 8;                          // set [28:28]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x10;                       // set [27:27]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x20;                       // set [26:26]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x40;                       // set [25:25]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x80;                       // set [24:24]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x100;                      // set [23:23]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x200;                      // set [22:22]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x400;                      // set [21:21]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x800;                      // set [20:20]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x1000;                     // set [19:19]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x2000;                     // set [18:18]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x4000;                     // set [17:17]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x2800000250C);
    value |= 0x8000;                     // set [16:16]
    write_BAR0(value, 0x2800000250C);
    
    value = read_BAR0(0x28000002048);
    value &= 0xFEFFFFFF;                 // unset [07:07]
    write_BAR0(value, 0x28000002048);
    
    write_BAR0(0, 0x28000003228);        // reset DMA instance register
    write_BAR0(0, 0x280000032E8);
    write_BAR0(0, 0x28000002410);
    write_BAR0(0, 0x28000002420);
    
    value = read_BAR0(0x2800000221C);
    value &= 0xFFFFFFFE;                 // unset [31:31]
    write_BAR0(value, 0x2800000221C);
    
    write_BAR0(1, 0x28000002058);        // set to 1
    
    value = read_BAR0(0x28000002500);
    value &= 0xFFFFFFFE;                 // unset [31:31]
    write_BAR0(value, 0x28000002500);
    
  value = read_BAR0(0x28000003250);
    value &= 0xFFFFFFFE;                 // unset [31:31]
    write_BAR0(value, 0x28000003250);
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_21B5FC(rsx_device_fifo_t* fifo, S32 arg1, S32 arg2) {
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
void rsx_device_fifo_21BA08(rsx_device_fifo_t* fifo) {
    S32 value, fc1_offset, fc2_offset, run_out_offset, ctr = 0;
    rsx_core_device_t* core = NULL;
    
    
    // get device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // get fc1 offset, if offset is valid(not out of address range)
    fc1_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, fifo->fc1_addr);
    
    // get fc2 offset, if offset is valid(not out of address range)
    fc2_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, fifo->fc2_addr);
    
    // update BAR0 registers
    value = read_BAR0(0x28000002230);
    value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
    value |= 1;                          // set value[31:31]
    write_BAR0(value, 0x28000002230);
    
    // BAR0:0x28000002220 = NV40_PFIFO_RAMFC, RAM FIFO cash?
    // (the offsets for fc1 and fc2 are stored here, FIFO cash 1 and 2?)
    value = read_BAR0(0x28000002220);
    value &= 0xFFFFE000;                           // unset value[19:31]
    value |= (fc1_offset & 0x1FFF0000) >>16;
    write_BAR0(value, 0x28000002220);
    
    value = read_BAR0(0x28000002220);
    value &= 0xFF00FFFF;                           // unset value[08:15]
    value |= (fc2_offset & 0xFF0000);
    write_BAR0(value, 0x28000002220);              // value now 0x????30002
    
    // get run_out address, 0x28002018000 and size, 0x200(512 byte)
    fifo->run_out_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem_obj, 3);
    fifo->run_out_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem_obj, 3);
    
    if (fifo->run_out_size <= 0x1FF) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    //////////////////////////////////////////////////////////////////////
    // update BAR0 registers
    value = read_BAR0(0x28000002218);
    value &= 0xFFFEFFFF;                           // unset value[15:15]
    write_BAR0(value, 0x28000002218);
    //////////////////////////////////////////////////////////////////////
    
    // get run_out offset, if offset is valid(not out of address range)
    run_out_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, fifo->run_out_addr);
    
    //////////////////////////////////////////////////////////////////////
    // update BAR0 registers
    value = read_BAR0(0x28000002218);
    value &= 0xFFFFFE01;                           // unset value[23:30]
    value |= (run_out_offset & 0x1FE00) >>9;
    write_BAR0(value, 0x28000002218);
    //////////////////////////////////////////////////////////////////////
    
    // get run_out offset, if offset is valid(not out of address range)
    run_out_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, fifo->run_out_addr);
    
    // init FIFO run_out memory, with 0xFADEFADE
    while (fifo->run_out_size > ctr) {
      DDR_write32(0xFADEFADE, (ctr + run_out_offset + g_rsx_bar2_addr));
      ctr+=4;
  }
    
    // ???
    rsx_device_fifo_21B5FC(fifo, 1, 0x80);
    rsx_device_fifo_21B5FC(fifo, 2, 0x80);
    rsx_device_fifo_21B5FC(fifo, 3, 8);
    
    
    //////////////////////////////////////////////////////////////////////
    // update BAR0 registers
    value = 0;
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 1;                          // set [31:31]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 1;                          // set [31:31], again
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x10;                       // set [27:27]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x100;                      // set [23:23]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x1000;                     // set [19:19]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x10000;                    // set [15:15]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x40000;                    // set [13:13]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x80000;                    // set [12:12]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x100000;                   // set [11:11]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x200000;                   // set [10:10]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x400000;                   // set [09:09]
    write_BAR0(value, 0x28000002140);
    
    value = read_BAR0(0x28000002140);
    value |= 0x1000000;                  // set [07:07]
    write_BAR0(value, 0x28000002140);
    
    rsx_device_fifo_21B0B0(fifo);     // init/update BAR0 registers
  
    return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fifo_init(rsx_device_fifo_t* fifo) {
    S32 ret = -1, fc1_offset, fc2_offset, ctr = 0;
    rsx_core_device_t* core = NULL;
    
    
    // ?
    fifo->pause = 0;
    fifo->unk_2C = 0;
    
    // get device core object, we need IOIF0 next
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // get fc1 address, 0x28002020000
    fifo->fc1_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem_obj, 4);
    
    // get fc2 address, 0x28002030000
    fifo->fc2_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem_obj, 5);
    
    // get fc1 size, 0x200(512 byte)
    fifo->fc1_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem_obj, 4);
    
    // get fc2 size, 0x4000(16 KB)
    fifo->fc2_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem_obj, 5);
    
    // get fc1 offset, if offset is valid(not out of address range)
    fc1_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, fifo->fc1_addr);
    
    // get fc2 offset, if offset is valid(not out of address range)
    fc2_offset = rsx_core_memory_get_BAR2_offset_by_address((void*)core->core_mem_obj, fifo->fc2_addr);
    
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
    rsx_device_fifo_21BA08(fifo);
    
    return;
}
