/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"



S32 rsx_core_context_iomap(rsx_ctx_obj_t* rsx_ctx, uS64 ea, S64 lpar_addr, S32 size, uS64 flags);
S64 rsx_core_context_get_dma_control_lpar_address(rsx_ctx_obj_t* rsx_ctx);
S64 rsx_core_context_get_driver_info_lpar_addr(rsx_ctx_obj_t* rsx_ctx);
uS32 rsx_core_context_get_rsx_context_id(rsx_ctx_obj_t* rsx_ctx);
S32 rsx_core_context_get_size_of_reports(void);
void rsx_core_context_init(rsx_ctx_obj_t* rsx_ctx, uS64 *out, S64 core_id, rsx_mem_ctx_obj_t* mem_ctx, uS64 system_mode, S32 idx);
rsx_ctx_obj_t* rsx_core_context_allocate(S64 core_id, rsx_mem_ctx_obj_t* mem_ctx, uS64 system_mode);


#endif // __RSX_CORE_CONTEXT_H__
