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
    class Colour;
    typedef const Colour color;    typedef Colour mut_color;
    typedef const Colour* color_ptr;   typedef const Colour& color_ref;
    typedef Colour* mut_color_ptr; typedef Colour& mut_color_ref;

    #define BYTE_DIGIT_CT 3
    i32 BYTE_STR_MAX = BYTE_DIGIT_CT + 1;
    i32 BYTE_STR_LEN = BYTE_DIGIT_CT;
    i32 COLOR_STRING_MAX = (BYTE_STR_LEN * 4) + (COMMA_SPACE_LEN * 3) + PARENTHESES_LEN + 2;
    i32 COLOR_STRING_LEN = COLOR_STRING_MAX - 1;

    i32 PPM_STRING_MAX = (BYTE_STR_LEN * 3) + (SPACE_CHAR_LEN * 2) + 1;
    i32 PPM_STRING_LEN = PPM_STRING_MAX - 1;
    class Colour
    {
    public:
        union
        {
            mut_u32 col = 0;
            mut_ubyte a;
            mut_ubyte r;
            mut_ubyte g;
            mut_ubyte b;
        };

        Colour() { }
        Colour(u8 _r, u8 _g, u8 _b, u8 _a = 255)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }
        Colour(u16 _r, u16 _g, u16 _b, u16 _a = 255)
        {
            r = (u8)_r;
            g = (u8)_g;
            b = (u8)_b;
            a = (u8)_a;
        }
        Colour(u32 _r, u32 _g, u32 _b, u32 _a = 255)
        {
            r = (u8)_r;
            g = (u8)_g;
            b = (u8)_b;
            a = (u8)_a;
        }
        Colour(u32 full_color) { col = full_color; }

        INLINE string::StrRef ToStr(string::mut_StrRef str) const
        {
            char buff[COLOR_STRING_MAX] = { 0 };
            sprintf_s(buff, COLOR_STRING_LEN, "(%u, %u, %u, %u)", r, g, b, a);
            return str = buff;
        }

        INLINE string::StrRef ToPpmStr(string::mut_StrRef str) const
        {
            char buff[PPM_STRING_MAX] = { 0 };
            sprintf_s(buff, PPM_STRING_LEN, "%u %u %u", r, g, b);
            return str = buff;
        }
    };
}