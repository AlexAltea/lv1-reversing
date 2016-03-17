/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX object sw object, size 0x18
struct rsx_object_sw_class_t {
    S32 id;                     // 0x00: ID ?
    U32 handle;                 // 0x04: 0xCAFEBABE
    S64 opd;                    // 0x08: OPD of rsx_object_sw_driver()
    S64 bar2_addr;              // 0x10: BAR2(PRAMIN) address

    // Methods
    U32  get_object_handle();
    S64  get_bar2_address();
};

void rsx_object_sw_class_create_object(rsx_object_sw_class_t* sw_obj, U32 type, S64* opd);