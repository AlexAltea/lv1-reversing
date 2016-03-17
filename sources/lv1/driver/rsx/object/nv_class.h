/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX object nv object, size 0x10
struct rsx_object_nv_class_t {
    S32 type;                   // 0x00: 
    U32 handle;                 // 0x04: 
    S64 bar2_offset;            // 0x08: BAR2(PRAMIN) offset

    // Methods
    S32 get_object_size();
    U32 get_object_handle();
    S64 get_bar2_address();
};

void rsx_object_nv_class_create_object(rsx_object_nv_class_t* nv_obj, U32 type, U32 handle);
