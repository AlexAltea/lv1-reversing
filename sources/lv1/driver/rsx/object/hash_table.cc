/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "hash_table.h"

#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/mmio.h"
#include "lv1/driver/rsx/core/device.h"

/*
class_type: e.g. 0xCAFEBABE, 0xFEED0001, 0xFEED0000, 0x66604200 to 0x6660420F
| Bits  | View (MSB:LSB)                      | Description                                 |
|-------|-------------------------------------|---------------------------------------------|
| 00:09 | XXXXXXXX XX000000 00000000 00000000 | 
| 10:19 | 00000000 00XXXXXX XXXX0000 00000000 | 
| 20:20 | 00000000 00000000 0000X000 00000000 | 
| 21:31 | 00000000 00000000 00000XXX XXXXXXXX | 
  

entry in hash table:





*/


/***********************************************************************
* rsx_hash_tbl_obj_t* hash_tbl = RSX obect hash table object to set up
***********************************************************************/
void rsx_object_hash_table_t::init() {
    S32 offset = 0, value;
    rsx_core_device_t* core = nullptr;
    rsx_core_memory_t* mem = nullptr;
    
    unk_00 = 0x1000;     // ?
    size_0 = 0x8000;     // ?
    
    
    // get device core object, we need IOIF0 next
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    RSX_ASSERT(core);
    
    mem = core->core_mem;
    // Get and store RSX hash table address, 0x28002010000
    io_addr = mem->get_mem_region_addr_by_id(2);  
    
    // Get and store RSX hash table size, 0x8000 (32 KB)
    size_1 = mem->get_mem_region_size_by_id(2);  
    
    // Get and store offset of the hash table, if offset is valid (not bigger than size)
    bar0_offset = rsx_core_memory_2120EC((void*)core->core_mem, io_addr);
    
    // check size
    if (size_1 < size_0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // check alignment
    if ((io_addr & 0x3FF) != 0) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // Initialize hash table with 0
    if (size_0 != 0)
        while (size_0 > offset) {
            DDR_write32(0, io_addr + offset);
            offset+=4;
        }
    }
    
    // Set hash table offset
    value = rsx_rd32(RSX_PFIFO_RAMHT);
    value &= 0xFFFFFE0F;
    value |= (bar0_offset >> 12);
    rsx_wr32(RSX_PFIFO_RAMHT, value);
    
    // Set hash table size
    value = rsx_rd32(RSX_PFIFO_RAMHT);
    value &= 0xFFFCFFFF;
    value |= 0x20000;
    rsx_wr32(RSX_PFIFO_RAMHT, value);
    
    // Clear hash table bits [05:07]
    value = rsx_rd32(RSX_PFIFO_RAMHT);
    value &= 0xF8FFFFFF;
    rsx_wr32(RSX_PFIFO_RAMHT, value);
    
    return;
}


void rsx_object_hash_table_t::finalize() {
    S32 value, offset = 0;

    // if table, clean table
    if (size_0 != 0)
        while (size_0 > offset) {
            DDR_write32(0, offset + io_addr);
            offset+=4;
        }
    }
    
    // Clear hash table size
    value = rsx_rd32(RSX_PFIFO_RAMHT);
    value &= 0xFFFFFE0F;                        // unset value[23:27]
    rsx_wr32(RSX_PFIFO_RAMHT, value);
    
    // Clear hash table offset
    value = rsx_rd32(RSX_PFIFO_RAMHT);
    value &= 0xFFFCFFFF;                        // unset value[14:15]
    rsx_wr32(RSX_PFIFO_RAMHT, value);
    
    // Clear hash table bits [05:07]
    value = rsx_rd32(RSX_PFIFO_RAMHT);
    value &= 0xF8FFFFFF;                        // unset value[05:07]
    rsx_wr32(RSX_PFIFO_RAMHT, value);
}




