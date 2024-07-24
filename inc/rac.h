#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <float.h>
#include <math.h>
#include <new>
#include <thread>

#include "rac-types.h"

#pragma warning(pop)

namespace rac
{
    u32 CACHELINE_SIZE = (u32)std::hardware_destructive_interference_size;
    u32 FALSE_SHARE_CACHE_SIZE = (u32)std::hardware_destructive_interference_size;
    u32 TRUE_SHARE_CACHE_SIZE = (u32)std::hardware_constructive_interference_size;
    u32 MAX_THREAD_CT = std::thread::hardware_concurrency();

    f32 PI = 3.14159265358979323846f;
    f32 INV_PI = 1.0f / PI;
    f32 SIXTH_PI = PI / 6.0f;
    f32 FOURTH_PI = PI / 4.0f;
    f32 THIRD_PI = PI / 3.0f;
    f32 HALF_PI = PI / 2.0f;
    f32 TAU = 2.0f * PI;

    f32 DEG2RAD = PI / 180.0f;
    f32 RAD2DEG = 180.0f / PI;

    f32 INV_U8_MAX = 1.0f / (f32)UINT8_MAX;
    f32 INV_U16_MAX = 1.0f / (f32)UINT16_MAX;
    f32 INV_U32_MAX = 1.0f / (f32)UINT32_MAX;
    f32 INV_U64_MAX = 1.0f / (f32)UINT64_MAX;

    u8 ABS_I8_MIN = -(INT8_MIN);
    u16 ABS_I16_MIN = -(INT16_MIN);
    u32 ABS_I32_MIN = -(INT32_MIN);
    u64 ABS_I64_MIN = -(INT64_MIN);

    f32 F32_EPSILON = FLT_EPSILON;
    f32 F32_MAX = FLT_MAX;
    f32 F32_MIN = FLT_MIN;

    f32 SIGNED_F32_EPSILON = -F32_EPSILON;
    f32 F32_ONE_EPSILON = 1.0f - F32_EPSILON;
    f32 SIGNED_F32_ONE_EPSILON = -F32_ONE_EPSILON;
}