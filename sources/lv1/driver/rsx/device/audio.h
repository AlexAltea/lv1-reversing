/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#ifndef __RSX_DEVICE_AUDIO_H__
#define __RSX_DEVICE_AUDIO_H__


S32 rsx_device_audio_map_dev_1(rsx_dev_audio_obj_t* dev_audio, S64 *dev_lpar_addr, S64* dev_lpar_size);
void rsx_device_audio_init(rsx_dev_audio_obj_t* dev_audio);


#endif // __RSX_DEVICE_AUDIO_H__
