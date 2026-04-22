#include "rac-mth.h"

namespace rac::mth
{
    const v2 V2_ZERO = v2(0.0f, 0.0f);
    const v2 V2_ONE = v2(1.0f, 1.0f);
    const v2 V2_SIGNED_ONE = -V2_ONE;

    const v2 V2_RIGHT = v2(1.0f, 0.0f);
    const v2 V2_LEFT = -V2_RIGHT;
    const v2 V2_UP = v2(0.0f, 1.0f);
    const v2 V2_DOWN = -V2_UP;

    const v3 V3_ZERO = v3(0.0f);
    const v3 V3_ONE = v3(1.0f);
    const v3 V3_SIGNED_ONE = -V3_ONE;

    const v3 V3_RIGHT =   v3(1.0f, 0.0f, 0.0f);
    const v3 V3_UP =      v3(0.0f, 1.0f, 0.0f);
    const v3 V3_FORWARD = v3(0.0f, 0.0f, 1.0f);

    const v3 V3_LEFT = -V3_RIGHT;
    const v3 V3_DOWN = -V3_UP;
    const v3 V3_BACKWARD = -V3_FORWARD;

    const quat IDENTITY = { 0.0f, 0.0f, 0.0f, 1.0f };

    const v4 V4_ZERO = v4(0.0f);
    const v4 V4_ONE = v4(1.0f);
    const v4 V4_SIGNED_ONE = -V4_ONE;
}
