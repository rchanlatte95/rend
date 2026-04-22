#pragma once

#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <filesystem>
#include <string>

#include "rac.h"
#include "rac-mth.h"
#include "rac-types.h"
#include "rac-str.h"
#include "rac-clr.h"
#include "rac-io.h"
#include "rac-ppm.h"

#pragma warning(pop)

namespace rac::img
{
    using namespace rac::gfx;
    using namespace rac::string;
    using namespace rac::io;

    class PortableNetworkGraphic;
    typedef const PortableNetworkGraphic png;    typedef PortableNetworkGraphic mut_png;
    typedef const PortableNetworkGraphic* png_ptr;   typedef const PortableNetworkGraphic& png_ref;
    typedef PortableNetworkGraphic* mut_png_ptr; typedef PortableNetworkGraphic& mut_png_ref;

    extern SmallStaticStr PNG_FILE_EXT;

    class alignas(WIN_PAGE_SIZE) PortableNetworkGraphic
    {
    public:
        mut_color pixels[HEIGHT][WIDTH];

        PortableNetworkGraphic() { }
        PortableNetworkGraphic(color_ref init)
        {
            memset(pixels, (i32)init, PIXEL_BYTE_CT);
        }
        PortableNetworkGraphic(ppm_ref source)
        {
            memcpy(pixels, source.ToPtr(), PIXEL_BYTE_CT);
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

        bool ToFile(cstr filename) const;
    };
}
