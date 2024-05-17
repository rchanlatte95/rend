#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::string
{
    #define RAC_DIGIT_TO_CHAR(x) (x + 48)
    i32 STATIC_STR_TARGET_BYTE_SIZE = 256;
    i32 STATIC_STR_CAPACITY = STATIC_STR_TARGET_BYTE_SIZE - sizeof(i32);
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
    typedef const StaticStr Str;        typedef StaticStr mut_Str;
    typedef const StaticStr* StrPtr;   typedef StaticStr* mut_StrPtr;
    typedef const StaticStr& StrRef;   typedef StaticStr& mut_StrRef;
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
        StaticStr(StrPtr _str)
        {
            len = _str->len;
            memcpy_s(chars, len, _str->chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(StrRef _str)
        {
            len = _str.len;
            memcpy_s(chars, len, _str.chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(StrPtr _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);

            len = (i32)clamp_len(char_ct);
            memcpy_s(chars, len, &_str->chars[start], len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }
        StaticStr(StrRef _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);

            len = (i32)clamp_len(char_ct);
            memcpy_s(chars, len, &_str.chars[start], len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;
        }

        operator cstr() const noexcept { return (cstr)chars; }

        INLINE i32 MaxLength() const noexcept { return STATIC_STR_MAX_LEN; }
        INLINE i32 Capacity() const noexcept { return STATIC_STR_CAPACITY; }
        INLINE i32 Length() const noexcept { return len; }
        INLINE utf8ptr Str() const noexcept { return chars; }
        INLINE bool Empty() const noexcept { return len == 0; }
        INLINE void Clear() { len = 0; chars[0] = 0; }
        INLINE void Fill(u8 c, i32 ct = STATIC_STR_MAX_LEN)
        {
            len = ct;
            memset(chars, c, len);
        }

        INLINE u8& operator[](i32 index) { return chars[(index & STATIC_STR_MAX_LEN)]; }
        INLINE StrRef operator=(cstr rhs)
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
        INLINE StrRef operator=(StrRef rhs)
        {
            if (*this == rhs)
                return *this;

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            chars[len] = 0;
            chars[STATIC_STR_MAX_LEN] = 0;

            return *this;
        }

        INLINE StrRef operator+=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            i32 new_len = len + rhs_len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
                return *this;

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE StrRef operator+=(StrRef rhs)
        {
            i32 new_len = len + rhs.len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
                return *this;

            memcpy_s(chars + len, new_len, rhs.chars, rhs.len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE StrRef operator+=(u8 c)
        {
            if (len == STATIC_STR_MAX_LEN) return *this;

            chars[len++] = c;
            chars[len] = 0;
            return *this;
        }

        INLINE bool operator>(StrRef rhs) const noexcept
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) > 0;
        }
        INLINE bool operator>=(StrRef rhs) const noexcept
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) >= 0;
        }
        INLINE bool operator<(StrRef rhs) const noexcept
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) < 0;
        }
        INLINE bool operator<=(StrRef rhs) const noexcept
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) <= 0;
        }
        INLINE bool operator==(StrRef rhs) const noexcept
        {
            return memcmp(&len, rhs.ToRef(), len) == 0;
        }
        INLINE bool operator!=(StrRef rhs) const noexcept
        {
            return memcmp(&len, rhs.ToRef(), len) != 0;
        }

        INLINE bool operator==(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            return rhs_len == len && memcmp(chars, rhs, rhs_len) == 0;
        }
        INLINE bool operator!=(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            return rhs_len == len && memcmp(chars, rhs, rhs_len) != 0;
        }

        INLINE logic::comp Compare(StrRef arg) const noexcept
        {
            return logic::comp(memcmp(&len, &arg.len, len));
        }
        INLINE logic::comp Compare(cstr c_str) const noexcept
        {
            return logic::comp(memcmp(chars, c_str, len));
        }

        INLINE cstr ToCstr() const noexcept { return (cstr)chars; }
        INLINE mut_cstr ToMutCstr() const noexcept { return (mut_cstr)chars; }
        INLINE ptr ToPtr() const noexcept { return &len; }
        INLINE u8ptr PtrToChars() const noexcept { return chars; }
        INLINE StrRef ToRef() const noexcept { return *this; }

        INLINE u8& First() const noexcept { return chars[0]; }
        INLINE u8& Last() const noexcept { return chars[len - 1]; }
        INLINE StrRef TrimStart()
        {
            if (len < 1) return *this;

            mut_u8ptr start = chars - 1;
            while (whitespace(++start));

            len -= (i32)(start - chars);
            memmove_s(chars, STATIC_STR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE StrRef TrimEnd()
        {
            if (len < 1) return *this;

            u8ptr fxd_end = chars + len - 1;
            u8ptr end = fxd_end;
            while (whitespace(--end));

            len -= (i32)(fxd_end - end);
            memmove_s(chars, STATIC_STR_MAX_LEN, chars, len);
            chars[len] = 0;

            return *this;
        }
        INLINE StrRef Trim()
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
        INLINE i32 IndexOf(u8 target) const noexcept
        {
            for (int i = 0; i < len; ++i)
                if (chars[i] == target) return i;

            return -1;
        }
        INLINE i32 IndexOf(u8 target, u8 startIndex) const noexcept
        {
            if (startIndex >= len) return -1;

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
            {
                if (!whitespace(chars[_len]))
                    return false;
            }
            return true;
        }
    };
    INLINE static Str operator +(StrRef lhs, StrRef rhs)
    {
        mut_Str res(lhs);
        res += rhs;
        return res;
    }
    INLINE static Str operator +(cstr lhs, StrRef rhs)
    {
        mut_Str res(lhs);
        res += rhs;
        return res;
    }

    i32 STATIC_SMALL_STR_CAPACITY = 16;
    i32 STATIC_SMALL_STR_MAX_LEN = STATIC_SMALL_STR_CAPACITY - 1;
    i32 STATIC_SMALL_STR_PENULT_LEN = STATIC_SMALL_STR_MAX_LEN - 1;
    class SmallStaticStr;
    typedef const SmallStaticStr SmallStr;      typedef SmallStaticStr mut_SmallStr;
    typedef const SmallStaticStr* SmallStrPtr; typedef SmallStaticStr* mut_SmallStrPtr;
    typedef const SmallStaticStr& SmallStrRef; typedef SmallStaticStr& mut_SmallStrRef;

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
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
        }
        SmallStaticStr(cstr _str)
        {
            len = (u8)strnlen_s(_str, STATIC_STR_MAX_LEN);
            chars[0] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
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
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, _str, len);
        }
        SmallStaticStr(SmallStrPtr _str)
        {
            len = _str->len;
            memcpy_s(chars, len, _str->chars, len);
            chars[len] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
        }
        SmallStaticStr(SmallStrRef _str)
        {
            len = _str.len;
            memcpy_s(chars, len, _str.chars, len);
            chars[len] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
        }
        SmallStaticStr(SmallStrPtr _str, u8 startIndex, u8 char_ct)
        {
            u8 start = clamp_len(startIndex);

            len = (u8)clamp_len(char_ct);
            memcpy_s(chars, len, &_str->chars[start], len);
            chars[len] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
        }
        SmallStaticStr(SmallStrRef _str, u8 startIndex, u8 char_ct)
        {
            u8 start = clamp_len(startIndex);

            len = (u8)clamp_len(char_ct);
            memcpy_s(chars, len, &_str.chars[start], len);
            chars[len] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
        }

        operator cstr() const noexcept { return (cstr)chars; }

        INLINE u8 MaxLength() const noexcept { return STATIC_SMALL_STR_MAX_LEN; }
        INLINE u8 Capacity() const noexcept { return STATIC_SMALL_STR_CAPACITY; }
        INLINE u8 Length() const noexcept { return len; }
        INLINE utf8ptr Str() const noexcept { return chars; }
        INLINE bool Empty() const noexcept { return len == 0; }
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

        INLINE u8& operator[](i32 index) const noexcept
        {
            return chars[(index & STATIC_SMALL_STR_MAX_LEN)];
        }
        INLINE SmallStrRef operator=(cstr rhs)
        {
            len = (u8)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            chars[0] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;
            memcpy_s(chars, STATIC_STR_MAX_LEN, rhs, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)

            return *this;
        }
        INLINE SmallStrRef operator=(SmallStrRef rhs)
        {
            if (*this == rhs) { return *this; }

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            chars[len] = 0;
            chars[STATIC_SMALL_STR_PENULT_LEN] = 0;

            return *this;
        }

        INLINE SmallStrRef operator+=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);
            i32 new_len = len + rhs_len;
            if (len == STATIC_STR_MAX_LEN || new_len >= STATIC_STR_MAX_LEN)
                return *this;

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStrRef operator+=(SmallStrRef rhs)
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
        INLINE SmallStrRef operator+=(u8 c)
        {
            if (len == STATIC_STR_MAX_LEN) return *this;
            chars[len++] = c;
            chars[len] = 0;
            return *this;
        }

        INLINE bool operator>(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) > 0;
        }
        INLINE bool operator>=(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) >= 0;
        }
        INLINE bool operator<(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) < 0;
        }
        INLINE bool operator<=(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) <= 0;
        }
        INLINE bool operator==(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs.ToRef(), len) == 0;
        }
        INLINE bool operator!=(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs.ToRef(), len) != 0;
        }

        INLINE bool operator==(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) == 0;
