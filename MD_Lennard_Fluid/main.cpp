#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>
#include <string>
#include <fstream>

#include <SDL/SDL.h>

#include "include/sdl_app.h"
#include "include/simulation.h"

using namespace std;

int main() {
	simulation sim;

	sdl_app sdl_instance;

    return sdl_instance.on_execute(&sim);

    cout << "done!" << endl;
    return 0;
}
