/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "ioif0.h"

#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/mmio.h"

const char* rsx_bus_ioif0_get_class_name_string() {
    return "rsx ioif0 bus";
}

inline void rsx_bus_ioif0_t::enable_interrupt(U32 bits) {
    U32 reg = rsx_rd32(RSX_PFIFO_INTR_EN_0) | bits;
    rsx_wr32(RSX_PFIFO_INTR_EN_0, reg);
}


// Get members
S32 rsx_bus_ioif0_t::get_init_flag() {
    return init;
}
S32 rsx_bus_ioif0_t::get_chip_revision() {
    return chip_revision;
}
S64 rsx_bus_ioif0_t::get_bar0_addr() {
    return bar0_addr;
}
S32 rsx_bus_ioif0_t::get_bar0_size() {
    return bar0_size;
}
S64 rsx_bus_ioif0_t::get_bar1_addr() {
    return bar1_addr;
}
S32 rsx_bus_ioif0_t::get_bar1_size() {
    return bar1_2_size;
}
S64 rsx_bus_ioif0_t::get_bar2_addr() {
    return bar2_addr;
}
S32 rsx_bus_ioif0_t::get_bar2_size() {
    return bar1_2_size;
}
S32 rsx_bus_ioif0_t::get_unk_38() {
    return unk_38;
}

// Access BAR0
S32 rsx_bus_ioif0_t::bar0_read(S64 offset) {
    return rsx_rd32(bar0_addr + offset);
}
void rsx_bus_ioif0_t::bar0_write(S32 offset, S32 value) {
    rsx_wr32(value, bar0_addr + offset);
}

/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_t::sub217814() {
    
    U64 value1 = 0x94A1510, value2 = 0xEE6B28;
    S32  value;
    
    /* bulshit
    
    for (i = 0; i < 3; i++) {
        value1 >>= 1;
        value2 >>= 1;
        
        if ((value1 & 1) != 0)
          break;
    }
    printf("(0x%08X)\n", value1);
    
    
    
    
    U64 r6, r9, r11;
    U64 r0 = 0x66666667;
    U64 r10 = r0;
    
    
    
    r6 = value1 >>31;
    r0 = value1 * r0;
    r0 = r0 >>1;
    r0 = r0 - r6;
    r9 = r9 + r0;
    
    
    printf("(0x%016llX) (0x%016llX)\n", r9, value1);
    */
  
  
  
  
  
  
  
  
  
  
  
  
    // store the result of the shit above in BAR0
    rsx_wr32(value1, 0x28000009200);
    rsx_wr32(value2, 0x28000009210);
  
    // Set interrupts
    enable_interrupt(RSX_IOIF_INTR_EMDRWER);
    enable_interrupt(RSX_IOIF_INTR_EMWDSER);
    enable_interrupt(RSX_IOIF_INTR_EMWSER);
    enable_interrupt(RSX_IOIF_INTR_EMWDTE);
    enable_interrupt(RSX_IOIF_INTR_UC_ATE);
    enable_interrupt(RSX_IOIF_INTR_UC_ERII);
    enable_interrupt(RSX_IOIF_INTR_UC_ERIT);
    enable_interrupt(RSX_IOIF_INTR_UC_IOCPLTO);
    enable_interrupt(RSX_IOIF_INTR_UC_IOODTO);
    enable_interrupt(RSX_IOIF_INTR_UC_IOCTO);
    enable_interrupt(RSX_IOIF_INTR_UC_RMRDSER);
    enable_interrupt(RSX_IOIF_INTR_UC_RMRSER);
    enable_interrupt(RSX_IOIF_INTR_UC_RMRDTE);
    enable_interrupt(RSX_IOIF_INTR_DEBUG_ERROR);
    enable_interrupt(RSX_IOIF_INTR_SPI_ERROR);
    enable_interrupt(RSX_IOIF_INTR_BE_READ_ERROR);
    enable_interrupt(RSX_IOIF_INTR_BE_WRITE_ERROR);
    enable_interrupt(RSX_IOIF_INTR_BE_COMMAND_OVERFLOW);
    enable_interrupt(RSX_IOIF_INTR_BE_DATA_OVERFLOW);
    enable_interrupt(RSX_IOIF_INTR_INVALID_DATA_OVERFLOW);
    enable_interrupt(RSX_IOIF_INTR_DATA_CREDIT_OVERFLOW);
    enable_interrupt(RSX_IOIF_INTR_COMMAND_CREDIT_OVERFLOW);
    enable_interrupt(RSX_IOIF_INTR_COMMAND_BUFFER_OVERFLOW);
    enable_interrupt(RSX_IOIF_INTR_INVALID_PLG_TYPE);
    enable_interrupt(RSX_IOIF_INTR_INVALID_NODE_ID);
    enable_interrupt(RSX_IOIF_INTR_INVALID_DSIZE);
    enable_interrupt(RSX_IOIF_INTR_INVALID_TTYPE);
    enable_interrupt(RSX_IOIF_INTR_CRC_ERROR);
    enable_interrupt(RSX_IOIF_INTR_RETRY_ERROR);
    enable_interrupt(RSX_IOIF_INTR_CONSECUTIVE_RETRY_ERROR);
}



