#ifndef __RSX_LV1_H__
#define __RSX_LV1_H__


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



// LV1 return codes
#define LV1_SUCCESS                       (0)
#define LV1_RESOURCE_SHORTAGE            (-2)
#define LV1_NO_PRIVILEGE                 (-3)
#define LV1_DENIED_BY_POLICY             (-4)
#define LV1_ACCESS_VIOLATION             (-5)
#define LV1_NO_ENTRY                     (-6)
#define LV1_DUPLICATE_ENTRY              (-7)
#define LV1_TYPE_MISMATCH                (-8)
#define LV1_BUSY                         (-9)
#define LV1_EMPTY                       (-10)
#define LV1_WRONG_STATE                 (-11)
#define LV1_NO_MATCH                    (-13)
#define LV1_ALREADY_CONNECTED           (-14)
#define LV1_UNSUPPORTED_PARAMETER_VALUE (-15)
#define LV1_CONDITION_NOT_SATISFIED     (-16)
#define LV1_ILLEGAL_PARAMETER_VALUE     (-17)
#define LV1_BAD_OPTION                  (-18)
#define LV1_IMPLEMENTATION_LIMITATION   (-19)
#define LV1_NOT_IMPLEMENTED             (-20)
#define LV1_INVALID_CLASS_ID            (-21)
#define LV1_CONSTRAINT_NOT_SATISFIED    (-22)
#define LV1_ALIGNMENT_ERROR             (-23)
#define LV1_HARDWARE_ERROR              (-24)
#define LV1_INVALID_DATA_FORMAT         (-25)
#define LV1_INVALID_OPERATION           (-26)
#define LV1_INTERNAL_ERROR           (-32768)



// repo node values, rsx related
#define SYS_BOOT_GOS         0                     // boot graphical OS ? 0(yes)
#define SYS_LV1_RSXENABLE    3                     // ?
#define SYS_LV1_RSXDEBUG     0                     // debuging level, 0(off), ..., 3(max debug output)
#define SYS_AC_MISC2         0x1000000000000001ULL // ?
#define SYS_RDCY_2           0xFFFFFFFFFFFFFFFFULL // ?
#define BE_0_TB_CLOCK        0x0000000004C1A6C0ULL // CELL BE clock







////////////////////////////////////////////////////////////////////////
// hypercalls
void lv1_gpu_open(int32_t arg1);
void lv1_gpu_close(void);
void lv1_gpu_device_map(int32_t device_id);
void lv1_gpu_memory_allocate(int32_t local_size, int64_t arg4, int64_t arg5, int64_t arg6, int64_t arg7);
void lv1_gpu_context_allocate(uint32_t mem_ctx_id, uint64_t system_mode);
void lv1_gpu_context_iomap(uint32_t rsx_ctx_id, uint64_t arg1, int64_t lpar_addr, int32_t size, uint64_t flags);

void lv1_test(void);



#endif // __RSX_LV1_H__
