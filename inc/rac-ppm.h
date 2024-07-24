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

#pragma warning(pop)

namespace rac::img
{
    using namespace rac::gfx;
    using namespace rac::string;

    class PortablePixelMap;
    typedef const PortablePixelMap ppm;    typedef PortablePixelMap mut_ppm;
    typedef const PortablePixelMap* ppm_ptr;   typedef const PortablePixelMap& ppm_ref;
    typedef PortablePixelMap* mut_ppm_ptr; typedef PortablePixelMap& mut_ppm_ref;

    i32 WIDTH = 1536;
    i32 HEIGHT = 1024;
    i32 PENULT_WIDTH = WIDTH - 1;
    i32 PENULT_HEIGHT = HEIGHT - 1;
    i32 PIXEL_COUNT = WIDTH * HEIGHT * sizeof(color);
    u64 PPM_HEADER_STR_CAP = 18;
    u64 PPM_HEADER_STR_LEN = PPM_HEADER_STR_CAP - 1;
    f32 ASPECT_RATIO = (f32)WIDTH / (f32)HEIGHT;
    SmallStaticStr PPM_FILE_EXT = ".ppm";

    std::filesystem::path GetDesktopPath()
    {
        wchar_t* p;
        if (S_OK != SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &p)) return "";
        std::filesystem::path result = p;
        CoTaskMemFree(p);
        return result;
    }
    std::string GetDesktopPathStr() { return GetDesktopPath().string(); }

    class alignas(WIN_PAGE_SIZE) PortablePixelMap
    {
    public:
        mut_color data[HEIGHT][WIDTH];

        PortablePixelMap() { }
        PortablePixelMap(color_ref init)
        {
            memset(data, init.GetI32(), PIXEL_COUNT);
        }

        INLINE mut_color_ref operator() (u32 row, u32 col) noexcept
        {
            return data[row][col];
        }
        INLINE color_ref operator[] (u32 idx) const noexcept
        {
            return *(&(data[0][0]) + idx);
        }

        MAY_INLINE bool ToFile(cstr filename) const
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
            mut_color c;
            for (int y = 0; y < HEIGHT; ++y)
            {
                for (int x = 0; x < PENULT_WIDTH; ++x)
                {
                    c = data[y][x];
                    c.r = ubyte((float(x) * WIDTH_FACTOR));
                    c.g = ubyte((float(y) * HEIGHT_FACTOR));
                    c.b = 0;
                    writeRes += fprintf_s(file, "%u %u %u ", c.r, c.g, c.b);
                    //writeRes += fprintf_s(file, "%u %u %u ", ubyte(float(x) * WIDTH_FACTOR), ubyte(float(y) * HEIGHT_FACTOR), 0);
                }

                c = data[y][PENULT_WIDTH];
                c.r = ubyte((float(PENULT_WIDTH) * WIDTH_FACTOR));
                c.g = ubyte((float(y) * HEIGHT_FACTOR));
                c.b = 0;
                writeRes += fprintf_s(file, "%u %u %u\n", c.r, c.g, c.b);
            }
            i32 closeResult = fclose(file);
            return closeResult >= 0 && writeRes >= 0;
        }
    };
}