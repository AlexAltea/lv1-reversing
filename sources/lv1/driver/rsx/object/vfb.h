/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX object v framebuffer object, size 0x40
struct rsx_object_vfb_t {
    S64 unk_00;                 // 0x00: 
    S64 unk_08;                 // 0x08: 
    S64 unk_10;                 // 0x10: 
    S32 unk_18;                 // 0x18: 
    S08 unk_1C;                 // 0x1C: 
    S08 unk_1D[3];              // 0x1D: padding
    S64 unk_20;                 // 0x20: 
    S64 unk_28;                 // 0x28: 
    S64 unk_30;                 // 0x30: 
    S64 unk_38;                 // 0x38: 

    void init();
};
