#ifndef __RSX_DEVICE_GRAPH_H__
#define __RSX_DEVICE_GRAPH_H__



//--- init -------------------------------------------------------------
void rsx_device_graph_21E9CC(int64_t *vtab);
void rsx_device_graph_21D2DC(rsx_graph_obj_t *graph);
void rsx_device_graph_21D350(rsx_graph_obj_t *graph);
int32_t rsx_device_graph_21EF60(int64_t *vtab);
void rsx_device_graph_21ED38(int64_t *vtab, int32_t offset, int32_t arg1, int32_t arg2);
void rsx_device_graph_21DA04(rsx_graph_obj_t *graph, int32_t idx);
int32_t rsx_device_graph_21E950(void);
int32_t rsx_device_graph_21D6C0(void);
void rsx_device_graph_21E95C(int64_t *vtab, int32_t *arg1, int32_t *arg2, int32_t *arg3, int32_t *arg4, int32_t *arg5);
void rsx_device_graph_21DD9C(rsx_graph_obj_t *graph);
void rsx_device_graph_21E93C(int64_t *vtab, int64_t *arg1, int32_t *arg2);
void rsx_device_graph_21D7A0(rsx_graph_obj_t *graph);
void rsx_device_graph_init(rsx_graph_obj_t *graph);
//----------------------------------------------------------------------



int64_t rsx_device_graph_get_graph_channel_address_by_id(rsx_graph_obj_t *graph, int32_t channel_id);
int32_t rsx_device_graph_21D224(rsx_graph_obj_t *graph, int32_t arg1);
void rsx_device_graph_21D054(rsx_graph_obj_t *graph);
void rsx_device_graph_21D038(rsx_graph_obj_t *graph);
void rsx_device_graph_21D01C(rsx_graph_obj_t *graph);


#endif // __RSX_DEVICE_GRAPH_H__