#pragma warning( default : 6385)
        }
        INLINE bool operator!=(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) != 0;
#pragma warning( default : 6385)
        }

        INLINE logic::comp Compare(SmallStrRef arg) const noexcept
        {
            return logic::comp(memcmp(&len, &arg.len, len));
        }
        INLINE logic::comp Compare(cstr c_str) const noexcept
        {
            return logic::comp(memcmp(chars, c_str, len));
        }

        INLINE cstr ToCstr() const noexcept { return (cstr)(chars); }
        INLINE mut_cstr ToMutCstr() const noexcept { return (mut_cstr)(chars); }
        INLINE ptr ToPtr() const noexcept { return (ptr)(&len); }
        INLINE u8ptr PtrToChars() const noexcept { return chars; }
        INLINE SmallStrRef ToRef() const noexcept { return *this; }

        INLINE u8& First() const noexcept { return chars[0]; }
        INLINE u8& Last() const noexcept { return chars[len - 1]; }
        INLINE SmallStrRef TrimStart()
        {
            if (len < 1) return *this;

            mut_u8ptr start = chars - 1;
            while (whitespace(++start));

            len -= (u8)(start - chars);
            memmove_s(chars, STATIC_STR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStrRef TrimEnd()
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
        INLINE SmallStrRef Trim()
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

            return 0;
        }
        INLINE u8 IndexOf(u8 target, u8 startIndex) const noexcept
        {
            if (startIndex >= len) return 0;

            for (int i = startIndex; i < len; ++i)
                if (chars[i] == target) return i;

            return 0;
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

            mut_i32 tmp_len = len;
            while (--tmp_len)
                if (!whitespace(chars[tmp_len])) return false;
            return true;
        }
    };
}