#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::string
{
    #define RAC_DIGIT_TO_CHAR(x) (x + 48)

    inline static constexpr i32 STATIC_STR_TARGET_BYTE_SIZE = 512;
    inline static constexpr i32 STATIC_STR_CAPACITY = STATIC_STR_TARGET_BYTE_SIZE - sizeof(i32) - sizeof(u8);
    inline static constexpr i32 STATIC_STR_MAX_INDEX = STATIC_STR_CAPACITY - 1;

    // https://en.wikipedia.org/wiki/Whitespace_character
    inline static constexpr u8 NULL_TERM = '\0';
    inline static constexpr u8 CHAR_TAB = 0x09;
    inline static constexpr u8 LINE_FEED = 0x0A;
    inline static constexpr u8 LINE_TAB = 0x0B;
    inline static constexpr u8 FORM_FEED = 0x0C;
    inline static constexpr u8 CARRIAGE_RETURN = 0x0D;
    inline static constexpr u8 SPACE = 0x20;
    inline static constexpr u8 NEXT_LINE = 0x85;
    inline static constexpr u8 NO_BREAK_SPACE = 0xA0;
    inline static constexpr bool whitespace(u8 c)
    {
        return  c == SPACE ||
                c == LINE_FEED || c == CARRIAGE_RETURN ||
                c == CHAR_TAB || c == LINE_TAB ||
                c == NEXT_LINE || c == NO_BREAK_SPACE;
    }
    inline static constexpr bool whitespace(u8ptr c_ptr)
    {
        return  whitespace(*c_ptr);
    }

    class StaticStr;
    typedef const StaticStr Str;        typedef StaticStr mut_Str;
    typedef const StaticStr* StrPtr;   typedef StaticStr* mut_StrPtr;
    typedef const StaticStr& StrRef;   typedef StaticStr& mut_StrRef;
    class StaticStr
    {

    private:
        mut_i32 len = 0;
        mut_u8 chars[STATIC_STR_CAPACITY];
        u8 NULL_TERMINATOR = NULL_TERM;

    public:
        INLINE static constexpr i32 ClampToBounds(i32 x)
        {
            return  x > STATIC_STR_MAX_INDEX ? STATIC_STR_MAX_INDEX :
                    (x < 0) ? 0 : x;
        }

        StaticStr() { chars[0] = 0; }
        StaticStr(cstr _str)
        {
            len = (i32)strnlen_s(_str, STATIC_STR_CAPACITY);
            memcpy_s(chars, STATIC_STR_CAPACITY, _str, len);
        }
        StaticStr(cstr _str, u32 char_ct, u32 startIndex = 0)
        {
            i32 ct = char_ct > STATIC_STR_CAPACITY ? STATIC_STR_CAPACITY : char_ct;
            len = (i32)strnlen_s(_str + startIndex, ct);
            memcpy_s(chars, STATIC_STR_CAPACITY, _str, len);
        }
        StaticStr(StrRef _str)
        {
            len = _str.len;
            memcpy_s(chars, len, _str.chars, len);
        }
        StaticStr(StrRef _str, i32 char_ct, u32 startIndex = 0)
        {
            i32 start = ClampToBounds(startIndex);
            len = (i32)ClampToBounds(char_ct);
            memcpy_s(chars, len, &_str.chars[start], len);
        }

        operator cstr() const noexcept { return (cstr)chars; }

        INLINE i32 Capacity() const noexcept { return STATIC_STR_CAPACITY; }
        INLINE i32 Len() const noexcept { return len; }
        INLINE i32 PenultLen() const noexcept { return ClampToBounds(len - 1); }
        INLINE utf8ptr Str() const noexcept { return chars; }
        INLINE logic::Bool Empty() const noexcept { return len == 0; }
        INLINE void Clear(bool shallow = true)
        {
            if (shallow)
                len = 0;
            else
                memset(&len, 0, STATIC_STR_TARGET_BYTE_SIZE);
        }
        INLINE void Fill(u8 c, u32 ct = STATIC_STR_CAPACITY)
        {
            len = ct > STATIC_STR_CAPACITY ? STATIC_STR_CAPACITY : ct;
            memset(chars, c, len);
        }

        INLINE u8& operator[](i32 index) { return chars[index]; }
        INLINE StrRef operator=(cstr rhs)
        {
            len = (i32)strnlen_s(rhs, STATIC_STR_CAPACITY);
            memcpy_s(chars, STATIC_STR_CAPACITY, rhs, len);
            return *this;
        }
        INLINE StrRef operator=(StrRef rhs)
        {
            if (*this == rhs)
                return *this;

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            return *this;
        }

        INLINE StrRef operator+=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_CAPACITY);
            i32 new_len = len + rhs_len;
            if (len == STATIC_STR_CAPACITY || new_len >= STATIC_STR_CAPACITY)
                return *this;

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            return *this;
        }
        INLINE StrRef operator+=(StrRef rhs)
        {
            i32 new_len = len + rhs.len;
            if (len == STATIC_STR_CAPACITY || new_len >= STATIC_STR_CAPACITY)
                return *this;

            memcpy_s(chars + len, new_len, rhs.chars, rhs.len);
            len = new_len;
            return *this;
        }
        INLINE StrRef operator+=(u8 c)
        {
            if (len == STATIC_STR_CAPACITY) return *this;
            chars[len++] = c;
            return *this;
        }

        INLINE logic::Bool operator>(StrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) > 0;
        }
        INLINE logic::Bool operator>=(StrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) >= 0;
        }
        INLINE logic::Bool operator<(StrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) < 0;
        }
        INLINE logic::Bool operator<=(StrRef rhs) const noexcept
        {
            return memcmp(&len, &rhs.len, len) <= 0;
        }
        INLINE logic::Bool operator==(StrRef rhs) const noexcept
        {
            return memcmp(&len, rhs.ToRef(), len) == 0;
        }
        INLINE logic::Bool operator!=(StrRef rhs) const noexcept
        {
            return memcmp(&len, rhs.ToRef(), len) != 0;
        }

        INLINE logic::Bool operator==(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_TARGET_BYTE_SIZE);
            return rhs_len == len && memcmp(chars, rhs, STATIC_STR_CAPACITY) == 0;
        }
        INLINE logic::Bool operator!=(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, STATIC_STR_TARGET_BYTE_SIZE);
            return rhs_len != len || memcmp(chars, rhs, STATIC_STR_CAPACITY) != 0;
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

        MAY_INLINE u8& First() const noexcept { return chars[0]; }
        MAY_INLINE u8& Last() const noexcept { return chars[len - 1]; }
        MAY_INLINE StrRef TrimStart()
        {
            if (len < 1) return *this;

            mut_u8ptr start = chars - 1;
            while (whitespace(++start));

            len -= (i32)(start - chars);
            memmove_s(chars, STATIC_STR_CAPACITY, start, len);

            return *this;
        }
        MAY_INLINE StrRef TrimEnd()
        {
            if (len < 1) return *this;

            u8ptr fxd_end = chars + len - 1;
            u8ptr end = fxd_end;
            while (whitespace(--end));

            len -= (i32)(fxd_end - end);
            memmove_s(chars, STATIC_STR_CAPACITY, chars, len);

            return *this;
        }
        MAY_INLINE StrRef Trim()
        {
            if (len < 1) return *this;

            u8ptr start = chars - 1;
            u8ptr end = chars + len - 1;

            while (whitespace(++start));
            while (whitespace(--end));

            len = (i32)(end - start) + 1;
            memmove_s(chars, STATIC_STR_CAPACITY, start, len);

            return *this;
        }
        MAY_INLINE i32 IndexOf(u8 target) const noexcept
        {
            for (int i = 0; i < len; ++i)
                if (chars[i] == target) return i;

            return -1;
        }
        MAY_INLINE i32 IndexOf(u8 target, u8 startIndex) const noexcept
        {
            if (startIndex >= len) return -1;

            for (int i = startIndex; i < len; ++i)
                if (chars[i] == target) return i;

            return -1;
        }
        MAY_INLINE logic::Bool Contains(u8 target) const noexcept
        {
            return IndexOf(target) > 0;
        }
        MAY_INLINE logic::Bool Contains(u8 target, u8 startIndex) const noexcept
        {
            return IndexOf(target, startIndex) > 0;
        }
        MAY_INLINE logic::Bool NullOrEmpty() const noexcept
        {
            return *this == nullptr || len < 1;
        }
        MAY_INLINE logic::Bool EmptyOrWhitespace() const noexcept
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

    inline static constexpr i8 SMALL_STATIC_STR_TARGET_BYTE_SIZE = 16; // must be power of two
    inline static constexpr i8 SMALL_STATIC_STR_CAPACITY = SMALL_STATIC_STR_TARGET_BYTE_SIZE - (sizeof(u8) * 2);
    inline static constexpr i8 SMALL_STATIC_STR_MAX_INDEX = SMALL_STATIC_STR_CAPACITY - 1;
    class SmallStaticStr;
    typedef const SmallStaticStr SmallStr;      typedef SmallStaticStr mut_SmallStr;
    typedef const SmallStaticStr* SmallStrPtr; typedef SmallStaticStr* mut_SmallStrPtr;
    typedef const SmallStaticStr& SmallStrRef; typedef SmallStaticStr& mut_SmallStrRef;

    class alignas(8) SmallStaticStr
    {
    private:
        mut_u8 len = 0;
        mut_u8 chars[SMALL_STATIC_STR_CAPACITY];
        u8 NULL_TERMINATOR = NULL_TERM;

    public:
        INLINE static constexpr u8 ClampToBounds(u8 x)
        {
            return (x & SMALL_STATIC_STR_CAPACITY) > SMALL_STATIC_STR_MAX_INDEX ? SMALL_STATIC_STR_MAX_INDEX : x;
        }

        SmallStaticStr() { }
        SmallStaticStr(cstr c_str)
        {
            len = (u8)strnlen_s(c_str, SMALL_STATIC_STR_CAPACITY);
            memcpy_s(chars, SMALL_STATIC_STR_CAPACITY, c_str, len);
            chars[len] = 0;
        }
        SmallStaticStr(cstr c_str, u8 char_ct, u8 startIndex = 0)
        {
            u8 start = ClampToBounds(startIndex);
            len = (u8)strnlen_s(&c_str[start], ClampToBounds(char_ct));
            memcpy_s(chars, SMALL_STATIC_STR_CAPACITY, c_str, len);
            chars[len] = 0;
        }
        SmallStaticStr(SmallStrRef sstr)
        {
            len = sstr.len;
            memcpy_s(chars, len, sstr.chars, len);
            chars[len] = 0;
        }
        SmallStaticStr(SmallStrRef sstr, u8 char_ct, u8 startIndex = 0)
        {
            u8 start = ClampToBounds(startIndex);
            len = (u8)ClampToBounds(char_ct);

            memcpy_s(chars, len, sstr.chars + start, len);
            chars[len] = 0;
        }

        operator cstr() const noexcept { return (cstr)chars; }

        INLINE u8 Capacity() const noexcept { return SMALL_STATIC_STR_CAPACITY; }
        INLINE u8 Len() const noexcept { return len; }
        INLINE u8 PenultLen() const noexcept { return ClampToBounds(len - (u8)1); }
        INLINE utf8ptr Str() const noexcept { return chars; }
        INLINE logic::Bool Empty() const noexcept { return len == (u8)0; }
        INLINE void Clear()
        {
            len = 0;
            chars[0] = 0;
        }
        INLINE void Fill(u8 c, u8 ct = SMALL_STATIC_STR_CAPACITY)
        {
            len = ct;
            memset(chars, c, len);
        }

        INLINE u8& operator[](u32 index) const noexcept { return chars[index]; }
        INLINE SmallStrRef operator=(cstr rhs)
        {
            len = (u8)strnlen_s(rhs, STATIC_STR_CAPACITY);
            memcpy_s(chars, SMALL_STATIC_STR_CAPACITY, rhs, len);
            return *this;
        }
        INLINE SmallStrRef operator=(SmallStrRef rhs)
        {
            if (*this == rhs) return *this;

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            return *this;
        }

        INLINE SmallStrRef operator+=(cstr rhs)
        {
            if (len > SMALL_STATIC_STR_CAPACITY) return *this;

            i32 rhs_len = (i32)strnlen_s(rhs, SMALL_STATIC_STR_CAPACITY);
            i32 new_len = len + rhs_len;
            if (new_len > SMALL_STATIC_STR_CAPACITY)
                return *this;

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStrRef operator+=(SmallStrRef rhs)
        {
            if (len > SMALL_STATIC_STR_CAPACITY) return *this;

            i32 new_len = len + rhs.len;
            if (new_len > SMALL_STATIC_STR_CAPACITY)
                return *this;

            memcpy_s(chars + len, new_len, rhs.chars, rhs.len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE SmallStrRef operator+=(u8 c)
        {
            if (len > SMALL_STATIC_STR_CAPACITY) return *this;
            chars[len++] = c;
            return *this;
        }

        INLINE logic::Bool operator>(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs, len) > 0;
        }
        INLINE logic::Bool operator>=(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs, len) >= 0;
        }
        INLINE logic::Bool operator<(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs, len) < 0;
        }
        INLINE logic::Bool operator<=(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs, len) <= 0;
        }
        INLINE logic::Bool operator==(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs, len) == 0;
        }
        INLINE logic::Bool operator!=(SmallStrRef rhs) const noexcept
        {
            return memcmp(&len, rhs, len) != 0;
        }

        INLINE logic::Bool operator==(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, SMALL_STATIC_STR_CAPACITY);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) == 0;
