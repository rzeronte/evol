

#include <SDL_opengl.h>
#include <vector>
#include <iostream>
#include "include/Specimen.h"
#include "include/Loader.h"
#include "include/Drawable.h"
#include "species/alpiste.h"
#include "species/andreita.h"
#include "species/eduito.h"
#include "species/gordito.h"
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/examples/imgui_impl_sdl.h"

#include "include/Window.h"
#include "include/types.h"

#define MAX_BY_SPECIES 256

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main()
{
    std::vector<Specie> types;

    auto specie1 = alpiste();
    auto specie2 = andreita();
    auto specie3 = eduito();
    auto specie4 = gordito();

    types.push_back(specie1);
    types.push_back(specie3);
    //types.push_back(specie3);
    //types.push_back(specie4);

    std::vector< std::vector<Types::chartFrameInfo> > frameInfoSpecies;

    std::vector<Types::chartFrameInfo> frameInfoSpecie1;
    std::vector<Types::chartFrameInfo> frameInfoSpecie2;
    std::vector<Types::chartFrameInfo> frameInfoSpecie3;
    std::vector<Types::chartFrameInfo> frameInfoSpecie4;

    frameInfoSpecies.push_back( frameInfoSpecie1 );
    frameInfoSpecies.push_back( frameInfoSpecie2 );
    //frameInfoSpecies.push_back( frameInfoSpecie3 );
    //frameInfoSpecies.push_back( frameInfoSpecie4 );

    Window window(SCREEN_WIDTH, SCREEN_HEIGHT);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(window.window, window.contextOpenGL);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.WantCaptureMouse = false;
    io.WantCaptureKeyboard = false;

    // Setup style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameBorderSize = 1.0f;

    Timer timer;
    float globalTime;
    float lastTicks;

    bool finish = false;

    int screen_width = SCREEN_WIDTH;
    int screen_height = SCREEN_HEIGHT;

    std::vector<Specimen*> specimens;

    // start
    timer.start();
    // Load specimens
    Loader::LoadSimulationSpecimens(MAX_BY_SPECIES, &timer, screen_width, screen_height, types, specimens);

    // main loop
    while ( !finish ) {
        // capture events
        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            ImGui_ImplSDL2_ProcessEvent(&event);
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

        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame( window.window );
        ImGui::NewFrame();

        ImGui::Render();
        SDL_GL_MakeCurrent(window.window, window.contextOpenGL);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window.window);*/


        // loop specimens
        int dead = 0;

        for (int j = 0 ; j < specimens.size(); j++) {

            if (specimens[j]->dead)  {
                dead++;
                continue;
            }

            specimens[j]->specie.evaluate();
            specimens[j]->evaluate( window.videoBuffer, screen_width, screen_height, specimens );

            bool drawCenter = false;
            if (!specimens[j]->percepted->empty()) {
                drawCenter = true;
            }

            Drawable::DrawCircle(
                    window.videoBuffer,
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
            Drawable::createStatisticsChart(window.videoBuffer, globalTime, lastTicks, screen_width, screen_height, &timer, frameInfoSpecies[i], &types[i], specimens);
        }

        window.updateScreen();

        if (dead == specimens.size()-1) {
            for (int j = 0 ; j < specimens.size(); j++) {
                if (specimens[j]->dead == false) {
                    std::cout << "Ganan los " << specimens[j]->specie.name << std::endl << std::endl;
                }
            }
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
