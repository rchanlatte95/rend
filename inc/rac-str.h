#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::string
{
    #define RAC_DIGIT_TO_CHAR(x) (x + 48)
    i32 STATIC_STR_CAPACITY = 1 << 8;
    i32 STATIC_STR_MAX_LEN = STATIC_STR_CAPACITY - 1;

    // https://en.wikipedia.org/wiki/Whitespace_character
    u8 CHAR_TAB = 0x09;
    u8 LINE_FEED = 0x0A;
    u8 LINE_TAB = 0x0B;
    u8 FORM_FEED = 0x0C;
    u8 CARRIAGE_RETURN = 0x0D;
    u8 SPACE = 0x20;
    u8 NEXT_LINE = 0x85;
    u8 NO_BREAK_SPACE = 0xA0;

    static constexpr i32 clamp_len(i32 x)
    {
        if (x > STATIC_STR_MAX_LEN) return STATIC_STR_MAX_LEN;
        if (x < 0) return 0;
        return x;
    }
    static constexpr bool whitespace(u8 c)
    {
        return  c == SPACE ||
            c == LINE_FEED || c == CARRIAGE_RETURN ||
            c == CHAR_TAB || c == LINE_TAB ||
            c == NEXT_LINE || c == NO_BREAK_SPACE;
    }
    static constexpr bool whitespace(u8ptr c_ptr) { return  whitespace(*c_ptr); }

    class StaticStr;
    typedef const StaticStr Str;   typedef StaticStr mut_Str;
    typedef const StaticStr* Str_ptr;   typedef StaticStr* mut_Str_ptr;
    typedef const StaticStr& Str_ref;   typedef StaticStr& mut_Str_ref;
    class StaticStr
    {
    private:
        mut_i32 len = 0;
        mut_u8 chars[STATIC_STR_CAPACITY];

    public:
        StaticStr()
        {
            len = 0;
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(cstr _str)
        {
            len = (i32)strnlen_s(_str, STATIC_STR_MAX_LEN);
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, _str, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)
        }
        StaticStr(cstr _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);
            len = (i32)strnlen_s(&_str[start], clamp_len(char_ct));
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, _str, len);
        }
        StaticStr(Str_ptr _str)
        {
            len = _str->len;
            memcpy_s(chars, len, _str->chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(Str_ref _str)
        {
            len = _str.len;
            memcpy_s(chars, len, _str.chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(Str_ptr _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);

            len = (i32)clamp_len(char_ct);
            memcpy_s(chars, len, &_str->chars[start], len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(Str_ref _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);

            len = (i32)clamp_len(char_ct);
            memcpy_s(chars, len, &_str.chars[start], len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }

        operator cstr() const { return (cstr)chars; }

        INLINE i32 MaxLength() const { return STATIC_STR_MAX_LEN; }
        INLINE i32 Capacity() const { return STATIC_STR_CAPACITY; }
        INLINE i32 Length() const { return len; }
        INLINE utf8ptr Str() const { return &chars[0]; }
        INLINE bool Empty() const { return len == 0; }
        INLINE void Clear() { len = 0; chars[0] = 0; }
        INLINE void Fill(u8 c, i32 ct = STATIC_STR_MAX_LEN)
        {
            len = ct;
            memset(chars, c, len);
        }

        INLINE u8& operator[](i32 index) { return chars[(index & STATIC_STR_MAX_LEN)]; }
        INLINE Str_ref operator=(cstr rhs)
        {
            len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, rhs, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)

            return *this;
        }
        INLINE Str_ref operator=(Str_ref rhs)
        {
            if (*this == rhs) { return *this; }

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;

            return *this;
        }

        INLINE Str_ref operator+=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            i32 new_len = len + rhs_len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
            {
                return *this;
            }

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE Str_ref operator+=(Str_ref rhs)
        {
            i32 new_len = len + rhs.len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
            {
                return *this;
            }

            memcpy_s(chars + len, new_len, rhs.chars, rhs.len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE Str_ref operator+=(u8 c)
        {
            if (len == STATIC_STR_MAX_LEN) return *this;
            chars[len++] = c;
            chars[len] = 0;
            return *this;
        }

        INLINE bool operator>(Str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) > 0;
        }
        INLINE bool operator>=(Str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) >= 0;
        }
        INLINE bool operator<(Str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) < 0;
        }
        INLINE bool operator<=(Str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) <= 0;
        }
        INLINE bool operator==(Str_ref rhs)
        {
            return memcmp(&len, rhs.ToRef(), len) == 0;
        }
        INLINE bool operator!=(Str_ref rhs)
        {
            return memcmp(&len, rhs.ToRef(), len) != 0;
        }

        INLINE bool operator==(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) == 0;
#pragma warning( default : 6385)
        }
        INLINE bool operator!=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) != 0;