#pragma warning( default : 6385)
        }
        INLINE logic::Bool operator!=(cstr rhs) const noexcept
        {
            i32 rhs_len = (i32)strnlen_s(rhs, SMALL_STATIC_STR_CAPACITY);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len != len || memcmp(chars, rhs, rhs_len) != 0;
#pragma warning( default : 6385)
        }

        INLINE logic::comp Compare(SmallStrRef arg) const noexcept
        {
            return memcmp(&len, arg, len);
        }
        INLINE logic::comp Compare(cstr c_str) const noexcept
        {
            return memcmp(chars, c_str, len);
        }

        INLINE cstr ToCstr() const noexcept { return (cstr)(chars); }
        INLINE mut_cstr ToMutCstr() const noexcept { return (mut_cstr)(chars); }
        INLINE ptr ToPtr() const noexcept { return &len; }
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
            memmove_s(chars, SMALL_STATIC_STR_CAPACITY, start, len);

            return *this;
        }
        INLINE SmallStrRef TrimEnd()
        {
            if (len < 1) return *this;

            const u8ptr fxd_end = chars + len - 1;
            u8ptr end = fxd_end;
            while (whitespace(--end));

            len -= (u8)(fxd_end - end);
            memmove_s(chars, SMALL_STATIC_STR_CAPACITY, chars, len);

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
            memmove_s(chars, SMALL_STATIC_STR_CAPACITY, start, len);

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
        INLINE logic::Bool Contains(u8 target) const noexcept
        {
            return IndexOf(target) > (u8)0;
        }
        INLINE logic::Bool Contains(u8 target, u8 startIndex) const noexcept
        {
            return IndexOf(target, startIndex) > (u8)0;
        }
        INLINE logic::Bool NullOrEmpty() const noexcept
        {
            return *this == nullptr || len < (u8)1;
        }
        INLINE logic::Bool EmptyOrWhitespace() const noexcept
        {
            if (len < (u8)1) return true;

            mut_i32 tmp_len = len;
            while (--tmp_len)
                if (!whitespace(chars[tmp_len])) return false;
            return true;
        }
    };
    INLINE static SmallStr operator +(SmallStrRef lhs, SmallStrRef rhs)
    {
        mut_SmallStr res(lhs);
        res += rhs;
        return res;
    }
    INLINE static SmallStr operator +(cstr lhs, SmallStrRef rhs)
    {
        mut_SmallStr res(lhs);
        res += rhs;
        return res;
    }

}