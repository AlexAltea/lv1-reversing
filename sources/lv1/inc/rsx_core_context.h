#ifndef __RSX_CORE_CONTEXT_H__
#define __RSX_CORE_CONTEXT_H__




int32_t rsx_core_context_iomap(rsx_ctx_obj_t *rsx_ctx, uint64_t ea, int64_t lpar_addr, int32_t size, uint64_t flags);
int64_t rsx_core_context_get_dma_control_lpar_address(rsx_ctx_obj_t *rsx_ctx);
int64_t rsx_core_context_get_driver_info_lpar_addr(rsx_ctx_obj_t *rsx_ctx);
uint32_t rsx_core_context_get_rsx_context_id(rsx_ctx_obj_t *rsx_ctx);
int32_t rsx_core_context_get_size_of_reports(void);
void rsx_core_context_init(rsx_ctx_obj_t *rsx_ctx, uint64_t *out, int64_t core_id, rsx_mem_ctx_obj_t *mem_ctx, uint64_t system_mode, int32_t idx);
rsx_ctx_obj_t *rsx_core_context_allocate(int64_t core_id, rsx_mem_ctx_obj_t *mem_ctx, uint64_t system_mode);


#endif // __RSX_CORE_CONTEXT_H__
