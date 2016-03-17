/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX device audio object, size 0x30
struct rsx_dev_audio_obj_t {
    S64 unk_00;                 // 0x00: ? eic_id_5
    S64 unk_08;                 // 0x08: ? eic_id_4
    S64 lpar_size;              // 0x10: RSX audio device LPAR size, 0x1000(4 KB), (memory region 9 size)
    S64 io_addr;                // 0x18: RSX audio device IO address, 0x2808FF20000, (memory region 9 start)
    S64 lpar_addr;              // 0x20: RSX audio device LPAR address, we use 0x30000001B000
    S64 io_addr_0;              // 0x28: copy of IO address, 0x2808FF20000

    // Methods
    S32 rsx_device_audio_map_dev_1(S64 *dev_lpar_addr, S64* dev_lpar_size);
    void rsx_device_audio_init();
};
