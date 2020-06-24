//
// Created by darkhead on 12/6/20.
//

#ifndef EVOL_DRAWABLE_H
#define EVOL_DRAWABLE_H

#include <SDL_types.h>
#include "Colors.h"
#include "types.h"

class Drawable {
public:

    static void drawPixel(Uint32 *videoBuffer, int screen_width, int screen_height, int x, int y, Uint32 value)
    {
        if ( (x <= 0 || x >= screen_width) || ( y <= 0 || y >= screen_height) )  {
            return;
        }
        videoBuffer[y * screen_width + x] = value;
    }

    static void drawLine2D(int x1, int y1, int x2, int y2, Uint32 *videoBuffer, int screen_width, int screen_height, Uint32 color)
    {
        if (x1 < 0 || x1 > screen_width) {
            return;
        }
        if (x2 < 0 || x2 > screen_width) {
            return;
        }
        if (y1 < 0 || y1 > screen_height) {
            return;
        }
        if (y2 < 0 || y1 > screen_height) {
            return;
        }

        Uint32 col = color;
        int pasoy;
        int pasox;
        int deltaX = (x2 - x1);
        int deltaY = (y2 - y1);

        if (deltaY < 0) {
            deltaY = -deltaY;
            pasoy = -1;
        } else {
            pasoy = 1;
        }


        if (deltaX < 0) {
            deltaX = -deltaX;
            pasox = -1;
        } else {
            pasox = 1;
        }

        int x = x1;
        int y = y1;

        if (deltaX>deltaY){
            int p = 2 * deltaY - deltaX;
            int incE = 2 * deltaY;
            int incNE = 2 * (deltaY - deltaX);
            while (x != x2){
                x = x + pasox;
                if (p < 0){
                    p = p + incE;
                } else {
                    y = y + pasoy;
                    p = p + incNE;
                }

                drawPixel(videoBuffer, screen_width, screen_height, x, y, col);

            }
        } else{
            int p = 2 * deltaX - deltaY;
            int incE = 2 * deltaX;
            int incNE = 2 * (deltaX - deltaY);
            while (y != y2){
                y = y + pasoy;
                if (p < 0) {
                    p = p + incE;
                } else {
                    x = x + pasox;
                    p = p + incNE;
                }

                drawPixel(videoBuffer, screen_width, screen_height, x, y, col);

            }
        }
    }


    static void DrawCircle(Uint32 *videoBuffer, int screen_width, int screen_height, int32_t centreX, int32_t centreY, int32_t radius, Uint32 color, bool center, bool cruise)
    {
        const int32_t diameter = (radius * 2);

        int32_t x = (radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        if (cruise) {
            for (int k = 0; k < radius/2; k++) {
                drawPixel(videoBuffer, screen_width, screen_height, centreX-k, centreY, color );
                drawPixel(videoBuffer, screen_width, screen_height, centreX+k, centreY, color );
            }
        }

        if (center) {
            drawPixel( videoBuffer, screen_width, screen_height, centreX, centreY, Colors::white() );
        }

        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            drawPixel(videoBuffer, screen_width, screen_height, centreX + x, centreY - y, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX + x, centreY + y, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX - x, centreY - y, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX - x, centreY + y, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX + y, centreY - x, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX + y, centreY + x, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX - y, centreY - x, color);
            drawPixel(videoBuffer, screen_width, screen_height, centreX - y, centreY + x, color);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }

    static void createStatisticsChart(Uint32 *videoBuffer, float globalTime, float lastTicks, int screen_width, int screen_height, Timer *timer, std::vector<Types::chartFrameInfo> &frameInfo, Specie *specie, std::vector<Specimen*> &specimens)
    {
        if ( frameInfo.size() >= screen_width ) {
            frameInfo.clear();
            globalTime = 0;
        }

        float ticks     = timer->getTicks();
        float deltatime = ticks - lastTicks;

        lastTicks = ticks;

        globalTime += (deltatime / 1000);

        int startX = 0;
        int startY = screen_height;


        int reductor = 4;
        for (int i = 0; i < frameInfo.size(); i++) {
            Drawable::drawPixel( videoBuffer, screen_width, screen_height, startX + frameInfo[i].second, startY - 1 - frameInfo[i].population/reductor, specie->morphology.color );
            Drawable::drawPixel( videoBuffer, screen_width, screen_height, startX + frameInfo[i].second, startY - 1 - frameInfo[i].population/reductor+1, specie->morphology.color );
            Drawable::drawPixel( videoBuffer, screen_width, screen_height, startX + frameInfo[i].second, startY - 1 - frameInfo[i].population/reductor+2, specie->morphology.color );
        }

        int contSpecie = 0;
        for (int i = 0; i < specimens.size(); i++) {
            if (specimens[i]->specie.name == specie->name && !specimens[i]->dead) {
                contSpecie++;
            }
        }

        Types::chartFrameInfo cInfo;
        cInfo.population = contSpecie;
        cInfo.second = frameInfo.size();

        frameInfo.push_back( cInfo );
    }
};
#endif //EVOL_DRAWABLE_H
