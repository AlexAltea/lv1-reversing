/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "hvcalls.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/***********************************************************************
* hypercall 217
***********************************************************************/
void lv1_gpu_context_allocate(U32 mem_ctx_id, U64 system_mode) {
    S32 idx;
    rsx_core_device_t* core = NULL;
    rsx_mem_ctx_obj_t* mem_ctx = NULL;
     = NULL;
    
    
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
  core = (void*)g_rsx_core_obj_tbl[2];
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // pause FIFO
    rsx_device_fifo_pause((void*)core->dev_fifo_obj);
    
    // ? pause GRAPH ?
    rsx_device_graph_21D054((void*)core->dev_graph_obj);
    
    // get our RSX memory context
    mem_ctx = rsx_core_memory_get_memory_context_by_id((void*)core->core_mem_obj, mem_ctx_id);
    if (mem_ctx == NULL) {
        g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
        return;
    }
    
    // get a free RSX context
    for (idx = 0; idx < 3; idx++) {
        if (core->rsx_ctx[idx] == NULL) {
            // allocate and set new RSX context object
          rsx_ctx = rsx_core_context_allocate(core->core_id, mem_ctx, system_mode);
          
          
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
    rsx_mem_ctx_obj_t* mem_ctx = NULL;
    
    
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
    mem_ctx = (void*)rsx_core_memory_allocate_memory_context((void*)core->core_mem_obj, // (IN) RSX device core memory object
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

/***********************************************************************
* test
***********************************************************************/
void lv1_test() {
    /*
    S32 i;
    S64 *map = NULL;
    rsx_utils_bitmap_t* bm_obj = NULL;
    rsx_core_device_t* core = NULL;
    rsx_core_memory_t* core_mem = NULL;
    
    
    
    // get device core object
    core = (void*)g_rsx_core_obj_tbl[g_rsx_core_id];
    
    core_mem = (void*)core->core_mem_obj;
    
    bm_obj = (void*)core_mem->bm_local_mem;
    map = bm_obj->bitmap;
    */
    /*
    printf("value1: 0x%016llX\nitem_total: 0x%016llX\n", bm_obj->value1, bm_obj->item_total);
    printf("seg_count: 0x%08X\n", bm_obj->seg_count);
    */
    
    
    //rsx_utils_bitmap_dealloc(bm_obj, 0x7000B0500000, 249);
    //rsx_utils_bitmap_dealloc(bm_obj, 0x7000B0000000, 5);
    
    /*
    S32 test = 62;
    for (i = 0; i < test; i++)
      rsx_utils_bitmap_set_bit(bm_obj, i, 1);
    
    
    
    rsx_utils_bitmap_set_bit(bm_obj, 4, 1);
    
    rsx_utils_bitmap_set_bit(bm_obj, 4, 0);
    
    for (i = 0; i < bm_obj->seg_count; i++)
      printf("seg %i: 0x%016llX\n", i, map[i]);
    */
    return;
}
