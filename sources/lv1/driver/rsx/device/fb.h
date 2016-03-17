/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// RSX framebuffer object, size 0x10
struct rsx_device_fb_t {
    S32 DDR_MB_CTags;       // 0x00: 0x80000
    S32 size;               // 0x04: BAR1(VRAM) size 0x10000000(256 MB), from register BAR0:0x2800010020C
    S64 io_addr;            // 0x08: BAR1(VRAM) start address, 0x28080000000

    // Methods
    void sub21ADAC();
    void init();
};
