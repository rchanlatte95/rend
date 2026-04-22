#pragma warning(push, 0)
#include "rac-png.h"
#include <stb_image_write.h>
#pragma warning(pop)

namespace rac::img
{
    using namespace rac::gfx;
    using namespace rac::io;

    SmallStaticStr PNG_FILE_EXT = ".png";

    bool PortableNetworkGraphic::ToFile(cstr filename) const
    {
        std::string pathStr = io::GetDesktopPathStr() + '\\';
        pathStr = pathStr + filename;
        pathStr += PNG_FILE_EXT;

        i32 stride = WIDTH * 3;
        mut_u8* rgb = (mut_u8*)malloc(HEIGHT * stride);
        if (rgb == nullptr) return false;

        mut_i32 pixels_done = 0;
        for (int y = 0; y < HEIGHT; ++y)
        {
            mut_u8* row = rgb + y * stride;
            for (int x = 0; x < WIDTH; ++x)
            {
                row[x * 3 + 0] = pixels[y][x].r;
                row[x * 3 + 1] = pixels[y][x].g;
                row[x * 3 + 2] = pixels[y][x].b;
            }

            pixels_done += WIDTH;
            mut_f32 pct_done = (pixels_done * INV_PIXEL_CT) * 100.0f;
            Console::ClearLine();
            printf("%f%% -> %d Pixels done out of %d", pct_done, pixels_done, PIXEL_CT);
        }

        i32 result = stbi_write_png(pathStr.c_str(), WIDTH, HEIGHT, 3, rgb, stride);
        free(rgb);
        return result != 0;
    }
}
