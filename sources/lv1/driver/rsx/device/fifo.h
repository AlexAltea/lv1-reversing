/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// RSX device FIFO object, size 0x80
struct rsx_device_fifo_t {
    S64 fc1_addr;           // 0x00: 0x28002020000(memory region 4 address)
    S32 fc1_size;           // 0x08: 0x200(memory region 4 size)
    S32 unk_0C;             // 0x0C: ?
    //--------------------------------------------------------------------
    S64 fc2_addr;           // 0x10: 0x28002030000(memory region 5 address)
    S64 fc2_size;           // 0x18: 0x4000(memory region 5 size)
    //--------------------------------------------------------------------
    S64 run_out_addr;       // 0x20: 0x28002018000(memory region 3 address)
    S32 run_out_size;       // 0x28: 0x200(memory region 3 size)
    S32 unk_2C;             // 0x2C: ?
    //--------------------------------------------------------------------
    S64 dma_control_addr;   // 0x30: 0x28000C00000, dma_control_addr
    S64 dma_control_lpar;   // 0x38: 0x4400001C0000, dma_control_lpar address, DBG: fifo mmio region
    S32 dma_control_size;   // 0x40: 0x20000, dma_control_size
    //--------------------------------------------------------------------
    S32 pause;              // 0x44: pause, 1(on) or 2(off) ?
    S32 init;               // 0x48: init_flag
    S32 unk_4C;             // 0x4C: 
    //--------------------------------------------------------------------
    S64 unk_50[4];          // 0x50: ?
    //S64 unk_58;             // 0x58: 
    //S64 unk_60;             // 0x60: 
    //S64 unk_68;             // 0x68: 
    //--------------------------------------------------------------------
    S64 unk_70;             // 0x70: 
    S64 unk_78;             // 0x78: 

    // Methods
    void sub21B5FC(S32 arg1, S32 arg2);
    void sub21BA08();
    void init();

    S64  get_channel_dma_control_lpar_address(S32 channel_id);
    void sub21B668(S32 channel_id, S32 arg1);
    void sub21C234(S32 channel_id);
    void sub21CCC0(S32 channel_id, S32 offset);
    S64  sub21B7DC(S32 idx);
    S64  get_fc2_channel_address_by_id(S32 channel_id);
    S64  get_fc1_channel_address_by_id(S32 channel_id);
    S32  pause();
    void resume();
};
