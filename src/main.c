
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL.h"

/* TODO: img.c */
/* Try test/img2.h for different resolution */
#include "test/img.h"

#define FPS 30

void die(char* s) {
    fprintf(stderr, s);
    SDL_Quit();
    exit(1);
}

int main() {
    // Start SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        die("Unable to start SDL\n");

    // Create window
    SDL_Window* sdl_window =
      SDL_CreateWindow("Image preview", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!sdl_window)
        die("Error creating a window\n");

    // Create renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, -1, render_flags);
    if (!sdl_renderer) {
        SDL_DestroyWindow(sdl_window);
        die("Error creating a renderer\n");
    }

    printf("Press <Esc> or <q> to exit...\n");

    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);

    // Save original ptr for restoring it (since the macro is going to alter it)
    char* data_original = header_data;

    // For key events
    SDL_Event sdl_event;

    // Current pixel
    SDL_Rect px;
    px.w = 1;
    px.h = 1;

    // Cur color
    unsigned char col[4];
    col[3] = 255;    // Full alpha by default

    int run = 1;
    while (run) {
        // Events
        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_KEYDOWN:
                    // Check the pressed key
                    switch (sdl_event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            run = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // Draw eax pixel with the color
        for (px.y = 0; px.y < height; px.y++) {
            for (px.x = 0; px.x < width; px.x++) {
                // Get the color from the data using macro
                HEADER_PIXEL(header_data, col);

                SDL_SetRenderDrawColor(sdl_renderer, col[0], col[1], col[2], col[3]);
                SDL_RenderFillRect(sdl_renderer, &px);
            }
        }

        // Reset data ptr
        header_data = data_original;

        // Send to renderer
        SDL_RenderPresent(sdl_renderer);

        SDL_Delay(FPS / 1000);
    }

    return 0;
}
