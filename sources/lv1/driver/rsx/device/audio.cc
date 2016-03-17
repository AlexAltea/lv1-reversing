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
S32 rsx_device_audio_map_dev_1(rsx_dev_audio_obj_t* dev_audio, S64 *dev_lpar_addr, S64* dev_lpar_size) {
    
    
    
    
    
    printf("test ...\n");
    return 0;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_audio_init(rsx_dev_audio_obj_t* dev_audio) {
    S32 /*ret = -1,*/ driver_version = 0x211;              // RSX driver version
    S64 io_addr = 0;
    rsx_core_device_t* core = NULL;
    
    
    // get device core object
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // get device audio address, 0x2808FF20000
    io_addr = rsx_core_memory_get_mem_reg_addr_by_id((void*)core->core_mem_obj, 9);
    dev_audio->io_addr = io_addr;
    
    // get device audio size, 0x1000(4 KB)
    dev_audio->lpar_size = rsx_core_memory_get_mem_reg_size_by_id((void*)core->core_mem_obj, 9);
    
    // init RSX device audio LPAR address with 0
    dev_audio->lpar_addr = 0;           
    
    // set audio device driver version(0x211) into audio devive memory
  DDR_write32(driver_version, io_addr + 0x10);
    
    // copy device audio address
    dev_audio->io_addr_0 = dev_audio->io_addr;
    
    // set BAR0 register to 1
    write_BAR0(1, 0x28000080140);
    
    // TODO: register EIC interrupts 4 and 5?
    // many shit to do
    /*ret = rsx_device_eic_219C18(5, &dev_audio->unk_00);
    if (ret != LV1_SUCCESS) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    ret = rsx_device_eic_219C18(4, &dev_audio->unk_08);
    if (ret != LV1_SUCCESS) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }*/
    
    // TEST: fixed values from my dump
    dev_audio->unk_00 = 0x1A;
    dev_audio->unk_08 = 0x1B;
    
    // store interrupt IDs 4 and 5 into audio devive memory
    DDR_write32(dev_audio->unk_00, io_addr + 0x04);
    DDR_write32(dev_audio->unk_08, io_addr + 0x0C);
    
    return;
}



