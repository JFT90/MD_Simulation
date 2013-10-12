#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>

#include "vek.h"

using namespace std;

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

		vector<vek> get_r();
		int get_global_N();
		double get_global_L();

		virtual ~simulation();
	protected:
	private:
		vek get_conn_vector(vek r1, vek r2);
		vek get_force(vek x);
		double get_potential(vek x);
		void calculate_forces();
		void init();
		void calculate_movement();
		void md_step();
		void init_r_old();

		double get_E_kin();
		double get_E_pot();
};

#endif // SIMULATION_H
