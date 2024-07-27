#pragma once
#pragma warning(push, 0) // Prevent warnings from libraries I can't fix.
#include <float.h>
#include <math.h>
#include "rac-mth.h"
#include "rac-types.h"
#include "rac-str.h"
#include "rac-ppm.h"
#pragma warning(pop)

namespace rac::cam
{
    using namespace rac::img;
    using namespace rac::mth;

    class mut_cam;                  typedef const mut_cam cam;
    typedef const mut_cam* cam_ptr; typedef const mut_cam& cam_ref;
    typedef mut_cam* mut_cam_ptr;   typedef mut_cam& mut_cam_ref;

    class mut_viewport;                  typedef const mut_viewport viewport;
    typedef const mut_viewport* viewport_ptr; typedef const mut_viewport& viewport_ref;
    typedef mut_viewport* mut_viewport_ptr;   typedef mut_viewport& mut_viewport_ref;

    class mut_rect;                  typedef const mut_rect rect;
    typedef const mut_rect* rect_ptr; typedef const mut_rect& rect_ref;
    typedef mut_rect* mut_rect_ptr;   typedef mut_rect& mut_rect_ref;

    class mut_rect
    {
    public:
        mut_f32 top;
        mut_f32 left;
        mut_f32 bot;
        mut_f32 right;

        mut_rect()
        {
            top = 0.0f;
            left = 0.0f;
            bot = 0.0f;
            right = 0.0f;
        }
        mut_rect(f32 x)
        {
            top = x;
            left = x;
            bot = x;
            right = x;
        }
        mut_rect(f32 length, f32 width)
        {
            top = length;
            left = width;
            bot = length;
            right = width;
        }

        INLINE v2 GetTopLeft() { return v2(left, top); }
        INLINE v2 GetBotLeft() { return v2(left, bot); }
        INLINE v2 GetTopRight() { return v2(right, top); }
        INLINE v2 GetBotRight() { return v2(right, bot); }

        INLINE v2 SetTopLeft(v2 v) { return left = v.x; top = v.y; }
        INLINE v2 SetBotLeft(v2 v) { return left = v.x; bot = v.y; }
        INLINE v2 SetTopRight(v2 v) { return right = v.x; top = v.y; }
        INLINE v2 SetBotRight(v2 v) { return right = v.x; bot = v.y; }
        INLINE v2 SetTopLeft(f32 x, f32 y) { return left = x; top = y; }
        INLINE v2 SetBotLeft(f32 x, f32 y) { return left = x; bot = y; }
        INLINE v2 SetTopRight(f32 x, f32 y) { return right = x; top = y; }
        INLINE v2 SetBotRight(f32 x, f32 y) { return right = x; bot = y; }
    };

    f32 FOCAL_LENGTH = 1.0f;
    class alignas(8) mut_viewport
    {
    public:
        mut_v3 u;
        mut_v3 v;
        mut_v3 pixel_delta_u;
        mut_v3 pixel_delta_v;
        mut_v3 top_left;
        mut_v3 top_left_pixel_pos;

        static f32 HEIGHT;
        static f32 WIDTH;

        mut_viewport() { }
        mut_viewport(v3_ref cam_center, i32 img_height, i32 img_width)
        {
            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            u = v3(WIDTH, 0.0f, 0.0f);
            v = v3(0.0f, -HEIGHT, 0.0f);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = u / img_width;
            pixel_delta_v = v / img_height;

            // Calculate the location of the upper left pixel.
            v3 half_u = v3(u.x * 0.5f, 0.0f, 0.0f);
            v3 half_v = v3(0.0f, v.y * 0.5f, 0.0f);
            v3 focal_z = v3(0.0f, 0.0f, FOCAL_LENGTH);
            top_left = cam_center - focal_z - half_u - half_v;
            top_left_pixel_pos = top_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        INLINE v3 GetPixelPos(i32 x, i32 y) const noexcept
        {
            return top_left_pixel_pos + (x * pixel_delta_u) + (y * pixel_delta_v);
        }
    };
    f32 mut_viewport::HEIGHT = 2.0f;
    f32 mut_viewport::WIDTH = HEIGHT * PPM_ASPECT_RATIO;

    class mut_cam
    {
    public:
        mut_v3 Center;
        mut_viewport Viewport;

        mut_cam() { }
        mut_cam(v3 cam_center, i32 img_height, i32 img_width)
        {
            Center = cam_center;
            Viewport = mut_viewport(Center, img_height, img_width);
        }

        INLINE mut_ray GetRayFromPixel(i32 x, i32 y) const noexcept
        {
            v3 pixelPos = Viewport.GetPixelPos(x, y);
            return mut_ray(Center, (pixelPos - Center));
        }
    };
}
