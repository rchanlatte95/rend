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

    i32 MAX_WIDTH = 1536;
    i32 MAX_HEIGHT = 1024;
    i32 PIXEL_COUNT = MAX_WIDTH * MAX_HEIGHT;
    f32 ASPECT_RATIO = (f32)MAX_WIDTH / (f32)MAX_HEIGHT;
    SmallStaticStr PPM_FILE_EXT = ".ppm";

    std::filesystem::path GetDesktopPath()
    {
        wchar_t* p;
        if (S_OK != SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &p)) return "";
        std::filesystem::path result = p;
        CoTaskMemFree(p);
        return result;
    }

    class alignas(WIN_PAGE_SIZE) PortablePixelMap
    {
    public:
        mut_color data[MAX_WIDTH][MAX_HEIGHT];

        PortablePixelMap() { }
        PortablePixelMap(color initColor)
        {
            mut_color_ptr cp = &(data[0][0]);
            mut_i32 ct = PIXEL_COUNT;
            while (--ct > -1) { cp[ct] = initColor; }
        }

        INLINE color_ref operator() (u32 row, u32 col) const noexcept
        {
            return data[row][col];
        }
        INLINE color_ref operator[] (u32 idx) const noexcept
        {
            return *(&(data[0][0]) + idx);
        }

        MAY_INLINE bool ToFile(cstr filename) const
        {
            std::string desktop = GetDesktopPath().string();
            desktop += filename;
            desktop += PPM_FILE_EXT;
            mut_FileHandle file;
            fopen_s(&file, filename, "w");

            u64 write_res = fwrite(data, sizeof(ppm), PIXEL_COUNT, file);
            return fclose(file) >= 0 && write_res >= 0;
        }
    };
}