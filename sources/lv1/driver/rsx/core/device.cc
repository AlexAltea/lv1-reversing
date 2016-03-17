/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>












/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_device_map_device(rsx_dev_core_obj_t* core, S32 device_id, S64 *dev_lpar_addr, S64 *dev_lpar_size) {
    S64 io_addr = 0, lpar_size = 0;
    S32 ret = -1, io_size = 0;
    rsx_dev_audio_obj_t* dev_audio = NULL;
    
    
    // device ID check
    if ((device_id < 15) && (core->dev_lpar_addr[device_id] != 0))
        return LV1_DUPLICATE_ENTRY;
    
    if (((device_id - 9) > 6) && (g_rsx_unk_01 == 0) /*&& (*(HSPRG0(-0x67E8)) + 0x99... a flag)*/)
    return LV1_DENIED_BY_POLICY;
  
  if (device_id > 15)
        return LV1_ILLEGAL_PARAMETER_VALUE;
    
    switch(device_id) {
        ////////////////////////////////////////////////////////////////////
      // RSX device 0, BAR0
        case  0: {
          if (g_rsx_unk_00 == 0)
          return LV1_ILLEGAL_PARAMETER_VALUE;
          
          //if ((*(HSPRG0(-0x67E8)) + 0x99... a flag) != 0)
          //  return LV1_DENIED_BY_POLICY;
      
        io_size = rsx_bus_ioif0_get_BAR0_size((void*)core->ioif0);
          *dev_lpar_size = io_size;
          
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address 
          break;
        }
        //////////////////////////////////////////////////////////////////////
      // RSX device 1, audio
      case  1: {
          // get RSX device core audio object
          dev_audio = (void*)core->dev_audio_obj;
          if (dev_audio == NULL)
            return LV1_ILLEGAL_PARAMETER_VALUE;
          
          // map devive
          ret = rsx_device_audio_map_dev_1(dev_audio, &core->dev_lpar_addr[device_id], &lpar_size);
          
          *dev_lpar_size = lpar_size;
        
          break;
        }
        //////////////////////////////////////////////////////////////////////
      // RSX device 2, ?
      case  2: {
          *dev_lpar_size = io_size = 0x8000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x80100;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
        }
        //////////////////////////////////////////////////////////////////////
      // RSX device 3, ?
      case  3: {
      *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x1000000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 4, ?
      case  4: {
          //if ((*(HSPRG0(-0x67E8)) + 0x99... a flag) == 0)
        //  return LV1_ILLEGAL_PARAMETER_VALUE;
          
          *dev_lpar_size = io_size = 0x200000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x1800000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 5, ?
      case  5: {
          *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x8A000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 6, ?
      case  6: {
          *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x200000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 7, ?
      case  7: {
          *dev_lpar_size = io_size = 0x10000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x600000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 8, GPU
      case  8: {
          io_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem_obj, 12);
          io_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem_obj, 12);
          *dev_lpar_size = io_size;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 9, ?
      case  9: {
          *dev_lpar_size = io_size = 0x8000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x400000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 10, ?
      case 10: {
          *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x100000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 11, ?
      case 11: {
          *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0xA000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
        //////////////////////////////////////////////////////////////////////
      // RSX device 12, ?
      case 12: {
          *dev_lpar_size = io_size = 0x3000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x680000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
      //////////////////////////////////////////////////////////////////////
      // RSX device 13, ?
      case 13: {
          *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x90000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
      //////////////////////////////////////////////////////////////////////
      // RSX device 14, ?
      case 14: {
          *dev_lpar_size = io_size = 0x2000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x2000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
      //////////////////////////////////////////////////////////////////////
      // RSX device 15, ?
      case 15: {
          *dev_lpar_size = io_size = 0x1000;
          io_addr = rsx_bus_ioif0_get_BAR0_addr((void*)core->ioif0);
          io_addr += 0x88000;
          
          // map device
        ret = rsx_map_io_to_lpar(core->core_id,                     // (IN)  memory core ID
                                 io_addr,                           // (IN)  IO address to map
                                 12,                                // (IN)  lshift, page size
                                 io_size,                           // (IN)  IO size
                                 &core->dev_lpar_addr[device_id]);  // (OUT) mapped LPAR address
        break;
      }
    }
    
    *dev_lpar_addr = core->dev_lpar_addr[device_id];
        
  return ret;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_core_device_finalize(rsx_dev_core_obj_t* core) {
    rsx_fifo_obj_t* fifo = NULL;
    rsx_graph_obj_t* graph = NULL;
    
    
    
    // 1. pause FIFO
    fifo = (void*)core->dev_fifo_obj;
    rsx_device_fifo_pause(fifo);
    
    // 2. graph pause ?
    graph = (void*)core->dev_graph_obj;
    rsx_device_graph_21D038(graph);
    
    // 3. RSX context ?
    //rsx_core_context_215CD8();
    
    
    // TODO: later, here are non existing stuff, like context
    
    
    
    
    
    
    
    
    
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_device_close_device(S32 dev_core_id) {
    U64 value = 0;
    rsx_dev_core_obj_t* core = NULL;
    
    
    get_param_value("sys.lv1.rsxenable", &value);
    if (value <= 1)
        return LV1_NO_ENTRY;
    
    get_param_value("sys.lv1.rsxdebug", &value);
    
    // global core ID is 0xFF(invalid), there is no RSX device core object
    if (g_rsx_core_id == 0xFF)
        return LV1_SUCCESS;
    
    // global core ID not given core ID 
    if (g_rsx_core_id != dev_core_id)
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // given core ID out of range, 0 to 15
    if (dev_core_id > 15)
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    core = (void*)g_rsx_core_obj_tbl[dev_core_id];
    
    // if RSX device core object not exist
    if (core == NULL)
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // finalize core
    rsx_core_device_finalize(core);
    
    
    
    
    
    //printf("test: 0x%08X\n", 0);
    
    return LV1_SUCCESS;
}






/***********************************************************************
* 
***********************************************************************/
rsx_dev_core_obj_t* rsx_core_device_get_core_object() {
    rsx_dev_core_obj_t* core = NULL;
    
    
    if (g_rsx_core_id > 15) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return NULL;
    }
    
    core = (void*)g_rsx_core_obj_tbl[g_rsx_core_id];
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return NULL;
    }
    
    return core;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_core_device_get_rsx_enable_flag() {
    return g_rsx_enabled;
}

/***********************************************************************
* 
***********************************************************************/
rsx_dev_core_obj_t* rsx_core_device_get_core_object_by_id(S32 dev_core_id) {
    // check device core ID range
    if (dev_core_id > MAX_RSX_CORE_ID)
      return NULL;
    
    // get RSX device core object
    rsx_dev_core_obj_t* core_obj = (void*)g_rsx_core_obj_tbl[dev_core_id];
    
    return core_obj;
}

/***********************************************************************
* 
***********************************************************************/
static void rsx_core_device_init(rsx_dev_core_obj_t* dev_core, S32 arg1, S32 dev_core_id) {
    S32 i;
    rsx_utils_bm_obj_t* bm_obj_channels = NULL;
    rsx_bus_ioif0_obj_t* ioif0 = NULL;
    rsx_dev_clock_obj_t* clock_1 = NULL;
    rsx_dev_clock_obj_t* clock_5 = NULL;
    rsx_core_mem_obj_t* core_mem = NULL;
    S64 *unk_0F0 = NULL;
    rsx_dev_fb_obj_t* fb = NULL;
    rsx_hash_tbl_obj_t* hash_tbl = NULL;
    rsx_fifo_obj_t* fifo = NULL;
    rsx_graph_obj_t* graph = NULL;
    rsx_obj_video_rsx_t* video_rsx = NULL;
    rsx_dev_audio_obj_t* dev_audio = NULL;
    rsx_obj_vfb_obj_t* vfb_obj = NULL;
    
    
    // set some core object values
    dev_core->device_id = arg1;
    dev_core->unk_000 = 8;
    dev_core->core_id = dev_core_id;
    
    // store new global device core object
    g_rsx_core_obj_tbl[dev_core_id] = (void*)dev_core;  
    
    // init debugger
    rsx_utils_debugger_ctor();
    
    // allocate a utils bitmap object, to handle 4 items, I think the 4(max) RSX channels
    bm_obj_channels = lv1_kmalloc(sizeof(rsx_utils_bm_obj_t));
    if (bm_obj_channels == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init bitmap object 
    rsx_utils_bitmap_create(bm_obj_channels, 0, 4, 1);
    
    // store RSX channel bitmap into core object
    dev_core->bm_obj_channels = (void*)bm_obj_channels;
    
    // init the 3 RSX context objects with 0
    for(i = 0; i < 3; i++)
      dev_core->rsx_ctx[i] = NULL;
    
    // init the 16 RSX device addresses with 0
    memset(&dev_core->dev_lpar_addr, 0, 0x80);
    
    // create and get IO interface 0
    ioif0 = rsx_bus_ioif0_init();
    if (ioif0 == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // store IO interface 0 into core object
    dev_core->ioif0 = (void*)ioif0;
    
    // set some globals
    g_rsx_unk_00 =  0;        // ? 
  g_rsx_unk_01 =  0;        // ? 
  g_rsx_unk_02 = -1;        // ? 
    
    // get BAR2 address 0x28002000000 and store global
    g_rsx_bar2_addr = rsx_bus_ioif0_get_BAR2_addr(ioif0);
    
    // get BAR0 address 0x28000000000 and store global
    g_rsx_bar0_addr = rsx_bus_ioif0_get_BAR0_addr(ioif0);
    
    
    // allocate RSX device clock object 1, "nvclk"
    clock_1 = lv1_kmalloc(sizeof(rsx_dev_clock_obj_t));
    if (clock_1 == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    rsx_device_clock_rsx_create(clock_1, 1, 1);
    
    // store RSX device clock object 1 into core object
    dev_core->dev_clock_1_obj = (void*)clock_1;
    
    
    // allocate RSX device clock object 5, "display clk"
    clock_5 = lv1_kmalloc(sizeof(rsx_dev_clock_obj_t));
    if (clock_5 == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    rsx_device_clock_rsx_create(clock_5, 5, 6);
    
    // store RSX device clock object 5 into core object
    dev_core->dev_clock_5_obj = (void*)clock_5;
    
    
    // TODO: not finished...
    // init IOIF0 memory
    rsx_bus_ioif0_219018(ioif0);
    
    //////////////////////////////////////////////////////////////////////
    // create and ret RSX core memory object
    core_mem = rsx_core_memory_ctor(dev_core->core_id);
    if (core_mem == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // store RSX core memory object into core object
    dev_core->core_mem_obj = (void*)core_mem;
    
    
    // TODO:
    // E(xternal)I(nterrupt)C(ontroller), object already exist, not created by RSX stuff
    // load global eic object and store it into core object
    /*
    if (g_rsx_eic_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    */
    dev_core->eic_obj = (void*)g_rsx_eic_obj;
    
    // EIC: set some register flags in BAR0, init ?
    rsx_device_eic_init_flags();
    
    
    //////////////////////////////////////////////////////////////////////
    // ??? a object, 1 byte in size, ?
    unk_0F0 = lv1_kmalloc(1);
    if (unk_0F0 == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // set BAR0 registers
    rsx_device_master_init();
    dev_core->unk_0F0 = (void*)unk_0F0;
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate device frame buffer object
    fb = lv1_kmalloc(sizeof(rsx_dev_fb_obj_t));
    if (fb == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init frame buffer and store RSX frame buffer object into core
    rsx_device_fb_init(fb);
    dev_core->dev_fb_obj = (void*)fb;
    
    //////////////////////////////////////////////////////////////////////
    // allocate RSX hash table object
    hash_tbl = lv1_kmalloc(sizeof(rsx_hash_tbl_obj_t));
    if (hash_tbl == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init RSX object hash table and store hash table object into core
    rsx_object_hash_table_init(hash_tbl);
    dev_core->obj_hash_tbl_obj = (void*)hash_tbl;
    
    //////////////////////////////////////////////////////////////////////
    // allocate RSX device FIFO object
    fifo = lv1_kmalloc(sizeof(rsx_fifo_obj_t));
    if (fifo == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init RSX FIFO and store object into core
    rsx_device_fifo_init(fifo);
    dev_core->dev_fifo_obj = (void*)fifo;
    
    //////////////////////////////////////////////////////////////////////
    // allocate RSX device graph object
    graph = lv1_kmalloc(sizeof(rsx_graph_obj_t));
    if (graph == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init RSX graph and store object into core
    rsx_device_graph_init(graph);
    dev_core->dev_graph_obj = (void*)graph;
    
    //////////////////////////////////////////////////////////////////////
    // allocate RSX object_video_RSX object
    video_rsx = lv1_kmalloc(sizeof(rsx_obj_video_rsx_t));
    if (video_rsx == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init RSX video object and store object into core
    rsx_object_video_rsx_init(video_rsx);
    dev_core->obj_video_rsx_obj = (void*)video_rsx;
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate a 1 byte object, looks like a dummy for something removed,
    // and init function is a nullsub(), I set object to NULL
    dev_core->unk_040 = NULL;
    
    
    //////////////////////////////////////////////////////////////////////
    // allocate RSX device audio object
    dev_audio = lv1_kmalloc(sizeof(rsx_dev_audio_obj_t));
    if (dev_audio == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init RSX device audio object and store object into core
    rsx_device_audio_init(dev_audio);
    dev_core->dev_audio_obj = (void*)dev_audio;
    
    //////////////////////////////////////////////////////////////////////
    // allocate RSX object v framebuffer object
    vfb_obj = lv1_kmalloc(sizeof(rsx_obj_vfb_obj_t));
    if (vfb_obj == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // init RSX object v frambuffer object and store object into core
    rsx_object_vfb_init(vfb_obj);
    dev_core->obj_vfb_obj = (void*)vfb_obj;
    
    
    // object creation finished
    
    
    // set a BAR0 register to -1
    rsx_device_graph_21D01C((void*)dev_core->dev_graph_obj);
    
    // again ?, called first time during init graph
    rsx_device_graph_21D2DC((void*)dev_core->dev_graph_obj);
    
    // continue, better, start RSX FIFO
    rsx_device_fifo_continue((void*)dev_core->dev_fifo_obj);
    
    // memset end of core object to 0
    memset(&dev_core->unk_128, 0, 0xA0);
    
    return;
}

/***********************************************************************
* create the device core object
***********************************************************************/
S32 rsx_core_device_open(S32 arg1, S32 dev_core_id) {
    U64 value = 0;
    rsx_dev_core_obj_t* dev_core = NULL;
    
  
    get_param_value("sys.lv1.rsxenable", &value);
    if (value <= 1)
        return LV1_NO_ENTRY;
    
    get_param_value("sys.lv1.rsxdebug", &value);
    
    // if not 0xFF(invalid, first time), RSX already inited
    if (g_rsx_core_id != 0xFF) {
        printf("rsx device already created! found:%i requesting:%i\n", g_rsx_core_id, dev_core_id);
        return LV1_BUSY;
    }
    
    // check device core ID range
    if (dev_core_id > MAX_RSX_CORE_ID)
      return LV1_ILLEGAL_PARAMETER_VALUE;
      
    // store global device core ID
    g_rsx_core_id = dev_core_id;
    
    // check global core object entry by ID, error if already exists
    if (g_rsx_core_obj_tbl[dev_core_id] != NULL)
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // alloc RSX device core object
    dev_core = lv1_kmalloc(sizeof(rsx_dev_core_obj_t));
    
    // init device core
    rsx_core_device_init(dev_core, arg1, dev_core_id);  
    
    // if RSX debug level 3, print rsx open time in usecs
    if (value == 3)
      printf("rsx t:    open %d us\n", 0);
    
    return LV1_SUCCESS;
}
