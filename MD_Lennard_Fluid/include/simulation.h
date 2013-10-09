#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>

using namespace std;

#ifndef BASE
#define BASE
struct base { // Structschablone für Ort oder Geschwindigkeit
    double x,y;
};
#endif // BASE

struct measurement {
    double E_kin;
	double E_pot;
};

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
		double get_global_L();

		virtual ~simulation();
	protected:
	private:
		base get_conn_vector(base r1, base r2);
		base get_force(base x);
		double get_potential(base x);
		void calculate_forces();
		void init();
		void calculate_movement();
		void md_step();
		void init_r_old();

		double get_E_kin();
		double get_E_pot();
};

#endif // SIMULATION_H
