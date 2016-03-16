#ifndef __RSX_CORE_MEMORY_H__
#define __RSX_CORE_MEMORY_H__



int32_t rsx_core_memory_get_BAR1_offset_by_address(rsx_core_mem_obj_t *core_mem, int64_t addr);
rsx_mem_ctx_obj_t *rsx_core_memory_get_memory_context_by_id(rsx_core_mem_obj_t *core_mem, uint32_t mem_ctx_id);
rsx_core_mem_obj_t *rsx_core_memory_allocate_memory_context(rsx_core_mem_obj_t *core_mem, int32_t local_size, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4);
int32_t rsx_core_memory_value_div_by_16(rsx_core_mem_obj_t *core_mem, int32_t offset);
int32_t rsx_core_memory_get_BAR2_offset_by_address(rsx_core_mem_obj_t *core_mem, int64_t addr);
int64_t rsx_core_memory_get_mem_reg_addr_by_id(rsx_core_mem_obj_t *core_mem, int32_t mem_region_id);
int32_t rsx_core_memory_get_mem_reg_size_by_id(rsx_core_mem_obj_t *core_mem, int32_t mem_region_id);
rsx_core_mem_obj_t *rsx_core_memory_ctor(int64_t rsx_core_id);


#endif // __RSX_CORE_MEMORY_H__
