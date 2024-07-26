#pragma once

#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <filesystem>
#include <string>

#include <windows.h>
#include <shlobj.h>
#include <objbase.h>

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

    i32 WIDTH = 1536;
    i32 HEIGHT = 1024;
    i32 PENULT_WIDTH = WIDTH - 1;
    i32 PENULT_HEIGHT = HEIGHT - 1;
    i32 PIXEL_CT = WIDTH * HEIGHT;
    i32 PIXEL_BYTE_CT = PIXEL_CT * sizeof(color);
    u64 PPM_HEADER_STR_CAP = 18;
    u64 PPM_HEADER_STR_LEN = PPM_HEADER_STR_CAP - 1;
    f32 PPM_ASPECT_RATIO = (f32)WIDTH / (f32)HEIGHT;
    f32 INV_PIXEL_CT = 1.0f / (float)PIXEL_CT;
    SmallStaticStr PPM_FILE_EXT = ".ppm";

    class alignas(WIN_PAGE_SIZE) PortablePixelMap
    {
    public:
        mut_color pixels[HEIGHT][WIDTH];

        PortablePixelMap() { }
        //PortablePixelMap(color_ref init) { memset(pixels, (i32)init, PIXEL_BYTE_CT); }
        PortablePixelMap(color_ref init) { memset(pixels, (i32)init, sizeof(pixels)); }

        INLINE ptr ToPtr() const noexcept { return (ptr)pixels; }
        INLINE color_ptr ToColorPtr() const noexcept { return (color_ptr)pixels; }
        INLINE mut_color_ref operator() (u32 row, u32 col) noexcept
        {
            return pixels[row][col];
        }
        INLINE color_ref operator[] (u32 idx) const noexcept
        {
            return *(&(pixels[0][0]) + idx);
        }

        MAY_INLINE bool DBG_ToFile(cstr filename) const
        {
            std::string pathStr = GetDesktopPathStr() + '\\';
            pathStr = pathStr + filename;
            pathStr += PPM_FILE_EXT;

            mut_FileHandle file = nullptr;
            fopen_s(&file, pathStr.c_str(), "w");
            if (file == nullptr) { return false; }

            mut_i64 writeRes = fprintf_s(file, "P3\n%u %u\n255\n", WIDTH, HEIGHT);
            f32 WIDTH_FACTOR = 255.999f / PENULT_WIDTH;
            f32 HEIGHT_FACTOR = 255.999f / PENULT_HEIGHT;
            mut_color_ptr c = (mut_color_ptr)(pixels - 1);
            mut_i32 pixels_done = 0;
            mut_f32 pct_done = (pixels_done * INV_PIXEL_CT) * 100.0f;
            for (int y = 0; y < HEIGHT; ++y)
            {
                ++c;
                for (int x = 0; x < PENULT_WIDTH; ++x)
                {
                    c->r = ubyte((float(x) * WIDTH_FACTOR));
                    c->g = ubyte((float(y) * HEIGHT_FACTOR));
                    c->b = 0;
                    writeRes += fprintf_s(file, "%u %u %u ", c->r, c->g, c->b);
                }
                c->r = ubyte((float(PENULT_WIDTH) * WIDTH_FACTOR));
                c->g = ubyte((float(y) * HEIGHT_FACTOR));
                c->b = 0;
                writeRes += fprintf_s(file, "%u %u %u\n", c->r, c->g, c->b);

                pixels_done += WIDTH;
                pct_done = (pixels_done * INV_PIXEL_CT) * 100.0f;
                Console::ClearLine();
                printf("%f%% -> %d Pixels done out of %d", pct_done, pixels_done, PIXEL_CT);
            }
            i32 closeResult = fclose(file);
            return closeResult >= 0 && writeRes >= 0;
        }

        MAY_INLINE bool ToFile(cstr filename) const
        {
            std::string pathStr = io::GetDesktopPathStr() + '\\';
            pathStr = pathStr + filename;
            pathStr += PPM_FILE_EXT;

            mut_FileHandle file = nullptr;
            fopen_s(&file, pathStr.c_str(), "w");
            if (file == nullptr) { return false; }

            mut_i64 writeRes = fprintf_s(file, "P3\n%u %u\n255\n", WIDTH, HEIGHT);
            mut_color_ptr c = (mut_color_ptr)(pixels - 1);
            mut_i32 pixels_done = 0;
            mut_f32 pct_done = 0.0f;
            for (int y = 0; y < HEIGHT; ++y)
            {
                ++c;
                for (int x = 0; x < PENULT_WIDTH; ++x)
                {
                    writeRes += fprintf_s(file, "%u %u %u ", c->r, c->g, c->b);
                }
                writeRes += fprintf_s(file, "%u %u %u\n", c->r, c->g, c->b);

                pixels_done += WIDTH;
                pct_done = (pixels_done * INV_PIXEL_CT) * 100.0f;
                Console::ClearLine();
                printf("%f%% -> %d Pixels done out of %d", pct_done, pixels_done, PIXEL_CT);
            }
            i32 closeResult = fclose(file);
            return closeResult >= 0 && writeRes >= 0;
        }
    };
}