/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "common/types.h"

// HyperCalls

/**
 * LV1 SysCall 210 (0xD2): lv1_gpu_open
 */
void lv1_gpu_open(S32 arg1);

/**
 * LV1 SysCall 211 (0xD3): lv1_gpu_close
 */
void lv1_gpu_close(void);

/**
 * LV1 SysCall 212 (0xD4): lv1_gpu_device_map
 */
void lv1_gpu_device_map(S32 device_id);

/**
 * LV1 SysCall 214 (0x6): lv1_gpu_memory_allocate
 */
void lv1_gpu_memory_allocate(S32 local_size, S64 arg4, S64 arg5, S64 arg6, S64 arg7);

/**
 * LV1 SysCall 217 (0x9): lv1_gpu_context_allocate
 */
void lv1_gpu_context_allocate(U32 mem_ctx_id, U64 system_mode);

/**
 * LV1 SysCall 221 (0xD): lv1_gpu_context_iomap
 */
void lv1_gpu_context_iomap(U32 rsx_ctx_id, U64 arg1, S64 lpar_addr, S32 size, U64 flags);
