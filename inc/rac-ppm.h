#pragma once

#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <filesystem>
#include <string>

#include <shlobj.h>

#pragma comment(lib,"Shell32")
#pragma comment(lib,"Ole32")

#include "rac.h"
#include "rac-mth.h"
#include "rac-types.h"
#include "rac-str.h"
#include "rac-clr.h"
#include "rac-io.h"

#pragma warning(pop)

namespace rac::img
{
    using namespace rac::gfx;
    using namespace rac::string;
    using namespace rac::io;

    class PortablePixelMap;
    typedef const PortablePixelMap ppm;    typedef PortablePixelMap mut_ppm;
    typedef const PortablePixelMap* ppm_ptr;   typedef const PortablePixelMap& ppm_ref;
    typedef PortablePixelMap* mut_ppm_ptr; typedef PortablePixelMap& mut_ppm_ref;

    inline constexpr i32 WIDTH = 1536;
    inline constexpr i32 HEIGHT = 1024;
    inline constexpr i32 PENULT_WIDTH = WIDTH - 1;
    inline constexpr i32 PENULT_HEIGHT = HEIGHT - 1;
    inline constexpr i32 PIXEL_CT = WIDTH * HEIGHT;
    inline constexpr i32 PIXEL_BYTE_CT = PIXEL_CT * sizeof(color);
    inline constexpr u64 PPM_HEADER_STR_CAP = 18;
    inline constexpr u64 PPM_HEADER_STR_LEN = PPM_HEADER_STR_CAP - 1;
    inline constexpr f32 PPM_ASPECT_RATIO = (f32)WIDTH / (f32)HEIGHT;
    inline constexpr f32 INV_PIXEL_CT = 1.0f / (float)PIXEL_CT;
    extern SmallStaticStr PPM_FILE_EXT;

    class alignas(WIN_PAGE_SIZE) PortablePixelMap
    {
    public:
        mut_color pixels[HEIGHT][WIDTH];

        PortablePixelMap() { }
        PortablePixelMap(color_ref init)
        {
            memset(pixels, (i32)init, PIXEL_BYTE_CT);
        }

        INLINE ptr ToPtr() const noexcept { return (ptr)pixels; }
        INLINE color_ptr ToColorPtr() const noexcept { return (color_ptr)pixels; }
        INLINE mut_color_ref operator() (u32 x, u32 y) noexcept { return pixels[y][x]; }
        INLINE color_ref operator[] (u32 idx) const noexcept
        {
            return *(&(pixels[0][0]) + idx);
        }

        INLINE void SetPixelColor(i32 x, i32 y, color_ref new_color) noexcept
        {
            pixels[y][x].r = new_color.r;
            pixels[y][x].g = new_color.g;
            pixels[y][x].b = new_color.b;
            pixels[y][x].opacity = new_color.opacity;
        }

        bool DBG_ToFile(cstr filename) const;
        bool ToFile(cstr filename) const;
    };
}