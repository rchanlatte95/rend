#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <float.h>
#include <math.h>

#include "rac-types.h"
#include "rac-str.h"

#pragma warning(pop)

namespace rac::gfx
{
    class Colour;
    typedef const Colour color;    typedef Colour mut_color;
    typedef const Colour* color_ptr;   typedef const Colour& color_ref;
    typedef Colour* mut_color_ptr; typedef Colour& mut_color_ref;

    class color
    {
    public:
        union
        {
            u32 col = 0;
            ubyte a;
            ubyte r;
            ubyte g;
            ubyte b;
        };
    };
}