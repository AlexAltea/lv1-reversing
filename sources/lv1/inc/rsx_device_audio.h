#ifndef __RSX_DEVICE_AUDIO_H__
#define __RSX_DEVICE_AUDIO_H__


int32_t rsx_device_audio_map_dev_1(rsx_dev_audio_obj_t *dev_audio, int64_t *dev_lpar_addr, int64_t* dev_lpar_size);
void rsx_device_audio_init(rsx_dev_audio_obj_t *dev_audio);


#endif // __RSX_DEVICE_AUDIO_H__
