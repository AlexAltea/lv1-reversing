/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "common/types.h"

// LV1 Configuration
#define LV1_CONFIG_SYS_BOOT_GOS         0                     // boot graphical OS ? 0(yes)
#define LV1_CONFIG_SYS_LV1_RSXENABLE    3                     // ?
#define LV1_CONFIG_SYS_LV1_RSXDEBUG     0                     // debuging level, 0(off), ..., 3(max debug output)
#define LV1_CONFIG_SYS_AC_MISC2         0x1000000000000001ULL // ?
#define LV1_CONFIG_SYS_RDCY_2           0xFFFFFFFFFFFFFFFFULL // ?
#define LV1_CONFIG_BE_0_TB_CLOCK        0x0000000004C1A6C0ULL // CELL BE clock

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
