#include "rac-gl.h"

namespace rac::gl
{
    void ScreenInfo::InitSystemInfo()
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

    WindowInitResult GL_Window::CreateCentered(SmallStrRef winTitle, v2i_ref winSize)
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
}
