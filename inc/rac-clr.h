#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <float.h>
#include <math.h>

#include "rac-mth.h"
#include "rac-types.h"
#include "rac-str.h"

#pragma warning(pop)

namespace rac::gfx
{
    using namespace rac::mth;

    class RAC_Color;
    typedef const RAC_Color color;      typedef RAC_Color mut_color;
    typedef const RAC_Color* color_ptr; typedef const RAC_Color& color_ref;
    typedef RAC_Color* mut_color_ptr;   typedef RAC_Color& mut_color_ref;

    // these factors are grabbed from: https://en.wikipedia.org/wiki/Luma_(video)
    // under the section: Rec. 601 luma versus Rec. 709 luma coefficients
    // I am assuming most monitors in circulation are digital HD monitors.
    #define LUMA_REC709_R 0.2126f
    #define LUMA_REC709_G 0.7152f
    #define LUMA_REC709_B 0.0722f

    u8 MAX_COLOR_COMPONENT_VALUE = 255;
    u8 MIN_COLOR_COMPONENT_VALUE = 0;

    #define BYTE_DIGIT_CT 3
    i32 BYTE_STR_MAX = BYTE_DIGIT_CT + 1;
    i32 BYTE_STR_LEN = BYTE_DIGIT_CT;
    i32 COLOR_STRING_MAX = (BYTE_STR_LEN * 4) + (COMMA_SPACE_LEN * 3) + PARENTHESES_LEN + 2;
    i32 COLOR_STRING_LEN = COLOR_STRING_MAX - 1;
    i32 PPM_STRING_MAX = (BYTE_STR_LEN * 3) + (SPACE_CHAR_LEN * 3) + 1;
    i32 PPM_STRING_LEN = PPM_STRING_MAX - 1;
    f32 GAMMA = 2.2222222f;
    f32 INV_GAMMA = 1.0f / GAMMA;
    class alignas(4) RAC_Color
    {
    public:

        mut_ubyte b;
        mut_ubyte g;
        mut_ubyte r;
        mut_ubyte opacity;

        RAC_Color() { }
        RAC_Color(u8 _r, u8 _g, u8 _b, u8 _a = 255)
        {
            r = _r;
            g = _g;
            b = _b;
            opacity = _a;
        }
        RAC_Color(u16 _r, u16 _g, u16 _b, u16 _a = 255)
        {
            r = (u8)_r;
            g = (u8)_g;
            b = (u8)_b;
            opacity = (u8)_a;
        }
        RAC_Color(u32 _r, u32 _g, u32 _b, u32 _a = 255)
        {
            r = (u8)_r;
            g = (u8)_g;
            b = (u8)_b;
            opacity = (u8)_a;
        }
        RAC_Color(f32 _r, f32 _g, f32 _b, f32 _a = 1.0f)
        {
            f32 ceil_ = 255.999f;
            r = (u8)(_r * ceil_);
            g = (u8)(_g * ceil_);
            b = (u8)(_b * ceil_);
            opacity = (u8)(_a * ceil_);
        }

        INLINE u32 GetU32() const noexcept { return *(u32ptr(&opacity)); }
        INLINE i32 GetI32() const noexcept { return *(i32ptr(&opacity)); }

        INLINE f32 LinearToGamma(f32 linear_color_component) const noexcept
        {
            return powf(NormU8(linear_color_component), INV_GAMMA);
        }

        INLINE f32 GammaToLinear(f32 gamma_color_component) const noexcept
        {
            return powf(NormU8(gamma_color_component), GAMMA);
        }

        INLINE color Luminance() const noexcept
        {
            f32 ceil_ = 255.999f;

            f32 rY = LUMA_REC709_R * LinearToGamma(r);
            f32 gY = LUMA_REC709_G * LinearToGamma(g);
            f32 bY = LUMA_REC709_B * LinearToGamma(b);
            return color((u8)(rY * ceil_),
                        (u8)(gY * ceil_),
                        (u8)(bY * ceil_),
                        opacity);
        }

        operator u32() const noexcept { return GetU32(); }
        operator i32() const noexcept { return GetI32(); }

        INLINE color_ref operator=(color_ref rhs)
        {
            b = rhs.b;
            g = rhs.g;
            r = rhs.r;
            opacity = rhs.opacity;
            return *this;
        }

        INLINE string::StrRef ToStr(string::mut_StrRef str) const
        {
            char buff[COLOR_STRING_MAX] = { 0 };
            sprintf_s(buff, COLOR_STRING_LEN, "(%u, %u, %u, %u)", r, g, b, opacity);
            return str = buff;
        }
    };

    color WHITE(MAX_COLOR_COMPONENT_VALUE, MAX_COLOR_COMPONENT_VALUE, MAX_COLOR_COMPONENT_VALUE);
    color BLACK(MIN_COLOR_COMPONENT_VALUE, MIN_COLOR_COMPONENT_VALUE, MIN_COLOR_COMPONENT_VALUE);
    color RED(MAX_COLOR_COMPONENT_VALUE, MIN_COLOR_COMPONENT_VALUE, MIN_COLOR_COMPONENT_VALUE);
    color GREEN(MIN_COLOR_COMPONENT_VALUE, MAX_COLOR_COMPONENT_VALUE, MIN_COLOR_COMPONENT_VALUE);
    color BLUE(MIN_COLOR_COMPONENT_VALUE, MIN_COLOR_COMPONENT_VALUE, MAX_COLOR_COMPONENT_VALUE);

    INLINE static bool operator >(color_ref lhs, color_ref rhs)
    {
        return  *u32ptr(lhs.b) > *u32ptr(rhs.b);
    }
    INLINE static bool operator <(color_ref lhs, color_ref rhs)
    {
        return *u32ptr(lhs.b) < *u32ptr(rhs.b);
    }
    INLINE static bool operator >=(color_ref lhs, color_ref rhs)
    {
        return *u32ptr(lhs.b) >= *u32ptr(rhs.b);
    }
    INLINE static bool operator <=(color_ref lhs, color_ref rhs)
    {
        return *u32ptr(lhs.b) <= *u32ptr(rhs.b);
    }
    INLINE static bool operator ==(color_ref lhs, color_ref rhs)
    {
        return  *u32ptr(lhs.b) == *u32ptr(rhs.b);
    }
    INLINE static bool operator !=(color_ref lhs, color_ref rhs)
    {
        return !(lhs == rhs);
    }
}