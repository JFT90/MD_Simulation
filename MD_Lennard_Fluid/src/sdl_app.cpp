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
    int draw_step = 7; // draw only every x-th step

    while(running) {
        while(SDL_PollEvent(&event)) {
            sdl_app::OnEvent(&event);
        }

        on_loop();
		step += 1;

		if (step>=draw_step) {
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
	// /*
	static int i = 0;
	static int too_high = 0;
	static double E0 = 0;
	//*/
	sim->main_step();
	///*
	double energy_kin = sim->measure().E_kin;
	double energy_pot = sim->measure().E_pot;
	if (i==0){E0=energy_kin+energy_pot;}
	cout << "i=" << i << "\tenergy_kin: " << energy_kin << "\tpot: " << energy_pot << "\t= " << energy_kin + energy_pot;
	i++;
	if ((energy_kin+energy_pot)>E0*2) {too_high++; cout << " <-- !!!!!!!!" << endl;}
	else { cout << endl;}
	if ((energy_kin+energy_pot)<=E0*2) {too_high = 0;}

	if (too_high>=3) {
		cout << "energy too high three times in a row!"<<endl;
		char in[5];
		cin >> in;
		too_high = 0;}

	// breakpoint
	//*/
}

void sdl_app::on_render() {
    surface::draw_rectangle(surf_display, 0, 0, WIDTH, HEIGHT, 0, 180, 5);
    for(int i=0;i<sim->get_global_N();i++) {
		// draw
		double x = sim->get_r()[i].x/sim->get_global_L();
		double y = sim->get_r()[i].y/sim->get_global_L();
		x *= HEIGHT*1.0;
		y *= WIDTH*1.0;
		draw_dot((int) x,(int) y);
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

void sdl_app::draw_dot(int x, int y) {
    surface::on_draw(surf_display, surf_dot, x, y);
}