#pragma warning( default : 6385)
        }

        INLINE logic::comp Compare(Str_ref arg)
        {
            return logic::comp(memcmp(ptr(&len), ptr(&arg.len), len));
        }
        INLINE logic::comp Compare(cstr c_str)
        {
            return logic::comp(memcmp(ptr(&chars[0]), ptr(c_str), len));
        }

        INLINE cstr ToCstr() const { return (cstr)chars; }
        INLINE mut_cstr ToMutCstr() { return (mut_cstr)chars; }
        INLINE ptr ToPtr() const { return (ptr)(&len); }
        INLINE u8ptr PtrToChars() const { return chars; }
        INLINE Str_ref ToRef() const { return *this; }

        INLINE u8& First() const { return chars[0]; }
        INLINE u8& Last() const { return chars[len - 1]; }
        INLINE Str_ref TrimStart()
        {
            if (len < 1) return *this;

            mut_u8ptr start = chars - 1;
            while (whitespace(++start));

            len -= (i32)(start - chars);
            memmove_s(chars, STATIC_STR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE Str_ref TrimEnd()
        {
            if (len < 1) return *this;

            const u8ptr fxd_end = chars + len - 1;
            u8ptr end = fxd_end;
            while (whitespace(--end));

            len -= (i32)(fxd_end - end);
            memmove_s(chars, STATIC_STR_MAX_LEN, chars, len);
            chars[len] = 0;

            return *this;
        }
        INLINE Str_ref Trim()
        {
            if (len < 1) return *this;

            u8ptr start = chars - 1;
            u8ptr end = chars + len - 1;

            while (whitespace(++start));
            while (whitespace(--end));

            len = (i32)(end - start) + 1;
            memmove_s(chars, STATIC_STR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE i32 IndexOf(u8 target) const
        {
            for (int i = 0; i < len; ++i)
            {
                if (chars[i] == target) return i;
            }

            return -1;
        }
        INLINE i32 IndexOf(u8 target, i32 startIndex) const
        {
            if (startIndex >= len) return -1;

            for (int i = startIndex; i < len; ++i)
            {
                if (chars[i] == target) return i;
            }

            return -1;
        }
        INLINE bool Contains(u8 target) const
        {
            return IndexOf(target) > 0;
        }
        INLINE bool Contains(u8 target, i32 startIndex) const
        {
            return IndexOf(target, startIndex) > 0;
        }
        INLINE bool NullOrEmpty() const
        {
            return *this == nullptr || len < 1;
        }
        INLINE bool EmptyOrWhitespace() const
        {
            if (len < 1) return true;

            mut_i32 _len = len;
            while (--_len)
            {
                if (!whitespace(chars[_len])) { return false; }
            }
            return true;
        }
    };
    INLINE static Str operator +(Str_ref lhs, Str_ref rhs)
    {
        mut_Str res(lhs);
        res += rhs;
        return res;
    }
    INLINE static Str operator +(cstr lhs, Str_ref rhs)
    {
        mut_Str res(lhs);
        res += rhs;
        return res;
    }

    i32 STATIC_SMALL_STR_CAPACITY = 16;
    i32 STATIC_SMALL_STR_MAX_LEN = STATIC_SMALL_STR_CAPACITY - 1;
    class SmallStaticStr;
    typedef const SmallStaticStr SmallStr;      typedef SmallStaticStr mut_SmallStr;
    typedef const SmallStaticStr* SmallStr_ptr; typedef SmallStaticStr* mut_SmallStr_ptr;
    typedef const SmallStaticStr& SmallStr_ref; typedef SmallStaticStr& mut_SmallStr_ref;

    class alignas(8) SmallStaticStr
    {
    private:
        mut_u8 len = 0;
        mut_u8 chars[STATIC_SMALL_STR_MAX_LEN];

    public:
        SmallStaticStr()
        {
            len = 0;
            chars[0] = 0;
            chars[STATIC_SMALL_STR_MAX_LEN] = 0;
        }
        SmallStaticStr(cstr _str)
        {
            len = (u8)strnlen_s(_str, STATIC_STR_MAX_LEN);
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, _str, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)
        }
        SmallStaticStr(cstr _str, u8 startIndex, u8 char_ct)
        {
            u8 start = clamp_len(startIndex);
            len = (u8)strnlen_s(&_str[start], clamp_len(char_ct));
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, _str, len);
        }
        SmallStaticStr(SmallStr_ptr _str)
        {
            len = _str->len;
            memcpy_s(chars, len, _str->chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        SmallStaticStr(SmallStr_ref _str)
        {
            len = _str.len;
            memcpy_s(chars, len, _str.chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        SmallStaticStr(SmallStr_ptr _str, u8 startIndex, u8 char_ct)
        {
            u8 start = clamp_len(startIndex);

            len = (u8)clamp_len(char_ct);
            memcpy_s(chars, len, &_str->chars[start], len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        SmallStaticStr(SmallStr_ref _str, u8 startIndex, u8 char_ct)
        {
            u8 start = clamp_len(startIndex);

            len = (u8)clamp_len(char_ct);
            memcpy_s(chars, len, &_str.chars[start], len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }

        operator cstr() const { return (cstr)chars; }

        INLINE u8 MaxLength() const { return STATIC_SMALL_STR_MAX_LEN; }
        INLINE u8 Capacity() const { return STATIC_SMALL_STR_CAPACITY; }
        INLINE u8 Length() const { return len; }
        INLINE utf8ptr Str() const { return chars; }
        INLINE bool Empty() const { return len == 0; }
        INLINE void Clear()
        {
            len = 0;
            chars[0] = 0;
        }
        INLINE void Fill(u8 c, u8 ct = STATIC_SMALL_STR_MAX_LEN)
        {
            len = ct;
            memset(chars, c, len);
        }

        INLINE u8& operator[](i32 index) { return chars[(index & STATIC_SMALL_STR_MAX_LEN)]; }
        INLINE SmallStr_ref operator=(cstr rhs)
        {
            len = (u8)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            chars[0] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, rhs, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)

            return *this;
        }
        INLINE SmallStr_ref operator=(SmallStr_ref rhs)
        {
            if (*this == rhs) { return *this; }

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;

            return *this;
        }

        INLINE SmallStr_ref operator+=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            i32 new_len = len + rhs_len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
            {
                return *this;
            }

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStr_ref operator+=(SmallStr_ref rhs)
        {
            i32 new_len = len + rhs.len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
            {
                return *this;
            }

            memcpy_s(chars + len, new_len, rhs.chars, rhs.len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStr_ref operator+=(u8 c)
        {
            if (len == STATIC_STR_MAX_LEN) return *this;
            chars[len++] = c;
            chars[len] = 0;
            return *this;
        }

        INLINE bool operator>(SmallStr_ref rhs)
        {
            return memcmp(&len, &rhs.len, len) > 0;
        }
        INLINE bool operator>=(SmallStr_ref rhs)
        {
            return memcmp(&len, &rhs.len, len) >= 0;
        }
        INLINE bool operator<(SmallStr_ref rhs)
        {
            return memcmp(&len, &rhs.len, len) < 0;
        }
        INLINE bool operator<=(SmallStr_ref rhs)
        {
            return memcmp(&len, &rhs.len, len) <= 0;
        }
        INLINE bool operator==(SmallStr_ref rhs)
        {
            return memcmp(&len, rhs.ToRef(), len) == 0;
        }
        INLINE bool operator!=(SmallStr_ref rhs)
        {
            return memcmp(&len, rhs.ToRef(), len) != 0;
        }

        INLINE bool operator==(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) == 0;
#pragma warning( default : 6385)
        }
        INLINE bool operator!=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) != 0;
#pragma warning( default : 6385)
        }

        INLINE logic::comp Compare(SmallStr_ref arg)
        {
            return logic::comp(memcmp(&len, &arg.len, len));
        }
        INLINE logic::comp Compare(cstr c_str)
        {
            return logic::comp(memcmp(chars, c_str, len));
        }

        INLINE cstr ToCstr() const noexcept { return (cstr)(chars); }
        INLINE mut_cstr ToMutCstr() const noexcept { return (mut_cstr)(chars); }
        INLINE ptr ToPtr() const noexcept { return (ptr)(&len); }
        INLINE u8ptr PtrToChars() const noexcept { return chars; }
        INLINE SmallStr_ref ToRef() const noexcept { return *this; }

        INLINE u8& First() const noexcept { return chars[0]; }
        INLINE u8& Last() const noexcept { return chars[len - 1]; }
        INLINE SmallStr_ref TrimStart()
        {
            if (len < 1) return *this;

            mut_u8ptr start = chars - 1;
            while (whitespace(++start));

            len -= (u8)(start - chars);
            memmove_s(chars, STATIC_STR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStr_ref TrimEnd()
        {
            if (len < 1) return *this;

            const u8ptr fxd_end = chars + len - 1;
            u8ptr end = fxd_end;
            while (whitespace(--end));

            len -= (u8)(fxd_end - end);
            memmove_s(chars, STATIC_STR_MAX_LEN, chars, len);
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStr_ref Trim()
        {
            if (len < 1) return *this;

            u8ptr start = chars - 1;
            u8ptr end = chars + len - 1;

            while (whitespace(++start));
            while (whitespace(--end));

            len = (u8)(end - start) + 1;
            memmove_s(chars, STATIC_STR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE u8 IndexOf(u8 target) const noexcept
        {
            for (int i = 0; i < len; ++i)
                if (chars[i] == target) return i;

            return -1;
        }
        INLINE u8 IndexOf(u8 target, i32 startIndex) const noexcept
        {
            if (startIndex >= len) return 0;

            for (int i = startIndex; i < len; ++i)
                if (chars[i] == target) return i;

            return -1;
        }
        INLINE bool Contains(u8 target) const noexcept
        {
            return IndexOf(target) > 0;
        }
        INLINE bool Contains(u8 target, u8 startIndex) const noexcept
        {
            return IndexOf(target, startIndex) > 0;
        }
        INLINE bool NullOrEmpty() const noexcept
        {
            return *this == nullptr || len < 1;
        }
        INLINE bool EmptyOrWhitespace() const noexcept
        {
            if (len < 1) return true;

            mut_i32 _len = len;
            while (--_len)
                if (!whitespace(chars[_len])) return false;
            return true;
        }
    };
}