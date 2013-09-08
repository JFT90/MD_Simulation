#include "../include/sdl_app.h"

using namespace std;

sdl_app::sdl_app() {
    running = true;
    surf_display = NULL;

    surf_dot = NULL;
}

sdl_app::~sdl_app() {}

int sdl_app::on_execute(simulation* sim) {
	this->sim = sim;

    if(sdl_app::on_init() == false) {
        return -1;
    }

    SDL_Event event;

    int step = 0;

    while(running) {
        while(SDL_PollEvent(&event)) {
            sdl_app::OnEvent(&event);
        }

        on_loop();

        // draw only every 100th step
		step += 1;
		if (step>=100) {
			on_render();
			step = 0;
		}

    }

    on_cleanup();

    return 0;
}

bool sdl_app::on_init() {
    if(SDL_Init(SDL_INIT_EVERYTHING)<0) {
        return false;
    }
    TTF_Init();

    if((surf_display = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }

    if((surf_dot = surface::on_load("images/dot.bmp")) == NULL) {
        cerr << "error loading image (dot.bmp)" << endl;
        return false;
    }

    reset();

    return true;
}

void sdl_app::on_loop() {
	// update positions in simulation
	sim->main_step();
}

void sdl_app::on_render() {
    surface::draw_rectangle(surf_display, 0, 0, WIDTH, HEIGHT, 0, 0, 0);
    for(int i=0;i<sim->get_global_N();i++) {
		// draw
		double x = sim->get_r()[i].x*WIDTH*1.0/sim->get_global_L();
		double y = sim->get_r()[i].y*HEIGHT*1.0/sim->get_global_L();
		draw_dot(x,y);
    }

    SDL_Flip(surf_display);

    // to take a little stress from the CPU
    // SDL_Delay(100);
}

void sdl_app::OnEvent(SDL_Event *i_event) {
    event::OnEvent(i_event);
}

void sdl_app::OnExit() {
    running = false;
}

void sdl_app::on_cleanup() {
    SDL_FreeSurface(surf_dot);
    SDL_FreeSurface(surf_display);
    TTF_Quit();
    SDL_Quit();
}

void sdl_app::reset() {
}

void sdl_app::draw_dot(double x, double y) {
    surface::on_draw(surf_display, surf_dot, x, y);
}
