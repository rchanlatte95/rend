#pragma once
#include "rac-types.h"

namespace rac::logic
{
    class Boolean;
    typedef const Boolean Bool;     typedef Boolean mut_Bool;
    typedef const Bool* Bool_ptr;   typedef Bool* mut_Bool_ptr;
    typedef const Bool& Bool_ref;   typedef Bool& mut_Bool_ref;

    enum Comparison : Int8
    {
        Null = SCHAR_MIN,
        LessThan = -1,
        Equal = 0,
        GreaterThan = 1
    };
    class comp;
    typedef const comp* comp_ptr;   typedef comp* mut_comp_ptr;
    typedef const comp& comp_ref;   typedef comp& mut_comp_ref;

    class v2bool;
    typedef const v2bool* v2bool_ptr;   typedef const v2bool& v2bool_ref;
    typedef v2bool* mut_v2bool_ptr; typedef v2bool& mut_v2bool_ref;

    class v3bool;
    typedef const v3bool* v3bool_ptr;   typedef const v3bool& v3bool_ref;
    typedef v3bool* mut_v3bool_ptr; typedef v3bool& mut_v3bool_ref;

    class Boolean
    {
    private:
        mut_Int8 value = 0;

    public:
        Boolean() { value = 0; }
        Boolean(bool b) { value = b ? 1 : 0; }

        Boolean(i8 i) { value = i != 0 ? 1 : 0; }
        Boolean(u8 u) { value = u != 0 ? 1 : 0; }

        Boolean(i16 i) { value = i != 0 ? 1 : 0; }
        Boolean(u16 u) { value = u != 0 ? 1 : 0; }

        Boolean(i32 i) { value = i != 0 ? 1 : 0; }
        Boolean(u32 u) { value = u != 0 ? 1 : 0; }

        Boolean(i64 i) { value = i != 0 ? 1 : 0; }
        Boolean(u64 u) { value = u != 0 ? 1 : 0; }

        operator bool() const { return value != 0; }

        cstr Cstr() const { return value ? "true" : "false"; }
        Int8 ToInt() const { return value ? 1 : 0; }

        INLINE Bool_ref operator=(bool rhs)
        {
            value = rhs ? 1 : 0;
            return *this;
        }

        INLINE Bool_ref operator=(ptr rhs)
        {
            value = rhs != nullptr ? 1 : 0;
            return *this;
        }
    };

    class comp
    {
    private:
        Comparison comparison_value = Null;

    public:
        comp() {}

        comp(i8 val)
        {
            if (val == 0) comparison_value = Equal;
            else if (val < 0) comparison_value = LessThan;
            else comparison_value = GreaterThan;
        }
        comp(i16 val)
        {
            if (val == 0) comparison_value = Equal;
            else if (val < 0) comparison_value = LessThan;
            else comparison_value = GreaterThan;
        }
        comp(i32 val)
        {
            if (val == 0) comparison_value = Equal;
            else if (val < 0) comparison_value = LessThan;
            else comparison_value = GreaterThan;
        }
        comp(i64 val)
        {
            if (val == 0) comparison_value = Equal;
            else if (val < 0) comparison_value = LessThan;
            else comparison_value = GreaterThan;
        }

        const char* Cstr() const
        {
            switch (comparison_value)
            {
                case LessThan: return "Less Than";
                case Equal: return "Equal";
                case GreaterThan: return "Greater Than";
                default: return "Null";
            }
        }
    };

    class v2bool
    {
        Bool x = false;
        Bool y = false;
    };

    class v3bool
    {
        Bool x = false;
        Bool y = false;
        Bool z = false;
    };
}