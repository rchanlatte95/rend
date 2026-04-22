#include "rac-ppm.h"

namespace rac::img
{
    using namespace rac::gfx;
    using namespace rac::string;
    using namespace rac::io;

    SmallStaticStr PPM_FILE_EXT = ".ppm";

    bool PortablePixelMap::DBG_ToFile(cstr filename) const
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

    bool PortablePixelMap::ToFile(cstr filename) const
    {
        std::string pathStr = io::GetDesktopPathStr() + '\\';
        pathStr = pathStr + filename;
        pathStr += PPM_FILE_EXT;

        mut_FileHandle file = nullptr;
        fopen_s(&file, pathStr.c_str(), "w");
        if (file == nullptr) { return false; }

        mut_i64 writeRes = fprintf_s(file, "P3\n%u %u\n255\n", WIDTH, HEIGHT);
        mut_i32 pixels_done = 0;
        mut_f32 pct_done = 0.0f;
        mut_color c;
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < PENULT_WIDTH; ++x)
            {
                c = pixels[y][x];
                writeRes += fprintf_s(file, "%u %u %u ", c.r, c.g, c.b);
            }
            c = pixels[y][PENULT_WIDTH];
            writeRes += fprintf_s(file, "%u %u %u\n", c.r, c.g, c.b);

            pixels_done += WIDTH;
            pct_done = (pixels_done * INV_PIXEL_CT) * 100.0f;
            Console::ClearLine();
            printf("%f%% -> %d Pixels done out of %d", pct_done, pixels_done, PIXEL_CT);
        }
        i32 closeResult = fclose(file);
        return closeResult >= 0 && writeRes >= 0;
    }
}
