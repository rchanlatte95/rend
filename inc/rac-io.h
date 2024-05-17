#pragma once

#include "rac-types.h"
#include "rac-str.h"

namespace rac::io
{
    static INLINE int Print(string::StrRef s)
    {
        u8ptr head = s.PtrToChars();
        mut_i32 len = s.Len();
        while (--len) { _fputchar(*head++); }
        return s.Len();
    }
    static INLINE int Println(string::StrRef s)
    {
        u8ptr head = s.PtrToChars();
        mut_i32 len = s.Len();
        while (--len) { _fputchar(*head++); }
        _fputchar('\r'); _fputchar('\n');
        return s.Len() + 2;
    }

    static INLINE int Print(char c) { return _fputchar(c); }
    static INLINE int Println(char c)
    {
        _fputchar(c); _fputchar('\r'); _fputchar('\n'); return 3;
    }
}