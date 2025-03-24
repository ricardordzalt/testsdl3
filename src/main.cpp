#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <cstring> // memcpy

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

struct PixelDelta
{
    int x;
    int y;
    uint32_t color;
};

// Estado simulado (posición de un bloque de 10x10)
int blockX = WIDTH / 2;
int blockY = HEIGHT / 2;
constexpr int BLOCK_SIZE = 10;

// Función mock que simula el modelo de IA
std::vector<PixelDelta> mockModelAI(SDL_Keycode key)
{
    std::vector<PixelDelta> deltas;

    // Borrar bloque anterior
    for (int y = 0; y < BLOCK_SIZE; ++y)
    {
        for (int x = 0; x < BLOCK_SIZE; ++x)
        {
            deltas.push_back({blockX + x, blockY + y, 0x000000FF}); // negro
        }
    }

    // Cambiar posición según la entrada
    std::cout << "Tipo de evento: " << key << std::endl;

    switch (key)
    {
    case SDLK_LEFT:
        blockX -= 10;
        break;
    case SDLK_RIGHT:
        blockX += 10;
        break;
    case SDLK_UP:
        blockY -= 10;
        break;
    case SDLK_DOWN:
        blockY += 10;
        break;
    default:
        break;
    }

    // Limitar dentro de ventana
    blockX = std::max(0, std::min(blockX, WIDTH - BLOCK_SIZE));
    blockY = std::max(0, std::min(blockY, HEIGHT - BLOCK_SIZE));

    // Dibujar bloque nuevo
    for (int y = 0; y < BLOCK_SIZE; ++y)
    {
        for (int x = 0; x < BLOCK_SIZE; ++x)
        {
            deltas.push_back({blockX + x, blockY + y, 0xFF0000FF}); // rojo
        }
    }

    return deltas;
}

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


    // Crear textura que representa el framebuffer
    SDL_Texture *frameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    // Framebuffer en CPU
    uint32_t framebuffer[HEIGHT][WIDTH] = {0};

    bool isApplicationRunning = true;
    SDL_Event sdlEvent;

    while (isApplicationRunning)
    {
        SDL_Keycode keyPressed = 0;

        while (SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_EVENT_QUIT)
            {
                isApplicationRunning = false;
            }
            else if (sdlEvent.type == SDL_EVENT_KEY_DOWN)
            {
                // keyPressed = sdlEvent.key.keysym.sym;
                SDL_Scancode scancode = sdlEvent.key.scancode;

                SDL_Keymod modstate = SDL_GetModState(); // ← obtiene los modificadores actuales

                // Usamos 'true' porque estamos en un evento de teclado
                SDL_Keycode keycode = SDL_GetKeyFromScancode(scancode, modstate, true);

                keyPressed = keycode;
            }
        }

        if (keyPressed)
        {
            // Obtener deltas del modelo de IA simulado
            std::vector<PixelDelta> deltas = mockModelAI(keyPressed);

            // Aplicar los cambios (deltas) al framebuffer
            for (const auto &delta : deltas)
            {
                if (delta.x >= 0 && delta.x < WIDTH && delta.y >= 0 && delta.y < HEIGHT)
                {
                    framebuffer[delta.y][delta.x] = delta.color;
                }
            }

            // Actualizar textura con el framebuffer modificado
            SDL_UpdateTexture(frameTexture, nullptr, framebuffer, WIDTH * sizeof(uint32_t));
        }

        // Dibujar
        SDL_RenderClear(sdlRenderer);
        SDL_RenderTexture(sdlRenderer, frameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);

        SDL_Delay(16); // ~60 fps
    }

    SDL_DestroyTexture(frameTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return 0;
}
