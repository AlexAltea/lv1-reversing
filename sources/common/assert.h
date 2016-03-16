/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/macro.h"

#include <cassert>

// Trigger an exception if expression is false
#define assert_true(...) \
    __LV1RE_MACRO_DISPATCH(assert_true, __VA_ARGS__)(__VA_ARGS__)
#define assert_true1(expr) assert(expr)
#define assert_true2(expr, message) assert((expr) && (message))

// Trigger an exception if expression is true
#define assert_false(...) \
    __LV1RE_MACRO_DISPATCH(assert_false, __VA_ARGS__)(__VA_ARGS__)
#define assert_false1(expr) assert(!(expr))
#define assert_false2(expr, message) assert(!(expr) && (message))

// Trigger an exception if expression is zero
#define assert_zero(...) \
    __LV1RE_MACRO_DISPATCH(assert_zero, __VA_ARGS__)(__VA_ARGS__)
#define assert_zero1(expr) assert(expr)
#define assert_zero2(expr, message) assert(((expr) == 0) && (message))

// Trigger an exception if expression is non-zero
#define assert_nonzero(...) \
    __LV1RE_MACRO_DISPATCH(assert_true, __VA_ARGS__)(__VA_ARGS__)
#define assert_nonzero1(expr) assert_nonzero(expr)
#define assert_nonzero2(expr, message) assert(((expr) != 0) && (message))

// Trigger an exception if expression is a nullptr
#define assert_null(...) \
    __LV1RE_MACRO_DISPATCH(assert_null, __VA_ARGS__)(__VA_ARGS__)
#define assert_null1(expr) assert(expr)
#define assert_null2(expr, message) assert(((expr) == nullptr) && (message))

// Trigger an exception if expression is not a nullptr
#define assert_nonnull(...) \
    __LV1RE_MACRO_DISPATCH(assert_nonnull, __VA_ARGS__)(__VA_ARGS__)
#define assert_nonnull1(expr) assert(expr)
#define assert_nonnull2(expr, message) assert(((expr) != nullptr) && (message))

// Trigger an exception
#define assert_always(...) \
    __LV1RE_MACRO_DISPATCH(assert_always, __VA_ARGS__)(__VA_ARGS__)
#define assert_always0(...) assert(0)
#define assert_always1(message) assert(0 && (message))
