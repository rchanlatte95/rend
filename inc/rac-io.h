#pragma once

#include "rac-types.h"
#include "rac-str.h"

namespace rac::io
{
    static INLINE int Print(string::Str_ref s)
    {
        u8ptr head = s.PtrToChars();
        mut_i32 len = s.Length();
        while (--len) { _fputchar(*head++); }
        return s.Length();
    }
    static INLINE int Println(string::Str_ref s)
    {
        u8ptr head = s.PtrToChars();
        mut_i32 len = s.Length();
        while (--len) { _fputchar(*head++); }
        _fputchar('\r'); _fputchar('\n');
        return s.Length() + 2;
    }

    static INLINE int Print(char c) { return _fputchar(c); }
    static INLINE int Println(char c)
    {
        _fputchar(c); _fputchar('\r'); _fputchar('\n'); return 3;
    }
}