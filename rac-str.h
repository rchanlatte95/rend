#pragma once

#include "rac-types.h"
#include "rac-logic.h"

namespace rac::string
{
    class str;
    typedef const str* str_ptr;   typedef str* mut_str_ptr;
    typedef const str& str_ref;   typedef str& mut_str_ref;

    #define RAC_DIGIT_TO_CHAR(x) (x + 48)
    const i32 SSTR_BASE_BYTE_SIZE = 1024;
    const i32 SSTR_CAPACITY = SSTR_BASE_BYTE_SIZE - sizeof(i32);
    const i32 SSTR_MAX_LEN = SSTR_CAPACITY - 1;

    // https://en.wikipedia.org/wiki/Whitespace_character
    const u8 CHAR_TAB = 0x09;
    const u8 LINE_FEED = 0x0A;
    const u8 LINE_TAB = 0x0B;
    const u8 FORM_FEED = 0x0C;
    const u8 CARRIAGE_RETURN = 0x0D;
    const u8 SPACE = 0x20;
    const u8 NEXT_LINE = 0x85;
    const u8 NO_BREAK_SPACE = 0xA0;

    static constexpr i32 clamp_len(i32 x)
    {
        if (x > SSTR_MAX_LEN) return SSTR_MAX_LEN;
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

    class str
    {
    private:
        mut_i32 len = 0;
        mut_u8 chars[SSTR_CAPACITY];

    public:
        str()
        {
            len = 0;
            chars[0] = 0;
            chars[SSTR_MAX_LEN] = 0;
        }

        str(cstr _str)
        {
            len = (i32)strnlen_s(_str, SSTR_MAX_LEN);
            chars[0] = 0;
            chars[SSTR_MAX_LEN] = 0;
            memcpy_s(chars, SSTR_MAX_LEN, _str, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)
        }

        str(cstr _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);
            len = (i32)strnlen_s(&_str[start], clamp_len(char_ct));
            chars[0] = 0;
            chars[SSTR_MAX_LEN] = 0;
            memcpy_s(chars, SSTR_MAX_LEN, _str, len);
        }

        str(str_ptr _str)
        {
            len = _str->len;
            memcpy_s(chars, len, _str->chars, len);
            chars[len] = 0;
            chars[SSTR_MAX_LEN] = 0;
        }

        str(str_ref _str)
        {
            len = _str.len;
            memcpy_s(chars, len, _str.chars, len);
            chars[len] = 0;
            chars[SSTR_MAX_LEN] = 0;
        }

        str(str_ptr _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);

            len = (i32)clamp_len(char_ct);
            memcpy_s(chars, len, &_str->chars[start], len);
            chars[len] = 0;
            chars[SSTR_MAX_LEN] = 0;
        }

        str(str_ref _str, i32 startIndex, i32 char_ct)
        {
            i32 start = clamp_len(startIndex);

            len = (i32)clamp_len(char_ct);
            memcpy_s(chars, len, &_str.chars[start], len);
            chars[len] = 0;
            chars[SSTR_MAX_LEN] = 0;
        }

        operator cstr() const { return (cstr)chars; }

        INLINE i32 MaxLength() const { return SSTR_MAX_LEN; }
        INLINE i32 Capacity() const { return SSTR_CAPACITY; }
        INLINE i32 Length() const { return len; }
        INLINE utf8ptr Str() const { return &chars[0]; }
        INLINE bool Empty() const { return len == 0; }
        INLINE void Clear() { len = 0; chars[0] = 0; }
        INLINE void Fill(u8 c, i32 ct = SSTR_MAX_LEN)
        {
            len = ct;
            memset(chars, c, len);
        }
        INLINE u8& operator[](i32 index) { return chars[index]; }
        INLINE str_ref operator=(cstr rhs)
        {
            len = (i32)strnlen_s(rhs, SSTR_MAX_LEN);
            chars[0] = 0;
            chars[SSTR_MAX_LEN] = 0;
            memcpy_s(chars, SSTR_MAX_LEN, rhs, len);

#pragma warning( disable : 6386) // Erroneous Buffer overrun warning
            chars[len] = 0;
#pragma warning( default : 6386)

            return *this;
        }
        INLINE str_ref operator=(str_ref rhs)
        {
            if (*this == rhs) { return *this; }

            len = rhs.len;
            memcpy_s(chars, len, rhs.chars, len);
            chars[len] = 0;
            chars[SSTR_MAX_LEN] = 0;

            return *this;
        }