/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_t::sub219018() {
    S32 i, value;
    S64 addr = 0;
    
    
    // set some flags in BAR0
    value = rsx_rd32(0x00088208);
    value |= 2;                            // set value[30:30]
    rsx_wr32(value, 0x00088208);
    
    value = rsx_rd32(0x00088208);
    value |= 1;                            // set value[31:31]
    rsx_wr32(value, 0x00088208);
    
    rsx_wr32(0, 0x2800008820C);          // set value to 0
    
    addr = 0x28000088280;
    rsx_wr32(1, addr);                   // set value[31:31]
    
    for (i = 1; i < 32; i++) {
        if (i < 23) {
            if (((1 << i) & 0x80) != 0) {
        rsx_wr32(0x301F001, addr + (i * 4));
            continue;
          }
      if (((1 << i) & 0xF64A00) != 0) {
              rsx_wr32(0x300F001, addr + (i * 4));
            continue;
          }
        }
        rsx_wr32(1, addr + (i * 4));
    }
    
    sub217814();
    
    value = rsx_rd32(0x000884C0);
    value |= 0x10;                         // set value[27:27]
    rsx_wr32(value, 0x000884C0);
    
    value = rsx_rd32(0x000884C0);
    value &= 0xFFFFFF1F;                   // unset value[24:26]
    rsx_wr32(value, 0x000884C0);
    
    value = rsx_rd32(0x00088208);
    value &= 0xFFFFF8FF;                   // unset value[21:23]
    value |= 0x100;                        // set value[23:23]
    rsx_wr32(value, 0x00088208);
  
    value = rsx_rd32(0x00088208);
    value &= 0xFFFF8FFF;                   // unset value[17:19]
    value |= 0x1000;                       // set value[19:19]
    rsx_wr32(value, 0x00088208);
  
    value = rsx_rd32(0x00088304);
    value |= 0x20000000;                   // set value[02:02]
    rsx_wr32(value, 0x00088304);
  
    value = rsx_rd32(0x00088304);
    value &= 1;                            // unset value[01:01]
    rsx_wr32(value, 0x00088304);
  
    return;
}



/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_t::set_chip_revision() {
    //S32 value1, value2;
    
    /*

    // read a value from 0x28000000004
    value1 = rsx_bus_ioif0_read_BAR0_by_offset(ioif0, 4);
    if (value1 != 0) {
        printf("rsx already booted\n");
        //...
        
    }
    else {
        value2 = rsx_bus_ioif0_read_BAR0_by_offset(ioif0, 0x88400);
        
        rsx_bus_ioif0_rsx_wr32_by_offset(ioif0, 4, 0x1000001);
        
        value1 = rsx_bus_ioif0_read_BAR0_by_offset(ioif0, 4);
        if ((value1 ^ 0x1000000) != 1) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
        
        
    }
    
    */
  
    chip_revision = 0x5C;
}




/***********************************************************************
* 
***********************************************************************/
rsx_bus_ioif0_t* rsx_bus_ioif0_init(void) {
    rsx_bus_ioif0_t* ioif0 = nullptr;

    // Check whether global RSX IOIF0 object already exists
    RSX_ASSERT(g_rsx_ioif_obj == nullptr);
    
    // allocate RSX IO interface 0 object
    ioif0 = lv1_kmalloc(sizeof(rsx_bus_ioif0_t));
    RSX_ASSERT(ioif0);
    
    // set members
    //ioif0->vtab = ;                      // useless in C
    ioif0->bar0_addr   = 0x28000000000;   // BAR0 start address
    ioif0->bar1_addr   = 0x28080000000;   // BAR1(VRAM) start address
    ioif0->bar2_addr   = 0x28002000000;   // BAR2(RAMIN) start address
    ioif0->bar0_size   = 0x2000000;       // BAR0 size, (32 MB)
    ioif0->bar1_2_size = 0x10000000;      // BAR1 and BAR2 size, (256 MB)
    ioif0->init        = 0;               // init flag unset
    ioif0->base_addr   = 0x28000000000;   
    
    // store global RSX IOIF0 object
    g_rsx_ioif_obj = (void*)ioif0;
    
    // invalidate a value in BAR0, init/reset ?
    rsx_wr32(0xFFFFFFFF, 0x28000088100);
    
    // get RSX chip revision
    ioif0->set_chip_revision();
    
    return ioif0;
}
