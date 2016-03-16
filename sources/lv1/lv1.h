/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "common/types.h"

#include "../../memory.h"
#include "lv1_misc.h"
#include "rsx_object.h"

#include "rsx_utils_debugger.h"
#include "rsx_utils_bitmap.h"
#include "rsx_bus_ioif0.h"
#include "rsx_device_clock.h"
#include "rsx_device_eic.h"
#include "rsx_device_master.h"
#include "rsx_device_fb.h"
#include "rsx_device_fifo.h"
#include "rsx_device_graph.h"
#include "rsx_device_audio.h"
#include "rsx_core_device.h"
#include "rsx_core_memory.h"
#include "rsx_core_context.h"
#include "rsx_object_hash_table.h"
#include "rsx_object_channel.h"
#include "rsx_object_context_dma.h"
#include "rsx_object_nv_class.h"
#include "rsx_object_sw_class.h"
#include "rsx_object_video_rsx.h"
#include "rsx_object_vfb.h"



// LV1 Return Codes
enum {
    LV1_SUCCESS                      = 0,
    LV1_RESOURCE_SHORTAGE            = -2,
    LV1_NO_PRIVILEGE                 = -3,
    LV1_DENIED_BY_POLICY             = -4,
    LV1_ACCESS_VIOLATION             = -5,
    LV1_NO_ENTRY                     = -6,
    LV1_DUPLICATE_ENTRY              = -7,
    LV1_TYPE_MISMATCH                = -8,
    LV1_BUSY                         = -9,
    LV1_EMPTY                        = -10,
    LV1_WRONG_STATE                  = -11,
    LV1_NO_MATCH                     = -13,
    LV1_ALREADY_CONNECTED            = -14,
    LV1_UNSUPPORTED_PARAMETER_VALUE  = -15,
    LV1_CONDITION_NOT_SATISFIED      = -16,
    LV1_ILLEGAL_PARAMETER_VALUE      = -17,
    LV1_BAD_OPTION                   = -18,
    LV1_IMPLEMENTATION_LIMITATION    = -19,
    LV1_NOT_IMPLEMENTED              = -20,
    LV1_INVALID_CLASS_ID             = -21,
    LV1_CONSTRAINT_NOT_SATISFIED     = -22,
    LV1_ALIGNMENT_ERROR              = -23,
    LV1_HARDWARE_ERROR               = -24,
    LV1_INVALID_DATA_FORMAT          = -25,
    LV1_INVALID_OPERATION            = -26,
    LV1_INTERNAL_ERROR               = -32768,
};



// repo node values, rsx related
#define SYS_BOOT_GOS         0                     // boot graphical OS ? 0(yes)
#define SYS_LV1_RSXENABLE    3                     // ?
#define SYS_LV1_RSXDEBUG     0                     // debuging level, 0(off), ..., 3(max debug output)
#define SYS_AC_MISC2         0x1000000000000001ULL // ?
#define SYS_RDCY_2           0xFFFFFFFFFFFFFFFFULL // ?
#define BE_0_TB_CLOCK        0x0000000004C1A6C0ULL // CELL BE clock



void lv1_test(void);
