/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "fb.h"

#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/mmio.h"
#include "lv1/driver/rsx/core/device.h"

/***********************************************************************
* 
***********************************************************************/
void rsx_device_fb_t::sub21ADAC() {
    S32 value = 0, size_MB;
    
    value = rsx_rd32(RSX_PFB_UNK0084);
    value &= 0xFFFFFFDF;                     // unset value[26:26]
    rsx_wr32(RSX_PFB_UNK0084, value);
    
    value = rsx_rd32(0x00100080);
    value |= 0x8000000;                      // set value[04:04]
    rsx_wr32(value, 0x00100080);
    
    rsx_wr32(0xCCCC5044, 0x28000100330);
    rsx_wr32(0xFFFF18FF, 0x2800010032C);
    rsx_wr32(0x0000440A, 0x28000100350);
    
    value = rsx_rd32(0x0010033C);
    value &= 0x000080FF;                     // unset value[16:16], value[24:31]
    value |= 0x00041200;                     // set value[13:13], value[19:19], value[22.22]
    rsx_wr32(value, 0x0010033C);
    
    rsx_wr32(RSX_PFB_UNK0340, 0x000001FB);
    rsx_wr32(RSX_PFB_UNK0344, 0x00000000);
    rsx_wr32(RSX_PFB_UNK0348, 0x00000000);
    rsx_wr32(RSX_PFB_UNK0338, 0x0000000A);
    rsx_wr32(RSX_PFB_UNK0334, 0x33332022);
    rsx_wr32(RSX_PFB_UNK0354, 0x00000806);
    
    // get size in MB from frame buffer object
    size_MB = fb->size >> 20;
    
    switch(size_MB) {
        case 128:
          value = rsx_rd32(0x00088304);
          value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
          value |= 1;                          // set value[31:31]
          rsx_wr32(value, 0x00088304);
          break;
        case 256:
          value = rsx_rd32(0x00088304);
          value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
          value |= 2;                          // set value[30:30]
          rsx_wr32(value, 0x00088304);
          break;
        case 64:
          value = rsx_rd32(0x00088304);
          value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
          rsx_wr32(value, 0x00088304);
        default:
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
    }
    
    return;
}


void rsx_device_fb_t::init() {
    S32 value = 0, size, DDR_MB_CTags;
    rsx_core_device_t* core = nullptr;
    rsx_bus_ioif0_t* ioif0 = nullptr;
    
    //rsx_wr32(0x10000000, 0x2800010020C);  // Test
    size = rsx_rd32(RSX_PFB_FIFO_DATA);
    size &= RSX_PFB_FIFO_DATA_RAM_AMOUNT_MB_MASK;                // should be 0x10000000, (256 MB)
    
    // Get device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);

    addr = core->ioif0->get_bar1_addr();
    RSX_ASSERT(core->ioif0->get_bar1_size() == size);

    // load DDR_MB_CTags from BAR0, increment by 1 and store back
    DDR_MB_CTags = rsx_rd32(RSX_PFB_UNK0320);
    DDR_MB_CTags++;
    rsx_wr32(RSX_PFB_UNK0320, DDR_MB_CTags);
    
    sub21ADAC();    
}
