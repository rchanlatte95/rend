#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <iostream>
#include <Windows.h>

#include "rac.h"
#include "rac-types.h"
#include "rac-mth.h"

#pragma warning(pop)

namespace rac::io
{
    using namespace rac::mth;

    class Console
    {
    public:
        static INLINE v2i16 GetConsoleSize()
        {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            i16 cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            i16 rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            return v2i16(cols, rows);
        }

        static INLINE i32 ClearLine(i32 chars)
        {
            _fputchar('\r');
            for (int i = 0; i < chars; ++i) { _fputchar(' '); }
            _fputchar('\r');
            return chars + 1;
        }

        static INLINE i32 ClearLine() { return ClearLine(GetConsoleSize().x); }

        static INLINE i32 Print(string::StrRef s)
        {
            u8ptr head = s.PtrToChars();
            mut_i32 len = s.Len();
            while (--len) { _fputchar(*head++); }
            return s.Len();
        }
        static INLINE i32 Println(string::StrRef s)
        {
            u8ptr head = s.PtrToChars();
            mut_i32 len = s.Len();
            while (--len) { _fputchar(*head++); }
            _fputchar('\r'); _fputchar('\n');
            return s.Len() + 2;
        }

        static INLINE i32 Print(char c) { return _fputchar(c); }
        static INLINE i32 Println(char c)
        {
            _fputchar(c); _fputchar('\r'); _fputchar('\n'); return 3;
        }
    };

}