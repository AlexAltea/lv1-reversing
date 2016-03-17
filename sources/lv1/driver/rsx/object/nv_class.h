/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX object nv object, size 0x10
struct rsx_object_nv_class_t {
    S32 unk_00;                 // 0x00: 
    S32 obj_class;              // 0x04: 
    S64 bar2_offset;            // 0x08: BAR2(PRAMIN) offset

    // Methods
    S32 rsx_object_nv_class_get_object_size(void);
    S64 rsx_object_nv_class_get_BAR2_address(rsx_object_nv_class_t* nv_obj);
    U32 rsx_object_nv_class_get_object_class(rsx_object_nv_class_t* nv_obj);
    void rsx_object_nv_class_create_object(rsx_object_nv_class_t* nv_obj, U32 arg1, U32 arg2);
};
