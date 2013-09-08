#ifndef SURFACE_H
#define SURFACE_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>


class surface
{
    public:
        surface();

        static SDL_Surface* on_load(char* file);
        static bool on_draw(SDL_Surface* surf_dest, SDL_Surface* surf_src, int x, int y);
        static bool on_draw(SDL_Surface* surf_dest, SDL_Surface* surf_src, int x, int y, int x2, int y2, int w, int h);
        static bool transparent(SDL_Surface* surf_dest, int r, int g, int b);
        static bool draw_text(SDL_Surface* surf_dest, const char* text, char* font, int size, int x, int y, int w, int h, int r, int g, int b);
        static bool draw_rectangle(SDL_Surface* surf_dest, int x, int y, int w, int h , int r, int g, int b);
    protected:
    private:
};

#endif // SURFACE_H
