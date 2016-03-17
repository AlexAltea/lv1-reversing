/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// RSX external interrupt controller object, DUMMY, TODO
struct rsx_device_eic_t {
    S32 unk_000;              // 0x00: 
    // ...
};

S32 rsx_device_eic_219C18(S32 id, S64 *out);
void rsx_device_eic_init_flags(void);
