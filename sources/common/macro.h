/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

// Count arguments
#define __LV1RE_ARGUMENT_EXPAND(x) x
#define __LV1RE_ARGUMENT_COUNT(...) \
    __LV1RE_ARGUMENT_EXPAND(__LV1RE_ARGUMENT_EXTRACT(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define __LV1RE_ARGUMENT_EXTRACT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, N, ...) N

// Dispatching macros
#define __LV1RE_MACRO_DISPATCH(function, ...) \
    __LV1RE_MACRO_SELECT(function, __LV1RE_ARGUMENT_COUNT(__VA_ARGS__))
#define __LV1RE_MACRO_SELECT(function, argc) \
    __LV1RE_MACRO_CONCAT(function, argc)
#define __LV1RE_MACRO_CONCAT(a, b) a##b
