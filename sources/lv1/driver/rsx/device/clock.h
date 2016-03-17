/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// Clock types
enum {
    L1GPU_DEVICE_CLOCK_ID_INVALID             = 0,
    L1GPU_DEVICE_CLOCK_ID_NVCLK               = 1,  // nvclk 
    L1GPU_DEVICE_CLOCK_ID_GEOM_CLKSHADER_CLK  = 2,  // geom clkshader clk
    L1GPU_DEVICE_CLOCK_ID_ROP_CLK             = 3,  // rop clk
    L1GPU_DEVICE_CLOCK_ID_MEMORY_CLK          = 4,  // memory clk
    L1GPU_DEVICE_CLOCK_ID_DISPLAY_CLK         = 5,  // display clk
    L1GPU_DEVICE_CLOCK_ID_HOST_CLK            = 6,  // host clk
};

// RSX device clock setting
struct rsx_dev_clock_setting_t {
    S32 unk_00;               // 0x00: 
    S32 unk_04;               // 0x04: 
    S32 offset_0;             // 0x08: 
    S32 offset_1;             // 0x0C: 
    S32 unk_10;               // 0x10: 
};

// LV1 RSX device clock object, size 0x30
struct rsx_device_clock_t {
    S64 vtab;                 // 0x00: pointer -> virtual table
    //--------------------------------------------------------------------
    S32 id;                   // 0x08: clock ID
    //--------------------------------------------------------------------
    S32 unk_0C;               // 0x0C: ? (init with 0)
    S32 unk_10;               // 0x10: ?
    S32 offset_0;             // 0x14: a IOIF offset, added to base address 
    S32 offset_1;             // 0x18: a IOIF offset, added to base address 
    S32 freq;                 // 0x1C: clock frequency in Hz
    S32 unk_20;               // 0x20: ?
    S32 unk_24;               // 0x24: ?

    // Methods
    S32 get_frequency();
};

void rsx_device_clock_rsx_create(rsx_device_clock_t* clock_obj, S32 id, S32 arg1);
