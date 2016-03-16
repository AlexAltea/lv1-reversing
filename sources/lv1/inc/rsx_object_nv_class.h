#ifndef __RSX_OBJECT_NV_CLASS_H__
#define __RSX_OBJECT_NV_CLASS_H__


int32_t rsx_object_nv_class_get_object_size(void);
int64_t rsx_object_nv_class_get_BAR2_address(rsx_nv_class_obj_t *nv_obj);
uint32_t rsx_object_nv_class_get_object_class(rsx_nv_class_obj_t *nv_obj);
void rsx_object_nv_class_create_object(rsx_nv_class_obj_t *nv_obj, uint32_t arg1, uint32_t arg2);



#endif // __RSX_OBJECT_NV_CLASS_H__
