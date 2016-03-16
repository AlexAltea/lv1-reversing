/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#ifndef __RSX_OBJECT_CONTEXT_DMA_H__
#define __RSX_OBJECT_CONTEXT_DMA_H__





S32 rsx_object_context_dma_get_object_size(void);
S64 rsx_object_context_dma_object_get_BAR2_address(rsx_ctx_dma_obj_t* dma_obj);
uS32 rsx_object_context_dma_get_object_class(rsx_ctx_dma_obj_t* dma_obj);


void rsx_object_context_dma_220064(rsx_ctx_dma_obj_t* dma_obj, S32 arg1, S32 arg2, S64 addr, S32 size);
rsx_ctx_dma_obj_t* rsx_object_context_dma_create_obj(uS32 type);



#endif // __RSX_OBJECT_CONTEXT_DMA_H__
