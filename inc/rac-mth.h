#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <float.h>
#include <math.h>

#include "rac-types.h"
#include "rac-str.h"

#pragma warning(pop)


namespace rac::mth
{
    class Vector2I16;
    typedef const Vector2I16 v2i16;    typedef Vector2I16 mut_v2i16;
    typedef const Vector2I16* v2i16_ptr;   typedef const Vector2I16& v2i16_ref;
    typedef Vector2I16* mut_v2i16_ptr; typedef Vector2I16& mut_v2i16_ref;

    class Vector2I32;
    typedef const Vector2I32 v2i;    typedef Vector2I32 mut_v2i;
    typedef const Vector2I32* v2i_ptr;   typedef const Vector2I32& v2i_ref;
    typedef Vector2I32* mut_v2i_ptr; typedef Vector2I32& mut_v2i_ref;

    class Vector2I16
    {
    public:
        mut_i16 x = 0;
        mut_i16 y = 0;

        Vector2I16() {}
        Vector2I16(i16 a) { x = a; y = a; }
        Vector2I16(i16 _x, i16 _y) { x = _x; y = _y; }

        INLINE v2i16_ref operator=(v2i16_ref rhs)
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }
        INLINE v2i16 operator -() const { return v2i16(-x, -y); }
        INLINE v2i16 operator *(i16 v) { return v2i16(x * v, y * v); }
        INLINE v2i16 operator /(i16 v)
        {
            return v2i16(x / v, y / v);
        }
        INLINE v2i16_ref operator +=(v2i16_ref rhs)
        {
            x += rhs.x;
            y += rhs.y;
        }
        INLINE v2i16_ref operator -=(v2i16_ref rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
        }
        INLINE v2i16_ref operator *=(v2i16_ref rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
        }
        INLINE v2i16_ref operator *=(i16 a)
        {
            x *= a;
            y *= a;
        }
        INLINE v2i16_ref operator /=(v2i16_ref rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
        }
        INLINE v2i16_ref operator /=(i16 a)
        {
            x /= a;
            y /= a;
        }
    };

    class Vector2I32
    {
    public:
        mut_i32 x = 0;
        mut_i32 y = 0;

        Vector2I32() {}
        Vector2I32(i32 a) { x = a; y = a; }
        Vector2I32(i32 _x, i32 _y) { x = _x; y = _y; }

        INLINE v2i_ref operator=(v2i_ref rhs)
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }
        INLINE v2i operator -() const { return v2i(-x, -y); }
        INLINE v2i operator *(i32 v) { return v2i(x * v, y * v); }
        INLINE v2i operator /(i32 v)
        {
            return v2i(x / v, y / v);
        }
        INLINE v2i_ref operator +=(v2i_ref rhs)
        {
            x += rhs.x;
            y += rhs.y;
        }
        INLINE v2i_ref operator -=(v2i_ref rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
        }
        INLINE v2i_ref operator *=(v2i_ref rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
        }
        INLINE v2i_ref operator *=(i32 a)
        {
            x *= a;
            y *= a;
        }
        INLINE v2i_ref operator /=(v2i_ref rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
        }
        INLINE v2i_ref operator /=(i32 a)
        {
            x /= a;
            y /= a;
        }
    };

    class Vector2;
    typedef const Vector2 v2;    typedef Vector2 mut_v2;
    typedef const Vector2* v2_ptr;   typedef const Vector2& v2_ref;
    typedef Vector2* mut_v2_ptr; typedef Vector2& mut_v2_ref;

    class Vector3;
    typedef const Vector3 v3; typedef Vector3 mut_v3;
    typedef const Vector3* v3_ptr;   typedef const Vector3& v3_ref;
    typedef Vector3* mut_v3_ptr; typedef Vector3& mut_v3_ref;

    class Quaternion;
    typedef const Quaternion quat; typedef Quaternion mut_quat;
    typedef const Quaternion* quat_ptr;   typedef const Quaternion& quat_ref;
    typedef Quaternion* mut_quat_ptr; typedef Quaternion& mut_quat_ref;

    const f32 PI = 3.14159265358979323846f;
    const f32 SIXTH_PI = PI / 6.0f;
    const f32 FOURTH_PI = PI / 4.0f;
    const f32 THIRD_PI = PI / 3.0f;
    const f32 HALF_PI = PI / 2.0f;
    const f32 TAU = 2.0f * PI;

    const f32 INV_PI = 1.0f / PI;
    const f32 DEG2RAD = PI / 180.0f;
    const f32 RAD2DEG = 180.0f / PI;

    f32 F32_EPSILON = FLT_EPSILON;
    f32 F32_MAX = FLT_MAX;
    f32 F32_MIN = FLT_MIN;

    f32 SIGNED_F32_EPSILON = -F32_EPSILON;
    f32 F32_ONE_EPSILON = 1.0f - F32_EPSILON;
    f32 SIGNED_F32_ONE_EPSILON = -F32_ONE_EPSILON;

    // Single precision floating point number has a range of eight
    // digits. Add a decimal point, a negative sign, and a space
    // for the null terminator and you have the length of a float
    // string.
    #define RAC_F32_UNSIGNED_RANGE 8
    #define RAC_F32_SIGNED_RANGE (RAC_F32_UNSIGNED_RANGE + 1)

    #define RAC_F32_APPROX_ZERO(a) (fabsf(a) <= F32_EPSILON)
    #define RAC_F32_APPROX_ONE(a) (RAC_F32_APPROX(a, F32_ONE_EPSILON))

    #define RAC_F32_APPROX_MORE(a, b) (fabsf(a - b) > F32_EPSILON)
    #define RAC_F32_APPROX_LESS(a, b) (fabsf(a - b) < SIGNED_F32_EPSILON)
    #define RAC_F32_APPROX_MORE_OR_EQUAL(a, b) (fabsf(a - b) >= F32_EPSILON)
    #define RAC_F32_APPROX_LESS_OR_EQUAL(a, b) (fabsf(a - b) <= SIGNED_F32_EPSILON)
    #define RAC_F32_APPROX_POS(a) (a >= F32_EPSILON)
    #define RAC_F32_APPROX_NEG(a) (a <= F32_EPSILON)

    static constexpr bool Approx(f32 a, f32 b)
    {
        if (a == b) return true; // Avoids divisions by zero
        if (_isnanf(a) || _isnanf(b)) return false; // Handle NaN cases

        // The min here handles the INF cases
        f32 _min = min((std::fabsf(a + b)), F32_MAX);
        f32 norm = F32_EPSILON * _min;
        return fabsf(a - b) < max(F32_MIN, norm);
    }

    i32 F32_STR_MAX = RAC_F32_SIGNED_RANGE + 3;
    i32 F32_STR_LEN = F32_STR_MAX - 1;

    static f32 Clamp(f32 x, f32 min, f32 max)
    {
        if (fabsf(x - max) > F32_EPSILON) return max;
        if (fabsf(x - min) < F32_EPSILON) return min;
        return x;
    }
    static f32 Clamp01(f32 x)
    {
        if (fabsf(x - F32_ONE_EPSILON) > F32_EPSILON) return 1.0f;
        if (fabsf(x) < F32_EPSILON) return 0.0f;
        return x;
    }

    i32 COMMA_SPACE_LEN = 2;
    i32 PARENTHESES_LEN = 2;
    i32 V2_STRING_MAX = (F32_STR_LEN * 2) + COMMA_SPACE_LEN + PARENTHESES_LEN + 1;
    i32 V2_STRING_LEN = V2_STRING_MAX - 1;

    class Vector2
    {
    public:
        mut_f32 x = 0.0f;
        mut_f32 y = 0.0f;

        Vector2(i8 _x, i8 _y) { x = (mut_f32)_x; y = (mut_f32)_y; }
        Vector2(i16 _x, i16 _y) { x = (mut_f32)_x; y = (mut_f32)_y; }
        Vector2(i32 _x, i32 _y) { x = (mut_f32)_x; y = (mut_f32)_y; }
        Vector2(i8 a) { x = (mut_f32)a; y = (mut_f32)a; }
        Vector2(i16 a) { x = (mut_f32)a; y = (mut_f32)a; }
        Vector2(i32 a) { x = (mut_f32)a; y = (mut_f32)a; }

        Vector2(f32 _x, f32 _y) { x = _x; y = _y; }
        Vector2(f32 a) { x = a; y = a; }
        Vector2() { }

        INLINE v2_ref operator=(v2_ref rhs)
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }
        INLINE v2 operator -() const { return v2(-x, -y); }
        INLINE v2 operator *(f32 v) { return v2(x * v, y * v); }
        INLINE v2 operator /(f32 v)
        {
            f32 inv_v = 1.0f / v;
            return v2(x * inv_v, y * inv_v);
        }
        INLINE v2_ref operator +=(v2_ref rhs)
        {
            x += rhs.x;
            y += rhs.y;
        }
        INLINE v2_ref operator -=(v2_ref rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
        }
        INLINE v2_ref operator *=(v2_ref rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
        }
        INLINE v2_ref operator *=(f32 a)
        {
            x *= a;
            y *= a;
        }
        INLINE v2_ref operator /=(v2_ref rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
        }
        INLINE v2_ref operator /=(f32 a)
        {
            f32 inv = 1.0f / a;
            x *= inv;
            y *= inv;
        }

        INLINE ptr Ptr() const { return (ptr)(&x); }
        INLINE v2_ref Ref() const { return *this; }

        INLINE string::Str_ref ToStr(string::mut_Str_ref str) const
        {
            char buff[V2_STRING_MAX] = { 0 };
            sprintf_s(buff, V2_STRING_LEN, "(%0.3f, %0.3f)", x, y);
            return str = buff;
        }

        INLINE v2 Max(v2 v)
        {
            return v2(fmaxf(x, v.x), fmaxf(y, v.y));
        }
        INLINE v2 Min(v2 v)
        {
            return v2(fminf(x, v.x), fminf(y, v.y));
        }
        INLINE v2 Clamp(f32 min, f32 max)
        {
            return v2(x > max ? max : (x < min ? min : x),
                y > max ? max : (y < min ? min : y));
        }
        INLINE v2 Clamp(f32 x_min, f32 x_max, f32 y_min, f32 y_max)
        {
            return v2(x > x_max ? x_max : (x < x_min ? x_min : x),
                y > y_max ? y_max : (y < y_min ? y_min : y));
        }
        INLINE v2 Clamp(v2 min, v2 max)
        {
            return v2(x > max.x ? max.x : (x < min.x ? min.x : x),
                y > max.y ? max.y : (y < min.y ? min.y : y));
        }

        INLINE v2 ClampMag(f32 min, f32 max)
        {
            mut_v2 res = *this;
            mut_f32 len = res.x * res.x + res.y + res.y;
            if (len > F32_EPSILON)
            {
                len = sqrtf(len);
                mut_f32 scale = 1.0f;
                if (len < min) { scale *= min / len; }
                else if (len > max) { scale *= max / len; }

                res.x *= scale;
                res.y *= scale;
            }

            return res;
        }

        INLINE v2 Inv() const { return v2(1.0f / x, 1.0f / y); }
        INLINE f32 Mag() const { return sqrtf(x * x + y * y); }
        INLINE f32 InvMag() const { return 1.0f / sqrtf(x * x + y * y); }
        INLINE f32 SqrMag() const { return x * x + y * y; }
        INLINE f32 Dot() const { return x * x + y * y; }
        INLINE f32 Dot(v2_ref v2) const { return x * v2.x + y * v2.y; }
        INLINE f32 Dist(v2_ref from) const
        {
            return sqrtf((x - from.x) * (x - from.x) + (y * from.y) * (y * from.y));
        }
        INLINE f32 SqrDist(v2_ref from) const
        {
            return (x - from.x) * (x - from.x) + (y * from.y) * (y * from.y);
        }
        INLINE f32 Angle(v2_ref from) const
        {
            f32 dot = x * from.x + y * from.y;
            f32 det = x * from.y - y * from.x;
            return atan2f(det, dot);
        }
        INLINE v2 Norm() const
        {
            f32 inv_mag = InvMag();
            if (inv_mag > F32_EPSILON)
            {
                return v2(x * inv_mag, y * inv_mag);
            }
            else return *this;
        }
        INLINE v2 Rotate(f32 radians) const
        {
            f32 cosRes = cosf(radians);
            f32 sinRes = sinf(radians);
            return v2(x * cosRes - y * sinRes, x * sinRes + y * cosRes);
        }
        INLINE v2 RotateDeg(f32 degrees) const
        {
            return Rotate(degrees * RAD2DEG);
        }
        INLINE v2 Perpendicular() const { return v2(y, -x); }
    };

    INLINE static bool operator >(v2_ref lhs, v2_ref rhs)
    {
        return  lhs.x > rhs.x && lhs.y > rhs.y;
    }
    INLINE static bool operator <(v2_ref lhs, v2_ref rhs)
    {
        return lhs.x < rhs.x && lhs.y < rhs.y;
    }
    INLINE static bool operator >=(v2_ref lhs, v2_ref rhs)
    {
        return lhs.x >= rhs.x && lhs.y >= rhs.y;
    }
    INLINE static bool operator <=(v2_ref lhs, v2_ref rhs)
    {
        return lhs.x <= rhs.x && lhs.y <= rhs.y;
    }
    INLINE static bool operator ==(v2_ref lhs, v2_ref rhs)
    {
        return  lhs.x == rhs.x && lhs.y == rhs.y;
    }
    INLINE static bool operator !=(v2_ref lhs, v2_ref rhs)
    {
        return !(lhs == rhs);
    }

    INLINE static v2 operator +(v2_ref lhs, v2_ref rhs)
    {
        return v2(lhs.x + rhs.x, lhs.y + rhs.y);
    }
    INLINE static v2 operator -(v2_ref lhs, v2_ref rhs)
    {
        return v2(lhs.x - rhs.x, lhs.y - rhs.y);
    }
    INLINE static v2 operator *(v2_ref lhs, v2_ref rhs)
    {
        return v2(lhs.x * rhs.x, lhs.y * rhs.y);
    }
    INLINE static v2 operator /(v2_ref lhs, v2_ref rhs)
    {
        return v2(lhs.x / rhs.x, lhs.y / rhs.y);
    }

    const v2 V2_ZERO = v2(0.0f, 0.0f);
    const v2 V2_ONE = v2(1.0f, 1.0f);
    const v2 V2_SIGNED_ONE = -V2_ONE;

    const v2 V2_RIGHT = v2(1.0f, 0.0f);
    const v2 V2_LEFT = -V2_RIGHT;
    const v2 V2_UP = v2(0.0f, 1.0f);
    const v2 V2_DOWN = -V2_UP;

    // Check if two vectors are approximately equal
    static MAY_INLINE bool Approx(v2_ref a, v2_ref b)
    {
        return Approx(a.x, b.x) && Approx(a.y, b.y);
    }
    static MAY_INLINE bool NotApprox(v2_ref a, v2_ref b)
    {
        return !Approx(a, b);
    }

    static MAY_INLINE v2 MoveToward(v2_ref from, v2_ref to, f32 maxDelta)
    {
        v2 dv = to - from;
        f32 dot = dv.Dot();

        if (dot <= F32_EPSILON || RAC_F32_APPROX_POS(maxDelta) &&
            (dot <= maxDelta * maxDelta))
        {
            return to;
        }

        f32 inv_scaled_dist = 1.0f / (sqrtf(dot) * maxDelta);
        return from + (dv * inv_scaled_dist);
    }

    static MAY_INLINE v2 Refract(v2_ref incoming, v2_ref normal, f32 refractRatio)
    {
        mut_v2 res = V2_ZERO;
        f32 dot = incoming.Dot(normal);
        mut_f32 d = 1.0f - refractRatio * refractRatio * (1.0f - dot * dot);
        if (d >= F32_EPSILON)
        {
            d = sqrtf(d);
            res.x = refractRatio * incoming.x - (refractRatio * dot + d) * normal.x;
            res.y = refractRatio * incoming.y - (refractRatio * dot + d) * normal.y;
        }

        return res;
    }

    i32 V3_STRING_MAX = (F32_STR_LEN * 3) + (COMMA_SPACE_LEN * 2) + PARENTHESES_LEN + 1;
    i32 V3_STRING_LEN = V3_STRING_MAX - 1;
    class Vector3
    {
    public:
        mut_f32 x = 0.0f;
        mut_f32 y = 0.0f;
        mut_f32 z = 0.0f;

        Vector3() { }
        Vector3(i8 _x, i8 _y, i8 _z)
        {
            x = (mut_f32)_x;
            y = (mut_f32)_y;
            z = (mut_f32)_z;
        }
        Vector3(i16 _x, i16 _y, i16 _z)
        {
            x = (mut_f32)_x;
            y = (mut_f32)_y;
            z = (mut_f32)_z;
        }
        Vector3(i32 _x, i32 _y, i32 _z)
        {
            x = (mut_f32)_x;
            y = (mut_f32)_y;
            z = (mut_f32)_z;
        }
        Vector3(u8 _x, u8 _y, u8 _z)
        {
            x = (mut_f32)_x;
            y = (mut_f32)_y;
            z = (mut_f32)_z;
        }
        Vector3(u16 _x, u16 _y, u16 _z)
        {
            x = (mut_f32)_x;
            y = (mut_f32)_y;
            z = (mut_f32)_z;
        }
        Vector3(u32 _x, u32 _y, u32 _z)
        {
            x = (mut_f32)_x;
            y = (mut_f32)_y;
            z = (mut_f32)_z;
        }

        Vector3(f32 _a)
        {
            x = _a;
            y = _a;
            z = _a;
        }
        Vector3(f32 _x, f32 _y)
        {
            x = _x;
            y = _y;
            z = 0.0f;
        }
        Vector3(f32 _x, f32 _y, f32 _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }
        Vector3(v2 v, f32 _z)
        {
            x = v.x;
            y = v.y;
            z = _z;
        }
        Vector3(v2 v)
        {
            x = v.x;
            y = v.y;
            z = 0.0f;
        }

        INLINE v3_ref operator=(v3_ref rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }
        INLINE v3 operator -() const { return v3(-x, -y, -z); }
        INLINE v3 operator *(f32 v) { return v3(x * v, y * v, z * v); }
        INLINE v3 operator /(f32 v)
        {
            f32 inv = 1.0f / v;
            return v3(x * inv, y * inv, z * inv);
        }
        INLINE v3_ref operator +=(v3_ref rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
        }
        INLINE v3_ref operator -=(v3_ref rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
        }
        INLINE v3_ref operator *=(v3_ref rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
        }
        INLINE v3_ref operator *=(f32 a)
        {
            x *= a;
            y *= a;
            z *= a;
        }
        INLINE v3_ref operator /=(v3_ref rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
        }
        INLINE v3_ref operator /=(f32 a)
        {
            f32 inv = 1.0f / a;
            x *= inv;
            y *= inv;
            z *= inv;
        }

        INLINE ptr Ptr() const { return (ptr)(&x); }
        INLINE v3_ref Ref() const { return *this; }

        INLINE string::Str_ref ToStr(string::mut_Str_ref str) const
        {
            char buff[V3_STRING_MAX] = { 0 };
            sprintf_s(buff, V3_STRING_LEN, "(%0.3f, %0.3f, %0.3f)", x, y, z);
            return str = buff;
        }

        INLINE v3 Max(v3 v)
        {
            return v3(fmaxf(x, v.x), fmaxf(y, v.y), fmaxf(z, v.z));
        }
        INLINE v3 Min(v3 v)
        {
            return v3(fminf(x, v.x), fminf(y, v.y), fminf(z, v.z));
        }
        INLINE v3 Clamp(f32 min, f32 max)
        {
            return v3(x > max ? max : (x < min ? min : x),
                y > max ? max : (y < min ? min : y),
                z > max ? max : (z < min ? min : z));
        }
        INLINE v3 Clamp(f32 x_min, f32 x_max, f32 y_min, f32 y_max, f32 z_min, f32 z_max)
        {
            return v3(x > x_max ? x_max : (x < x_min ? x_min : x),
                y > y_max ? y_max : (y < y_min ? y_min : y),
                z > z_max ? z_max : (z < z_min ? z_min : z));
        }
        INLINE v3 Clamp(v3 min, v3 max)
        {
            return v3(x > max.x ? max.x : (x < min.x ? min.x : x),
                y > max.y ? max.y : (y < min.y ? min.y : y),
                z > max.z ? max.z : (z < min.z ? min.z : z));
        }
        INLINE v3 Inv() const
        {
            return v3(1.0f / x, 1.0f / y, 1.0f / z);
        }
        INLINE f32 Mag() const
        {
            return sqrtf(x * x + y * y + z * z);
        }
        INLINE f32 InvMag() const
        {
            return 1.0f / sqrtf(x * x + y * y + z * z);
        }
        INLINE f32 SqrMag() const
        {
            return x * x + y * y + z * z;
        }
        INLINE f32 Dot(v3_ref v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }
        INLINE f32 Dist(v3_ref from) const
        {
            return sqrtf((x - from.x) * (x - from.x) +
                (y * from.y) * (y * from.y) +
                (z * from.z) * (z * from.z));
        }
        INLINE f32 SqrDist(v3_ref from) const
        {
            return  (x - from.x) * (x - from.x) +
                (y * from.y) * (y * from.y) +
                (z * from.z) * (z * from.z);
        }
        INLINE v3 Norm() const
        {
            f32 inv_mag = InvMag();
            if (inv_mag > F32_EPSILON)
            {
                return v3(x * inv_mag, y * inv_mag, z * inv_mag);
            }
            else return *this;
        }

        // Cross product with unit X Vector3
        // Unit X == v3(1.0f, 0.0f, 0.0f)
        INLINE v3 CrossX() { return v3(0.0f, z, -y); }

        // Cross product with unit Y Vector3
        // Unit Y == v3(0.0f, 1.0f, 0.0f)
        INLINE v3 CrossY() { return v3(-z, 0.0f, x); }

        // Cross product with unit X Vector3
        // Unit Z == v3(0.0f, 0.0f, 1.0f)
        INLINE v3 CrossZ() { return v3(y, -x, 0.0f); }
    };

    INLINE static bool operator >(v3_ref lhs, v3_ref rhs)
    {
        return  lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
    }
    INLINE static bool operator <(v3_ref lhs, v3_ref rhs)
    {
        return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
    }
    INLINE static bool operator >=(v3_ref lhs, v3_ref rhs)
    {
        return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
    }
    INLINE static bool operator <=(v3_ref lhs, v3_ref rhs)
    {
        return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
    }
    INLINE static bool operator ==(v3_ref lhs, v3_ref rhs)
    {
        return  lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
    INLINE static bool operator !=(v3_ref lhs, v3_ref rhs)
    {
        return !(lhs == rhs);
    }

    INLINE static v3 operator +(v3_ref lhs, v3_ref rhs)
    {
        return v3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }
    INLINE static v3 operator -(v3_ref lhs, v3_ref rhs)
    {
        return v3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }
    INLINE static v3 operator *(v3_ref lhs, v3_ref rhs)
    {
        return v3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }
    INLINE static v3 operator /(v3_ref lhs, v3_ref rhs)
    {
        return v3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
    }

    // Check if two vectors are approximately equal
    static MAY_INLINE bool Approx(v3_ref a, v3_ref b)
    {
        return  Approx(a.x, b.x) && Approx(a.y, b.y) && Approx(a.z, b.z);
    }
    static MAY_INLINE bool NotApprox(v3_ref a, v3_ref b)
    {
        return !Approx(a, b);
    }
    static MAY_INLINE v3 Cross(v3 v1, v3 v2)
    {
        return v3(v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x);
    }

    const v3 V3_ZERO = v3(0.0f);
    const v3 V3_ONE = v3(1.0f);
    const v3 V3_SIGNED_ONE = -V3_ONE;

    const v3 V3_RIGHT =     v3(1.0f, 0.0f, 0.0f);
    const v3 V3_UP =        v3(0.0f, 1.0f, 0.0f);
    const v3 V3_FORWARD =   v3(0.0f, 0.0f, 1.0f);

    const v3 V3_LEFT = -V3_RIGHT;
    const v3 V3_DOWN = -V3_UP;
    const v3 V3_BACKWARD = -V3_FORWARD;
}