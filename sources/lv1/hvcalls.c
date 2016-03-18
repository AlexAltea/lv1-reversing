/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "hvcalls.h"

#include "lv1/lv1.h"
#include "lv1/lv1_misc.h"
#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/core/device.h"

// HyperCall 115
S32 lv1_undocumented_function_115(S64 ioas_id, S64 segment_size, S64 io_page_size) {
    sub2DBE04(ioas_id, segment_size, io_page_size);
}

// HyperCall 116
S32 lv1_allocate_io_segment(S64 ioas_id, S64 segment_size, S64 io_page_size, S64& ioif_addr) {
    sub2DBE04(ioas_id, segment_size, io_page_size);
    if (ioas_id < 4) {
        return sub2DAE94(-0x11ULL, 1, 0, 0, 0, 0);
    } else {
        S64 unk = sub_2DAD24();
        return sub_2DC630(0, unk, ioas_id);
    }
}

/***********************************************************************
* hypercall 217
***********************************************************************/
void lv1_gpu_context_allocate(U32 mem_ctx_id, U64 system_mode) {
    rsx_core_device_t* core = NULL;
    rsx_memory_context_t* mem_ctx = NULL;
    rsx_core_context_t* context = NULL;
    
    if (g_rsx_open_status != 0)    {
        // extldi    r0, r4, 63,59 # system_mode, unset [58:58]
        // extldi    r0, r0, 60,5  # system_mode, unset [60:63]
        //if ((ROTL64((ROTL64(system_mode, 59) & 0xFFFFFFFFFFFFFFFE), 5) & 0xFFFFFFFFFFFFFFF0) != 0)
        if (((system_mode & 0xFFFFFFFFFFFFFFD0) != 0) && (g_rsx_open_status > 1)) {
            g_r3 = LV1_SUCCESS;
            return;
        }
    }
    
    // TODO: tb and eic stuff...
    
    // get RSX device core object
    core = g_rsx_core_obj_tbl[2];
    RSX_ASSERT(core);
    
    // pause FIFO
    core->dev_fifo->pause();
    
    // ? pause GRAPH ?
    rsx_device_graph_21D054((void*)core->dev_graph_obj);
    
    // get our RSX memory context
    mem_ctx = rsx_core_memory_get_memory_context_by_id((void*)core->core_mem, mem_ctx_id);
    if (mem_ctx == NULL) {
        g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
        return;
    }
    
    // get a free RSX context
    for (S32 i = 0; i < 3; i++) {
        if (core->context[i] == NULL) {
            // allocate and set new RSX context object
            context = rsx_core_context_allocate(core->core_id, mem_ctx, system_mode);

            // ...

            printf("current... [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);

            g_r3 = LV1_SUCCESS;
            return;
        }
    }
    
    // fail, all 3 RSX contexts are in use
    g_r3 = LV1_RESOURCE_SHORTAGE;
    return;
}


/***********************************************************************
* hypercall 214
***********************************************************************/
void lv1_gpu_memory_allocate(S32 local_size, S64 arg1, S64 arg2, S64 arg3, S64 arg4) {
    S32 size = 0;
    rsx_core_device_t* core = NULL;
    rsx_memory_context_t* mem_ctx = NULL;
    
    
    if (g_rsx_open_status > 1) {
        g_r3 = LV1_SUCCESS;          // fail, no core inited
        return;
    }
    
    // get RSX device core object
    core = (void*)g_rsx_core_obj_tbl[2];
    if (core == NULL) {
        g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
        return;
    }
    
    // if local size to allocate == 0, allocate all
    if (local_size == 0) {
        // get total DDR memory size, 256 MB
        size = rsx_bus_ioif0_get_BAR1_size((void*)core->ioif0);
        
        // but cut off the 2 MB system reserved DDR memory
        size -= 0x200000;
    } else {
        size = local_size;
    }
    
    // allocate
    mem_ctx = (void*)rsx_core_memory_allocate_memory_context((void*)core->core_mem, // (IN) RSX device core memory object
                                                             size,                      // (IN) local memory size to allocate
                                                             arg1,                      // (IN) ?, 8
                                                             arg2,                      // (IN) ?, 0x30000
                                                             arg3,                      // (IN) ?, 16
                                                             arg4);                     // (IN) ?, 8
    
    if (mem_ctx == NULL) {
        g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
        return;
    }
    
    // OK
    g_r3 = LV1_SUCCESS;
    g_r4 = mem_ctx->id;
    g_r5 = mem_ctx->lm_start_lpar;
    return;
}


/***********************************************************************
* hypercall 212
***********************************************************************/
void lv1_gpu_device_map(S32 device_id) {
    S32 ret = -1;
    S64 dev_lpar_addr = 0;
    S64 dev_lpar_size = 0;
    rsx_core_device_t* core = NULL;
    
    
    // check device ID based on global stats
    if (g_rsx_open_status != 0) {
        // device ID 1, 2 or 5 
      if ((device_id <= 11) && ((1 << device_id) & 0x26)) {
            g_r3 = LV1_SUCCESS;
        return;
        }
      
      // device ID 9 or 11
      if (((1 << device_id) & 0xA00) && (g_rsx_unk_01 == 0) && ((g_rsx_open_status > 1))) {
            g_r3 = LV1_SUCCESS;
        return;
        }
    }
    
    // TODO: tb and eic stuff...
  
  // get RSX device core object
  core = (void*)g_rsx_core_obj_tbl[2];
  if (core == NULL) {
    g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
    return;
  }
  
  // map RSX device
  ret = rsx_core_device_map_device(core, device_id, &dev_lpar_addr, &dev_lpar_size);
  
  // store return values
  g_r4 = dev_lpar_addr;
  g_r5 = dev_lpar_size;
  
  // TODO: tb and eic stuff...
  
  g_r3 = ret;
  return;
}


/***********************************************************************
* hypercall 211
***********************************************************************/
void lv1_gpu_close(void) {
    S32 ret = -1;
    S32 dev_core_id = 2;    // it's always 2 in my dumps
    
    
    if (g_rsx_open_status != 0)   // not opened, nothing to close {
        g_r3 = LV1_SUCCESS;
        return;
    }
    
    ret = rsx_core_device_close_device(dev_core_id);
    
    g_r3 = ret;
    return;
}


/***********************************************************************
* hypercall 210
***********************************************************************/
void lv1_gpu_open(S32 arg1) {
    S32 ret = -1;
    U64 value = 0;
    S32 dev_core_id = 2;    // it's always 2 in my dumps
    
    
    if (g_rsx_open_status > 1)   // already open... {
        g_r3 = LV1_SUCCESS;
        return;
    }
    
    get_param_value("sys.boot.gos", &value);
    if (value > 3) {
        g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
        return;
    }
    
    if (value == 3)
      g_rsx_open_status = 1;
    
    ret = rsx_core_device_open(arg1, dev_core_id);
    
    g_r3 = ret;
    return;
}
