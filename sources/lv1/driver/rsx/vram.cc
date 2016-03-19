/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "vram.h"

#include "lv1/memory.h"

inline U32 vram_rd32(U32 offset) {
    return DDR_read32(0x28080000000ULL | offset);
}

inline void vram_wr32(U32 offset, S32 value) {
    return DDR_write32(value, 0x28080000000ULL | offset);
}

inline U32 ramin_rd32(U32 offset) {
    return DDR_read32(0x28002000000ULL | offset);
}

inline void ramin_wr32(U32 offset, S32 value) {
    return DDR_write32(value, 0x28002000000ULL | offset);
}
