
#include <SDL.h>
#include <vector>
#include <iostream>
#include "include/Specimen.h"
#include "include/Loader.h"
#include "include/Drawable.h"
#include "species/alpiste.h"
#include "species/andreita.h"
#include "species/eduito.h"
#include "species/gordito.h"

#define MAX_BY_SPECIES 512

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

auto specie1 = alpiste();
auto specie2 = andreita();
auto specie3 = eduito();
auto specie4 = gordito();

std::vector<Specie> types;

float globalTime;
float lastTicks;

struct chartFrameInfo {
    float second;
    int population;
};

void createStatisticsChart(Uint32 *videoBuffer, Timer *timer, std::vector<chartFrameInfo> &frameInfo, Specie *specie, std::vector<Specimen*> &specimens)
{
    if ( frameInfo.size() >= SCREEN_WIDTH ) {
        frameInfo.clear();
        globalTime = 0;
    }

    float ticks     = timer->getTicks();
    float deltatime = ticks - lastTicks;

    lastTicks = ticks;

    globalTime += (deltatime / 1000);

    int startX = 0;
    int startY = SCREEN_HEIGHT;


    int reductor = 4;
    for (int i = 0; i < frameInfo.size(); i++) {
        Drawable::drawPixel( videoBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, startX + frameInfo[i].second, startY - 1 - frameInfo[i].population/reductor, specie->morphology.color );
        Drawable::drawPixel( videoBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, startX + frameInfo[i].second, startY - 1 - frameInfo[i].population/reductor+1, specie->morphology.color );
        Drawable::drawPixel( videoBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, startX + frameInfo[i].second, startY - 1 - frameInfo[i].population/reductor+2, specie->morphology.color );
    }

    int contSpecie = 0;
    for (int i = 0; i < specimens.size(); i++) {
        if (specimens[i]->specie.name == specie->name && !specimens[i]->dead) {
            contSpecie++;
        }
    }

    chartFrameInfo cInfo;
    cInfo.population = contSpecie;
    cInfo.second = frameInfo.size();

    frameInfo.push_back( cInfo );
}

int main()
{
    types.push_back(specie1);
    types.push_back(specie2);
    types.push_back(specie3);
    types.push_back(specie4);

    std::vector< std::vector<chartFrameInfo> > frameInfoSpecies;

    std::vector<chartFrameInfo> frameInfoSpecie1;
    std::vector<chartFrameInfo> frameInfoSpecie2;
    std::vector<chartFrameInfo> frameInfoSpecie3;
    std::vector<chartFrameInfo> frameInfoSpecie4;

    frameInfoSpecies.push_back( frameInfoSpecie1 );
    frameInfoSpecies.push_back( frameInfoSpecie2 );
    frameInfoSpecies.push_back( frameInfoSpecie3 );
    frameInfoSpecies.push_back( frameInfoSpecie4 );

    SDL_Window    *window;
    SDL_Renderer  *renderer;
    SDL_Surface   *screenSurface;
    SDL_Texture   *screenTexture;

    Timer timer;
    bool finish = false;

    int screen_width = SCREEN_WIDTH;
    int screen_height = SCREEN_HEIGHT;

    int sizeBuffers = screen_width*screen_height;
    auto *videoBuffer = new Uint32[sizeBuffers];

    std::vector<Specimen*> specimens;

    int total_size = 0;

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
                screen_width,
                screen_height,
                SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC |SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
        );

        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            exit(-1);
        } else {

            screenSurface = SDL_CreateRGBSurface(0, screen_width, screen_height, 32, 0, 0, 0, 0);
            SDL_SetSurfaceBlendMode(screenSurface, SDL_BLENDMODE_BLEND);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

            screenTexture = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ARGB8888,
                                               SDL_TEXTUREACCESS_STATIC,
                                               screen_width,screen_height
            );

        }
    }

    // start
    timer.start();

    // Load specimens
    Loader::LoadSimulationSpecimens(MAX_BY_SPECIES, &timer, screen_width, screen_height, types, specimens);

    // main loop
    while ( !finish ) {

        // capture events
        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                finish = true;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                Loader::ResetSimulation(MAX_BY_SPECIES, &timer, screen_width, screen_height, types, specimens);
                for (int j = 0 ; j < frameInfoSpecies.size(); j++) {
                    frameInfoSpecies[j].clear();
                }
                globalTime = 0;
            }
        }

        // loop specimens
        int dead = 0;
        int total_size = 0;
        for (int i = 0; i < specimens.size() ; i++) {
            total_size+= (int) specimens[i]->specie.genome.size;
        }

        for (int j = 0 ; j < specimens.size(); j++) {

            if (specimens[j]->dead)  {
                dead++;
                continue;
            }
            specimens[j]->specie.evaluate();
            specimens[j]->evaluate( videoBuffer, screen_width, screen_height, specimens, total_size);

            bool drawCenter = false;
            if (!specimens[j]->percepted->empty()) {
                drawCenter = true;
            }

            Drawable::DrawCircle(
                    videoBuffer,
                    screen_width,
                    screen_height,
                    specimens[j]->position.x,
                    specimens[j]->position.y,
                    specimens[j]->specie.genome.size,
                    specimens[j]->base_color,
                    drawCenter,
                    specimens[j]->isHungry()
            );

        }

        // draw population chart
        for (int i = 0; i < types.size(); i++) {
            createStatisticsChart(videoBuffer, &timer, frameInfoSpecies[i], &types[i], specimens);
        }

        // Update Window
        SDL_UpdateTexture( screenTexture, NULL, videoBuffer, screenSurface->pitch );
        SDL_RenderCopy( renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent( renderer );

        // Clear Buffer
        std::fill(videoBuffer, videoBuffer + sizeBuffers, NULL);

        if (dead == specimens.size()) {
            Loader::ResetSimulation(MAX_BY_SPECIES, &timer, screen_width, screen_height, types, specimens);
            for (int j = 0 ; j < frameInfoSpecies.size(); j++) {
                frameInfoSpecies[j].clear();
            }
            globalTime = 0;
        }
    }

    printf("bye!");

    return 0;
}
