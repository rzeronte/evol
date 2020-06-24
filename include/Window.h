//
// Created by darkhead on 17/6/20.
//

#ifndef EVOL_WINDOW_H
#define EVOL_WINDOW_H

#include <SDL.h>
#include <algorithm>
#include <SDL_ttf.h>

class Window {
    int width;
    int height;

    SDL_Renderer  *renderer;
    SDL_Surface   *screenSurface;
    SDL_Texture   *screenTexture;

    int sizeBuffers;

public:
    Window(int width, int height) : width(width), height(height)
    {
        sizeBuffers = width*height;
        videoBuffer = new Uint32[sizeBuffers];

        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        //Initialize SDL
        if (SDL_Init( SDL_INIT_EVERYTHING) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            exit(-1);
        } else {

            //Create window
            window = SDL_CreateWindow(
                    "Evol",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    this->width,
                    this->height,
                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
            );

            if (window == NULL) {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                exit(-1);
            } else {
                contextOpenGL = SDL_GL_CreateContext(window);

                screenSurface = SDL_CreateRGBSurface(0, this->width, this->height, 32, 0, 0, 0, 0);
                SDL_SetSurfaceBlendMode(screenSurface, SDL_BLENDMODE_BLEND);
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

                screenTexture = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB8888,
                                                   SDL_TEXTUREACCESS_STATIC,
                                                   this->width,this->height
                );

                SDL_GL_SetSwapInterval(1); // Enable vsync
                SDL_GL_SwapWindow(window);

            }
        }
    }

    void updateScreen()
    {
        // Update Window
        SDL_UpdateTexture( screenTexture, NULL, videoBuffer, screenSurface->pitch );
        SDL_RenderCopy( renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent( renderer );

        // Clear Buffer
        std::fill(videoBuffer, videoBuffer + sizeBuffers, NULL);

    }

    Uint32 *videoBuffer;
    SDL_Window    *window;
    SDL_GLContext contextOpenGL;
};
#endif //EVOL_WINDOW_H
