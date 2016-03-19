/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

inline U32 vram_rd32(U32 offset);
inline void vram_wr32(U32 offset, U32 value);

inline U32 ramin_rd32(U32 offset);
inline void ramin_wr32(U32 offset, U32 value);