        INLINE str_ref operator+=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, SSTR_MAX_LEN);
            i32 new_len = len + rhs_len;
            if (len == SSTR_MAX_LEN || new_len >= SSTR_MAX_LEN)
            {
                return *this;
            }

            memcpy_s(chars + len, new_len, rhs, rhs_len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE str_ref operator+=(str_ref rhs)
        {
            i32 new_len = len + rhs.len;
            if (len == SSTR_MAX_LEN || new_len >= SSTR_MAX_LEN)
            {
                return *this;
            }

            memcpy_s(chars + len, new_len, rhs.chars, rhs.len);
            len = new_len;
            chars[len] = 0;

            return *this;
        }
        INLINE str_ref operator+=(u8 c)
        {
            if (len == SSTR_MAX_LEN) return *this;
            chars[len++] = c;
            chars[len] = 0;
            return *this;
        }

        INLINE bool operator>(str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) > 0;
        }
        INLINE bool operator>=(str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) >= 0;
        }
        INLINE bool operator<(str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) < 0;
        }
        INLINE bool operator<=(str_ref rhs)
        {
            return memcmp(ptr(&len), ptr(&rhs.len), len) <= 0;
        }
        INLINE bool operator==(str_ref rhs)
        {
            return memcmp(&len, rhs.ToRef(), len) == 0;
        }
        INLINE bool operator!=(str_ref rhs)
        {
            return memcmp(&len, rhs.ToRef(), len) != 0;
        }

        INLINE bool operator==(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, SSTR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) == 0;
#pragma warning( default : 6385)
        }
        INLINE bool operator!=(cstr rhs)
        {
            i32 rhs_len = (i32)strnlen_s(rhs, SSTR_MAX_LEN);

#pragma warning( disable : 6385) // Erroneous invalid data read warning
            return rhs_len == len && memcmp(chars, rhs, rhs_len) != 0;
#pragma warning( default : 6385)
        }

        INLINE logic::comp Compare(str_ref arg)
        {
            return logic::comp(memcmp(ptr(&len), ptr(&arg.len), len));
        }
        INLINE logic::comp Compare(cstr c_str)
        {
            return logic::comp(memcmp(ptr(&chars[0]), ptr(c_str), len));
        }

        INLINE cstr ToCstr() const { return (cstr)(&chars[0]); }
        INLINE mut_cstr ToMutCstr() { return (mut_cstr)(&chars[0]); }
        INLINE ptr ToPtr() const { return (ptr)(&len); }
        INLINE u8ptr PtrToChars() const { return chars; }
        INLINE str_ref ToRef() const { return *this; }

        INLINE u8& First() const { return chars[0]; }
        INLINE u8& Last() const { return chars[len - 1]; }
        INLINE str_ref TrimStart()
        {
            if (len < 1) return *this;

            mut_u8ptr start = chars - 1;
            while (whitespace(++start));

            len -= (i32)(start - chars);
            memmove_s(chars, SSTR_MAX_LEN, start, len);
            chars[len] = 0;

            return *this;
        }
        INLINE str_ref TrimEnd()
        {
            if (len < 1) return *this;

            const u8ptr fxd_end = chars + len - 1;
            u8ptr end = fxd_end;
            while (whitespace(--end));

            len -= (i32)(fxd_end - end);
            memmove_s(chars, SSTR_MAX_LEN, chars, len);
            chars[len] = 0;

            return *this;
        }
        INLINE str_ref Trim()
        {
            if (len < 1) return *this;

            u8ptr start = chars - 1;
            u8ptr end = chars + len - 1;

            while (whitespace(++start));
            while (whitespace(--end));

            len = (i32)(end - start) + 1;
            memmove_s(chars, SSTR_MAX_LEN, start, len);
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

    INLINE static str operator +(str_ref lhs, str_ref rhs)
    {
        str res(lhs);
        res += rhs;
        return res;
    }
    INLINE static str operator +(cstr lhs, str_ref rhs)
    {
        str res(lhs);
        res += rhs;
        return res;
    }
}