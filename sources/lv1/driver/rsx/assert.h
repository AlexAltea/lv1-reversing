/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#define RSX_ASSERT_LIGHT(condition) \
    if (!(condition)) { printf("RSX driver assert failed: [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__); return; }

#define RSX_ASSERT_STRONG(condition) \
    assert_true((condition), "RSX driver assert failed")

#define RSX_ASSERT RSX_ASSERT_LIGHT
