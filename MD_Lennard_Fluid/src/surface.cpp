#include "../include/surface.h"

#include <iostream>

surface::surface()
{
    //ctor
}

SDL_Surface* surface::on_load(char* file) {
    SDL_Surface* surf_temp = NULL;
    SDL_Surface* surf_return = NULL;

    if((surf_temp = SDL_LoadBMP(file)) == NULL) { return NULL; }

    surf_return = SDL_DisplayFormat(surf_temp);
    SDL_FreeSurface(surf_temp);

    return surf_return;
}

bool surface::on_draw(SDL_Surface* surf_dest, SDL_Surface* surf_src, int x, int y) {
    if(surf_dest == NULL || surf_src == NULL) { return false; }

    SDL_Rect dest_r;
    dest_r.x = x;
    dest_r.y = y;

    SDL_BlitSurface(surf_src, NULL, surf_dest, &dest_r);

    return true;
}

bool surface::on_draw(SDL_Surface* surf_dest, SDL_Surface* surf_src, int x, int y, int x2, int y2, int w, int h) {
    if(surf_dest == NULL || surf_src == NULL) { return false; }

    SDL_Rect dest_r;
    dest_r.x = x;
    dest_r.y = y;

    SDL_Rect src_r;
    src_r.x = x2;
    src_r.y = y2;
    src_r.w = w;
    src_r.h = h;

    SDL_BlitSurface(surf_src, &src_r, surf_dest, &dest_r);

    return true;
}

bool surface::transparent(SDL_Surface* surf_dest, int r, int g, int b) {
    if(surf_dest == NULL) { return false; }

    SDL_SetColorKey(surf_dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(surf_dest->format, r, g, b));

    return true;
}

bool surface::draw_text(SDL_Surface* surf_dest, const char* text, char* font, int size, int x, int y, int h, int w, int r, int g, int b) {
    TTF_Font* t_font = TTF_OpenFont(font, size);
    SDL_Color color = { r, g, b };
    SDL_Rect rect = {x, y, h, w};
    SDL_Surface* surf_text = TTF_RenderText_Solid(t_font, text, color);
    SDL_BlitSurface(surf_text, NULL, surf_dest, &rect);
    SDL_FreeSurface(surf_text);

    TTF_CloseFont(t_font);
}

bool surface::draw_rectangle(SDL_Surface* surf_dest, int x, int y, int w, int h , int r, int g, int b) {
    boxRGBA(surf_dest, x, y, x+w, y+h, r, g, b, 255);
}