/***********************************************************************
* rsx_hash_tbl_obj_t* hash_tbl = RSX device core hash table object
* S32 channel_id           = channel ID of RSX context, 0 to 3(max)
* U32 class_type          = type of class object, sw, hv or dma
*                                sw  e.g. 0xCAFEBABE
*                                hv  e.g. 0x31337303, 0x3137C0DE, ...
*                                dma e.g. 0xFEED0001, 0xFEED0000, 0x66604200 to 0x6660420F, ...
* S32 offset               = 
* S32 arg1                 = ?, 1 or 2
***********************************************************************/
void rsx_object_hash_table_t::create_entry(S32 channel_id, U32 class_type, S32 offset, S32 arg1) {
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    S32 value, max_channels, entry_offset = 0;
    
    
    // core ID comes here from HSPRG context, I use 2, we know it is always 2
    core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // get channels bitmap from core object
    bm_channels = (void*)core->bm_obj_channels;
    
    // get total count of channels from channels bitmap
    max_channels = rsx_utils_bitmap_get_item_total(bm_channels);
    
    // check: channel ID out of range(0 to 3) ?
    if (max_channels <= channel_id) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // build entry offset:
    // class_type[10:19] XOR class_type[21:31] XOR (channel_id * 128) XOR class_type[00:09]
    entry_offset = (((class_type & 0x3FF800) >>11) ^ (class_type & 0x7FF) ^
                   ((channel_id & 7) <<7) ^ (class_type >>22)) << 3;
    
    entry_offset += bar0_offset;
    
    // ?
    if (arg1 == 1) {
        value = DDR_read32(entry_offset + g_rsx_bar2_addr + 4);
        value &= 0xFF8FFFFF;
        value |= 0x100000;
        DDR_write32(value, entry_offset + g_rsx_bar2_addr + 4);
    }
    else if (arg1 == 2) {
        value = DDR_read32(entry_offset + g_rsx_bar2_addr + 4);
        value &= 0xFF8FFFFF;
        DDR_write32(value, entry_offset + g_rsx_bar2_addr + 4);
    }
    else {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    }
    
    // store class type
    DDR_write32(class_type, entry_offset + g_rsx_bar2_addr);
    
    // prepare offset
    offset = rsx_core_memory_value_div_by_16((void*)core->core_mem, offset) & 0xFFFFF;
    
    // store offset into hash table entry
    value = DDR_read32(entry_offset + g_rsx_bar2_addr + 4);
    value &= 0xFFF00000;
    value |= offset;
    DDR_write32(value, entry_offset + g_rsx_bar2_addr + 4);
    
    // ?
    value = DDR_read32(entry_offset + g_rsx_bar2_addr + 4);
    value &= 0x7FFFFF;
    value |= (channel_id <<23);
    DDR_write32(value, entry_offset + g_rsx_bar2_addr + 4);
    
    return;
}

/***********************************************************************
* rsx_hash_tbl_obj_t* hash_tbl = RSX device core hash table object
* S32 channel_id           = channel ID of RSX context, 0 to 3(max)
* U32 class_type          = type of class object, sw, hv or dma
*                                sw  e.g. 0xCAFEBABE
*                                hv  e.g. 0x31337303, 0x3137C0DE, ...
*                                dma e.g. 0xFEED0001, 0xFEED0000, 0x66604200 to 0x6660420F, ...
***********************************************************************/
void rsx_object_hash_table_t::invalidate_entry(S32 channel_id, U32 class_type) {
    rsx_core_device_t* core = NULL;
    rsx_utils_bitmap_t* bm_channels = NULL;
    S32 value, max_channels, entry_offset = 0;
    
    
    // core ID comes here from HSPRG context, I use 2, we know it is always 2
    core = rsx_core_device_get_core_object_by_id(2);
    if (core == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // get channels bitmap from core object
    bm_channels = (void*)core->bm_obj_channels;
    
    // get total count of channels from channels bitmap
    max_channels = rsx_utils_bitmap_get_item_total(bm_channels);
    
    // check: channel ID out of range(0 to 3) ?
    if (max_channels <= channel_id) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
    
    // build entry offset
    // class_type[10:19] XOR class_type[21:31] XOR (channel_id * 128) XOR class_type[00:09]
    entry_offset = (((class_type & 0x3FF800) >>11) ^ (class_type & 0x7FF) ^
                   ((channel_id & 7) <<7) ^ (class_type >>22)) << 3;
    
    entry_offset += bar0_offset;
    
    // no idea why, old value read but not used
    value = DDR_read32(entry_offset + g_rsx_bar2_addr);
    
    // delete entry:class_type
    DDR_write32(0, entry_offset + g_rsx_bar2_addr);
    
    // delete entry:offset
    value = DDR_read32(entry_offset + g_rsx_bar2_addr + 4);
    value &= 0xFFF00000;
    DDR_write32(value, entry_offset + g_rsx_bar2_addr + 4);
    
    // ? channel
    value = DDR_read32(entry_offset + g_rsx_bar2_addr + 4);
    value &= 0x3FFFFF;
    value |= 0x7F800000;
    DDR_write32(value, entry_offset + g_rsx_bar2_addr + 4);
    
    return;
}
