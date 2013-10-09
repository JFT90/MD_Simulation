#ifndef SDL_APP_H_INCLUDED
#define SDL_APP_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>

#include "event.h"
#include "surface.h"

#include "simulation.h"

using namespace std;

#ifndef BASE
#define BASE
struct base {       // Structschablone für Ort oder Geschwindigkeit
    double x,y;
};
#endif // BASE

#define WIDTH 200
#define HEIGHT 200

class sdl_app : event {
    public:
        sdl_app();
        ~sdl_app();

        int on_execute(simulation* sim);
        bool on_init();
        void OnEvent(SDL_Event *event);
        void on_loop();
        void on_render();
        void on_cleanup();
        //overloaded events
        void OnExit();
        //void OnLButtonDown(int mX, int mY);
        //game specific
        void reset();

        void draw_dot(int x, int y);
    private:
        bool running;
        SDL_Surface* surf_display;

        simulation* sim;

        SDL_Surface* surf_dot;
};

#endif // SDL_APP_H_INCLUDED
