//
// Created by darkhead on 13/6/20.
//

#ifndef EVOL_TEXT_H
#define EVOL_TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Text {
public:
    int width;
    int height;

    static void writeText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, Uint32 color, std::string text)
    {
        SDL_Texture *textTexture;
        SDL_Rect textRect;
        this->getTextAndRect(renderer, x, y, const_cast<char *>(text.c_str()), font, &textTexture, &textRect, color);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    void getTextAndRect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, Uint32)
    {
        int renderer_w, renderer_h;
        int text_width;
        int text_height;

        SDL_GetRendererOutputSize(renderer, &renderer_w, &renderer_h);

        SDL_Color textColor = {
                (Uint8) 255,
                (Uint8) 255,
                (Uint8) 255,
                0
        };

        SDL_Surface *surface = TTF_RenderText_Solid(font, text, textColor);
        *texture = SDL_CreateTextureFromSurface(renderer, surface);

        text_width = surface->w;
        text_height = surface->h;
        SDL_FreeSurface(surface);

        rect->x = (float) x / this->width;
        rect->y = (float) y / this->height;

        rect->w = text_width;
        rect->h = text_height;
    }

};



#endif //EVOL_TEXT_H
