#pragma once
#pragma warning(push, 0)// Prevent warnings from libraries I can't fix.

#include <iostream>
#include <limits>
#include <vector>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdint.h>

#include <SDL.h>

#include <gl/GL.h>

#include "rac-types.h"
#include "rac-mth.h"
#include "rac-logic.h"

#pragma warning(pop)

namespace rac::gl
{
	using namespace logic;
	using namespace mth;
	using namespace string;

	class ScreenInfo;
	typedef const ScreenInfo screen;		typedef ScreenInfo mut_screen;
	typedef const ScreenInfo& screen_ref;	typedef ScreenInfo& mut_screen_ref;
	typedef const ScreenInfo* screen_ptr;   typedef ScreenInfo* mut_screen_ptr;
	class ScreenInfo
	{
	public:
		mut_u32 pixelFormat = 0;
		mut_i32 width = 640;
		mut_i32 height = 480;
		mut_i32 refreshRate = 0;
		mut_f32 aspectRatio = 1.0f;

		ScreenInfo() { }
		ScreenInfo(f32 w, f32 h)
		{
			width = (i32)w;
			height = (i32)h;
			aspectRatio = w / h;
		}

		INLINE screen_ref operator=(screen_ref rhs)
		{
			memcpy_s(&pixelFormat, sizeof(ScreenInfo), &rhs.pixelFormat, sizeof(ScreenInfo));
			return *this;
		}
		INLINE screen operator *(f32 v) { return screen(width * v, height * v); }

		INLINE v2i SizeToV2I() const { return v2i(width, height); }
		INLINE v2i SizeToV2I(f32 v) const { return v2i(width * v, height * v); }
		constexpr void Scale(u32 multiplyBy, u32 divisor = 1)
		{
			if (multiplyBy == divisor || divisor == 0) return;
			width = (multiplyBy * width) / divisor;
			height = (multiplyBy * height) / divisor;
		}

		MAY_INLINE void InitSystemInfo()
		{
			SDL_DisplayMode DM;
			i32 SUCCESSFUL = 0;
			if (SDL_GetCurrentDisplayMode(0, &DM) == SUCCESSFUL)
			{
				pixelFormat = DM.format;
				width = DM.w;
				height = DM.h;
				refreshRate = DM.refresh_rate;
				aspectRatio = ((f32)width / (f32)height);
			}
			else
			{
				SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			}
		}
	};

	enum WindowInitResult
	{
		Succeeded = 0,
		WindowCreateFailed = -1,
		ContextCreateFailed = -2
	};

	class GL_Window;
	typedef const GL_Window Win;		typedef GL_Window mut_Win;
	typedef const GL_Window& Win_ref;	typedef GL_Window& mut_Win_ref;
	typedef const GL_Window* Win_ptr;   typedef GL_Window* mut_Win_ptr;
	class GL_Window
	{
	private:
		u32 SdlWindowFlagBitfield = SDL_WINDOW_OPENGL |
									SDL_WINDOW_MAXIMIZED |
									SDL_WINDOW_SHOWN |
									SDL_WINDOW_ALLOW_HIGHDPI;

	public:
		mut_SdlWin_ptr Main = nullptr;
		mut_SdlGlContext Context = nullptr;
		mut_v2i size = { 0 };
		mut_v2i pos = { 0 };
		str title;

		constexpr void SetViewport() { glViewport(0, 0, size.x, size.y); }
		constexpr const SDL_WindowFlags GetFlags() const
		{
			return (SDL_WindowFlags)SdlWindowFlagBitfield;
		}
		constexpr const SDL_WindowFlags GetFlags(mut_u32ptr out_u32_bitfield) const
		{
			*out_u32_bitfield = SdlWindowFlagBitfield;
			return (SDL_WindowFlags)SdlWindowFlagBitfield;
		}
		INLINE void Swap() const { SDL_GL_SwapWindow(Main); }

		// NOTE(RYAN_2024-04-26): Note, OPENGL is ASSUMED in this OPENGL library.
		MAY_INLINE WindowInitResult CreateCentered(str_ref winTitle, v2i_ref winSize)
		{
			Main = SDL_CreateWindow(winTitle.ToCstr(),
									SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
									winSize.x, winSize.y, GetFlags());
			if (Main == nullptr)
				return WindowCreateFailed;

			SDL_GetWindowPosition(Main, &pos.x, &pos.y);
			size = winSize;
			title = winTitle;

			Context = SDL_GL_CreateContext(Main);
			if (Context == nullptr)
				return ContextCreateFailed;

			return Succeeded;
		}

	};
}