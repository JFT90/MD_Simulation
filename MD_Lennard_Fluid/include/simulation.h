#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>

using namespace std;

//#########################################################################
// Simulation parameters
extern const int global_L; // Kastenlänge
extern const int global_N; // Anzahl der Teilchen
extern const int global_T; // Temperatur
extern const double global_m; // Teilchenmasse in kg
extern const double t_step;
extern const double t_max;
extern const double t_equi;
extern const double t_measure;
//#########################################################################


extern const double kB; // Boltzmannkonstante

#ifndef BASE
#define BASE
struct base { // Structschablone für Ort oder Geschwindigkeit
    double x,y;
};
#endif // BASE

struct measurement {
    double E;
};

extern vector<base> r;
extern vector<base> r_old;
extern vector<base> v;
extern vector<base> F;

class simulation
{
	public:
		simulation();

		void equilibrate();
		bool main_step();

		double gettemp();
		void settemp(float T);

		measurement measure();

		vector<base> get_r();
		int get_global_N();
		int get_global_L();

		virtual ~simulation();
	protected:
	private:
		base get_force(base x);
		void calculate_forces();
		void init();
		void calculate_movement();
		void md_step();
};

#endif // SIMULATION_H
