/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX debug object
struct rsx_dbg_obj_t {
    S64 unk_00;                  // 0x00: ? init 0
    S32 unk_08;                  // 0x08: ?
    S32 unk_0C;                  // 0x0C: ?
    S64 unk_10;                  // 0x10: init 0xF4240(1000000)
    S64 unk_18;                  // 0x18: init creation-time tb value + unk_10
    S64 unk_20;                  // 0x20: ?
} rsx_dbg_obj;

S32 rsx_utils_debugger_ctor(void);
