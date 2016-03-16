#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"







/***********************************************************************
* hypercall 221
***********************************************************************/
void lv1_gpu_context_iomap(uint32_t rsx_ctx_id, uint64_t arg1, int64_t lpar_addr, int32_t size, uint64_t flags)
{
	int32_t ret = -1, idx;
	uint32_t id;
	rsx_dev_core_obj_t *core = NULL;
	rsx_ctx_obj_t *rsx_ctx = NULL;
	
	
	if(g_rsx_open_status <= 1)
	{
		g_r3 = LV1_SUCCESS;          // fail, no core inited
		return;
	}
	
	// TODO: tb and eic stuff...
	
	// get RSX device core object
  core = (void*)g_rsx_core_obj_tbl[2];
	if(core == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// get RSX context by ID
	for(idx = 0; idx < 3; idx++)
	{
		// get RSX context
		rsx_ctx = (void*)core->rsx_ctx[idx];
		if(rsx_ctx == NULL)  // if no RSX context, try next
		  continue;
		
		// get RSX context ID
		id = rsx_core_context_get_rsx_context_id((void*)rsx_ctx);
		if(id == rsx_ctx_id)
		{
			// if right context, map IO
			ret = rsx_core_context_iomap((void*)rsx_ctx, arg1, lpar_addr, size, flags);
			
			// TODO: tb and eic stuff...
			
			g_r3 = LV1_SUCCESS;
	    return;
		}
	}
	
	// RSX context not found
	g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
	return;
}







/***********************************************************************
* hypercall 217
***********************************************************************/
void lv1_gpu_context_allocate(uint32_t mem_ctx_id, uint64_t system_mode)
{
	int32_t idx;
	rsx_dev_core_obj_t *core = NULL;
	rsx_mem_ctx_obj_t *mem_ctx = NULL;
	rsx_ctx_obj_t *rsx_ctx = NULL;
	
	
	if(g_rsx_open_status != 0)   
	{
		// extldi    r0, r4, 63,59 # system_mode, unset [58:58]
    // extldi    r0, r0, 60,5  # system_mode, unset [60:63]
		//if((ROTL64((ROTL64(system_mode, 59) & 0xFFFFFFFFFFFFFFFE), 5) & 0xFFFFFFFFFFFFFFF0) != 0)
		if(((system_mode & 0xFFFFFFFFFFFFFFD0) != 0) && (g_rsx_open_status > 1))
		{
			g_r3 = LV1_SUCCESS;
			return;
		}
	}
	
	// TODO: tb and eic stuff...
	
	// get RSX device core object
  core = (void*)g_rsx_core_obj_tbl[2];
	if(core == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// pause FIFO
	rsx_device_fifo_pause((void*)core->dev_fifo_obj);
	
	// ? pause GRAPH ?
	rsx_device_graph_21D054((void*)core->dev_graph_obj);
	
	// get our RSX memory context
	mem_ctx = rsx_core_memory_get_memory_context_by_id((void*)core->core_mem_obj, mem_ctx_id);
	if(mem_ctx == NULL)
	{
		g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
		return;
	}
	
	// search a free RSX context
	for(idx = 0; idx < 3; idx++)
	{
		if(core->rsx_ctx[idx] == NULL)
		{
			// allocate and build new RSX context object
		  rsx_ctx = rsx_core_context_allocate(core->core_id, mem_ctx, system_mode);
		  
		  // if RSX context creation fails
		  if(rsx_ctx == NULL)
		  {
				g_r3 = LV1_RESOURCE_SHORTAGE;
				return;
			}
		  
		  // store RSX context object into RSX core object
		  core->rsx_ctx[idx] = (void*)rsx_ctx;
		  
		  // return RSX context ID into r4
		  g_r4 = rsx_core_context_get_rsx_context_id((void*)rsx_ctx);
		  
		  // return RSX context DMA control LPAR address into r5
		  g_r5 = rsx_core_context_get_dma_control_lpar_address((void*)rsx_ctx);
		  
		  // return RSX context driver info LPAR address into r6
		  g_r6 = rsx_core_context_get_driver_info_lpar_addr((void*)rsx_ctx);
		  
		  // return RSX context reports LPAR address into r7
		  g_r7 = rsx_ctx->reports_lpar_addr;
		  
		  // return RSX context reports size(0x10000, 64 KB) into r8
		  g_r8 = rsx_ctx->reports_size;
		  
		  // TODO: tb and eic stuff...
		  
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
void lv1_gpu_memory_allocate(int32_t local_size, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4)
{
	int32_t size = 0;
	rsx_dev_core_obj_t *core = NULL;
	rsx_mem_ctx_obj_t *mem_ctx = NULL;
	
	
	if(g_rsx_open_status > 1)   
	{
		g_r3 = LV1_SUCCESS;          // fail, no core inited
		return;
	}
	
	// TODO: tb and eic stuff...
	
	// get RSX device core object
  core = (void*)g_rsx_core_obj_tbl[2];
	if(core == NULL)
	{
	  g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
	  return;
	}
	
	// if local size to allocate == 0, allocate all
	if(local_size == 0)
	{
		// get total DDR memory size, 256 MB
		size = rsx_bus_ioif0_get_BAR1_size((void*)core->ioif0);
		
		// but cut off the 2 MB system reserved DDR memory
		size -= 0x200000;
	}
	else
	  size = local_size;
	
	// allocate
	mem_ctx = (void*)rsx_core_memory_allocate_memory_context((void*)core->core_mem_obj, // (IN) RSX device core memory object
	                                                         size,                      // (IN) local memory size to allocate
	                                                         arg1,                      // (IN) ?, 8
	                                                         arg2,                      // (IN) ?, 0x30000
	                                                         arg3,                      // (IN) ?, 16
	                                                         arg4);                     // (IN) ?, 8
	
	if(mem_ctx == NULL)
	{
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
void lv1_gpu_device_map(int32_t device_id)
{
	int32_t ret = -1;
	int64_t dev_lpar_addr = 0;
	int64_t dev_lpar_size = 0;
	rsx_dev_core_obj_t *core = NULL;
	
	
	// check device ID based on global stats
	if(g_rsx_open_status != 0)
	{
		// device ID 1, 2 or 5 
	  if((device_id <= 11) && ((1 << device_id) & 0x26))
	  {
			g_r3 = LV1_SUCCESS;
	    return;
		}
	  
	  // device ID 9 or 11
	  if(((1 << device_id) & 0xA00) && (g_rsx_unk_01 == 0) && ((g_rsx_open_status > 1)))
	  {
			g_r3 = LV1_SUCCESS;
	    return;
		}
	}
	
	// TODO: tb and eic stuff...
  
  // get RSX device core object
  core = (void*)g_rsx_core_obj_tbl[2];
  if(core == NULL)
  {
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
void lv1_gpu_close(void)
{
	int32_t ret = -1;
	int32_t dev_core_id = 2;    // it's always 2 in my dumps
	
	
	if(g_rsx_open_status != 0)   // not opened, nothing to close
	{
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
void lv1_gpu_open(int32_t arg1)
{
	int32_t ret = -1;
	uint64_t value = 0;
	int32_t dev_core_id = 2;    // it's always 2 in my dumps
	
	
	if(g_rsx_open_status > 1)   // already open...
	{
		g_r3 = LV1_SUCCESS;
		return;
	}
	
	get_param_value("sys.boot.gos", &value);
	if(value > 3)
	{
		g_r3 = LV1_ILLEGAL_PARAMETER_VALUE;
		return;
	}
	
	if(value == 3)
	  g_rsx_open_status = 1;
	
	ret = rsx_core_device_open(arg1, dev_core_id);
	
	g_r3 = ret;
	return;
}











/***********************************************************************
* test
***********************************************************************/
void lv1_test()
{
	
	
	
	
	
	
	
	
	
	
	return;
}
