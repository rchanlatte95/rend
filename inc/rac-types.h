#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.
#include <iostream>
#include <limits>
#include <vector>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdint.h>

#include <gl/GL.h>
#pragma warning(pop)

#ifndef RAC
#define RAC

#define MAY_INLINE __inline
#define INLINE __forceinline
#define NO_INLINE __declspec(noinline)

typedef const void* ptr;	typedef void* mut_ptr;

typedef const char* cstr;		typedef char* mut_cstr;
typedef const char** cstrptr;	typedef char** mut_cstrptr;

typedef const int8_t i8;    typedef const uint8_t u8;
typedef int8_t mut_i8;      typedef uint8_t mut_u8;

typedef const int8_t sbyte;    typedef const uint8_t ubyte;
typedef int8_t mut_sbyte;      typedef uint8_t mut_ubyte;

typedef const int16_t i16;  typedef const uint16_t u16;
typedef int16_t mut_i16;    typedef uint16_t mut_u16;

typedef const int32_t i32;  typedef const uint32_t u32;
typedef int32_t mut_i32;    typedef uint32_t mut_u32;

typedef const int64_t i64;  typedef const uint64_t u64;
typedef int64_t mut_i64;    typedef uint64_t mut_u64;

typedef const int_fast8_t Int8;    typedef const uint_fast8_t UInt8;
typedef int_fast8_t mut_Int8;    typedef uint_fast8_t mut_UInt8;

typedef const int_fast16_t Int16;    typedef const uint_fast16_t UInt16;
typedef int_fast16_t mut_Int16;    typedef uint_fast16_t mut_UInt16;

typedef const int_fast32_t Int32;    typedef const uint_fast32_t UInt32;
typedef int_fast32_t mut_Int32;    typedef uint_fast32_t mut_UInt32;

typedef const int_fast64_t Int64;    typedef const uint_fast64_t UInt64;
typedef int_fast64_t mut_Int64;    typedef uint_fast64_t mut_UInt64;

typedef const float f32;    typedef const float& f32_ref;   typedef float mut_f32;
typedef const double f64;   typedef const double& f64_ref;  typedef double mut_f64;

typedef const uint8_t* utf8ptr; typedef const wchar_t* utf16ptr;
typedef uint8_t* mut_utf8ptr;   typedef wchar_t* mut_utf16ptr;

typedef const int8_t* i8ptr;    typedef int8_t* mut_i8ptr;
typedef const uint8_t* u8ptr;   typedef uint8_t* mut_u8ptr;

typedef const int16_t* i16ptr;  typedef int16_t* mut_i16ptr;
typedef const uint16_t* u16ptr; typedef uint16_t* mut_u16ptr;

typedef const int32_t* i32ptr;  typedef int32_t* mut_i32ptr;
typedef const uint32_t* u32ptr; typedef uint32_t* mut_u32ptr;

typedef const int64_t* i64ptr;  typedef int64_t* mut_i64ptr;
typedef const uint64_t* u64ptr; typedef uint64_t* mut_u64ptr;

typedef const float* f32ptr;  typedef int32_t* mut_f32ptr;
typedef const double* f64ptr; typedef uint32_t* mut_f64ptr;

#ifndef RAC_SDL_OPENGL_TYPES
#define RAC_SDL_OPENGL_TYPES

typedef const SDL_Window SdlWin; typedef SDL_Window mut_SdlWin;
typedef const SDL_Window* SdlWin_ptr; typedef SDL_Window* mut_SdlWin_ptr;

typedef const SDL_GLContext SdlGlContext; typedef SDL_GLContext mut_SdlGlContext;
typedef const SDL_GLContext* SdlGlContext_ptr; typedef SDL_GLContext* mut_SdlGlContext_ptr;

typedef const SDL_Event SdlEvent; typedef SDL_Event mut_SdlEvent;
typedef const SDL_Event* SdlEvent_ptr; typedef SDL_Event mut_SdlEvent_ptr;

#endif

#endif