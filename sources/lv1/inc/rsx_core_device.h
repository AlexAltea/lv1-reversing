#ifndef __RSX_CORE_DEVICE_H__
#define __RSX_CORE_DEVICE_H__


int32_t rsx_core_device_map_device(rsx_dev_core_obj_t *core, int32_t device_id, int64_t *dev_lpar_addr, int64_t *dev_lpar_size);
void rsx_core_device_finalize(rsx_dev_core_obj_t *core);
int32_t rsx_core_device_close_device(int32_t dev_core_id);

rsx_dev_core_obj_t *rsx_core_device_get_core_object(void);
int32_t rsx_core_device_get_rsx_enable_flag(void);
rsx_dev_core_obj_t *rsx_core_device_get_core_object_by_id(int32_t dev_core_id);

int32_t rsx_core_device_open(int32_t arg1, int32_t dev_core_id);




#endif // __RSX_CORE_DEVICE_H__
