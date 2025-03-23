#include <SDL3/SDL.h>
#include <iostream>

int main()
{
    bool isSdlVideoInitialized = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if (!isSdlVideoInitialized)
    {
        const char* sdlInitError = SDL_GetError();
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error al inicializar SDL3: %s", sdlInitError);
        return 1;
    }

    SDL_Window *sdlWindow = SDL_CreateWindow("Ventana SDL3", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!sdlWindow)
    {
        const char* sdlWindowError = SDL_GetError();

        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Error al crear la ventana: %s", sdlWindowError);
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, nullptr);
    if (!sdlRenderer)
    {
        const char* sdlRendererError = SDL_GetError();

        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error al crear el renderer: %s", sdlRendererError);
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        return 1;
    }

    bool isApplicationRunning = true;
    SDL_Event sdlEvent;

    while (isApplicationRunning)
    {
        while (SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_EVENT_QUIT)
            {
                isApplicationRunning = false;
            }
        }

        SDL_SetRenderDrawColor(sdlRenderer, 0, 128, 255, 255);
        SDL_RenderClear(sdlRenderer);
        SDL_RenderPresent(sdlRenderer);
    }

    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return 0;
}
