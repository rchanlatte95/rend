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

    class mut_cam;                  typedef const mut_cam cam;
    typedef const mut_cam* cam_ptr; typedef const mut_cam& cam_ref;
    typedef mut_cam* mut_ppm_ptr;   typedef mut_cam& mut_cam_ref;

    f32 VIEWPORT_HEIGHT = 2.0f;
    f32 VIEWPORT_WIDTH = VIEWPORT_HEIGHT * PPM_ASPECT_RATIO;
}
