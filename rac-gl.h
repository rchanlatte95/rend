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
#include "rac-logic.h"
#pragma warning(pop)

namespace rac::gl
{
	using namespace logic;

	class ScreenInfo;
	typedef const ScreenInfo screen;		typedef ScreenInfo mut_screen;
	typedef const ScreenInfo& screen_ref;	typedef ScreenInfo& mut_screen_ref;
	typedef const ScreenInfo* screen_ptr;   typedef ScreenInfo* mut_screen_ptr;
	class ScreenInfo
	{
	private:
		u32 SdlWindowFlagBitfield = SDL_WINDOW_OPENGL |
									SDL_WINDOW_MAXIMIZED |
									SDL_WINDOW_SHOWN |
									SDL_WINDOW_ALLOW_HIGHDPI;

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

		constexpr const SDL_WindowFlags GetFlags() const
		{
			return (SDL_WindowFlags)SdlWindowFlagBitfield;
		}
		constexpr const SDL_WindowFlags GetFlags(mut_u32ptr out_u32_bitfield) const
		{
			*out_u32_bitfield = SdlWindowFlagBitfield;
			return (SDL_WindowFlags)SdlWindowFlagBitfield;
		}

		INLINE screen_ref operator=(screen_ref rhs)
		{
			memcpy_s(&pixelFormat, sizeof(ScreenInfo), &rhs.pixelFormat, sizeof(ScreenInfo));
			return *this;
		}
		INLINE screen operator *(f32 v) { return screen(width * v, height * v); }

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

		constexpr void SetGlViewport() { glViewport(0, 0, width, height); }
	};

	// NOTE(RYAN_2024-04-26): Note, OPENGL is ASSUMED in this OPENGL library.
	mut_SdlWin_ptr CreateCenteredWindow(cstr caption, screen_ref screenParams)
	{
		return SDL_CreateWindow(caption,
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								screenParams.width, screenParams.height,
								screenParams.GetFlags());
	}
}