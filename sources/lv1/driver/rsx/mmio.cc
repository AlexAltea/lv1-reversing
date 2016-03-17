/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "mmio.h"

#include "lv1/memory.h"

inline U32 rsx_rd32(U32 offset) {
    return read_BAR0(0x28000000000ULL | offset);
}

inline void rsx_wr32(U32 offset, S32 value) {
    return write_BAR0(value, 0x28000000000ULL | offset);
}
