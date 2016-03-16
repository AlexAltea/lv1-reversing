#ifndef __RSX_OBJECT_CONTEXT_DMA_H__
#define __RSX_OBJECT_CONTEXT_DMA_H__





int32_t rsx_object_context_dma_get_object_size(void);
int64_t rsx_object_context_dma_object_get_BAR2_address(rsx_ctx_dma_obj_t *dma_obj);
uint32_t rsx_object_context_dma_get_object_class(rsx_ctx_dma_obj_t *dma_obj);


void rsx_object_context_dma_220064(rsx_ctx_dma_obj_t *dma_obj, int32_t arg1, int32_t arg2, int64_t addr, int32_t size);
rsx_ctx_dma_obj_t *rsx_object_context_dma_create_obj(uint32_t type);



#endif // __RSX_OBJECT_CONTEXT_DMA_H__
