#include "../include/simulation.h"


//#########################################################################
// Simulation parameters
const double global_L = 8; // Kastenlänge
const int global_N = 16; // Anzahl der Teilchen
const double global_T = 8; // Temperatur
const double global_m = 1; // Teilchenmasse in kg
const double t_step = 0.001;
const double t_max = 1e7*t_step;
const double t_equi = 100*t_step;
const double t_measure = 1000*t_step;
//#########################################################################

const double kB = 1; // Boltzmannkonstante
// const double kB=1.3806488e-23; // Boltzmannkonstante

vector<vek> r;
vector<vek> r_old;
vector<vek> v;
vector<vek> F;

simulation::simulation() {
	// ctor
	// initialize random generator
	// srand(time(NULL));

    srand(4); //debug
    init();
    settemp(global_T);
    // std::cout << "temp_init:" << gettemp() << endl;
    init_r_old();

	//debug
    // equilibrate();
}

double simulation::gettemp() {
    // gibt aktuelle Temperatur des Systems in Kelvin zurück
    double temp = 0;
    double sum;
    for(int i=0 ; i<global_N ; i++) {
    	sum += v[i] * v[i];
    }
    temp = 0.5*global_m/(kB*(2*global_N-2))*sum;
    return temp;
}

void simulation::settemp(float T) {
    // ändert Teilchen Geschwindigkeiten so, dass E=kB*T
    double T_akt=gettemp();
    for(int i=0 ; i<global_N ; i++) {
        v[i] /= sqrt(T_akt/T);
    }

}

vek simulation::get_conn_vector(vek r1, vek r2) {
	vek r_temp = r2 - r1;
	// loop around boundary
	if(abs(r_temp.GetX())>global_L/2) { // get the distance around the boundary and switch direction
		if(r_temp.GetX()>0){
			r_temp.SetX(r_temp.GetX() - global_L);
		}
		else {
			r_temp.SetX(global_L + r_temp.GetX());
		}
	}
	if(abs(r_temp.GetY())>global_L/2) { // get the distance around the boundary and switch direction
		if(r_temp.GetY()>0){
			r_temp.SetY(r_temp.GetY() - global_L);
		}
		else {
			r_temp.SetY(global_L + r_temp.GetY());
		}
	}
	return r_temp;
}

vek simulation::get_force(vek x) {
    double mag = 0;
    // Lennard-Jones potential
    double epsilon = kB;
    double sigma = 1;
    double r_len = x.get_len();
    x = x.get_dir();
    mag = 48*(epsilon)*( -0.5*pow(sigma/r_len, 7) + pow(sigma/r_len, 13) );
    x *= mag;
    return x;
}

double simulation::get_potential(vek x) {
    // Lennard-Jones potential
    double epsilon = kB;
    double sigma = 1;
    double r_len = x.get_len();
    double retval = 4*(epsilon)*( pow(sigma/r_len, 12)        - pow(sigma/r_len, 6));
    // retval 		 -= 4*(epsilon)*( pow(sigma/(global_L/2), 12) - pow(sigma/(global_L/2), 6)); // Cutoff mitberechnen
    return retval;
}

void simulation::calculate_forces() {
    for(int i=0; i<global_N; i++) {
        F[i] = 0;
    }

    for(int i=0; i<global_N; i++) {
        for(int k=i+1; k<global_N; k++) {
			vek F_temp = get_force(get_conn_vector(r[i],r[k]));
			F[k] = F[k] + F_temp;
			F[i] = F[i] - F_temp;
		}
    }
}

void simulation::init() {
    // Init()
    // plaziert N Teilchen gleichmäßig im quadratischen Kasten mit Kantenlänge L,
    // gibt ihnen eine zufällige Geschwindigkeit zwischen -0.5 und +0.5 L pro Zeiteinheit
    // und zieht die Schwerpunktsgeschwindigkeit des Systems ab
    double sqr=sqrt(global_N);
    double step= global_L/sqr; // Feld muss Platz für verschiebung bieten--> Teilchen von 0 und L liegen sonst aufeinander
    for(int i=0; i*step<global_L; i++) {
        vek tempr;
        tempr.SetX(i*step);
        for(int k=0; k*step<global_L; k++) {
            tempr.SetY(k*step);
            r.push_back(tempr);
            vek tempv;
            tempv.SetX(1.0*rand()/RAND_MAX*global_L-global_L*0.5); // Zufallsgeschwindigkeit zwischen -0.5L und +0.5L
            tempv.SetY(1.0*rand()/RAND_MAX*global_L-global_L*0.5); // Zufallsgeschwindigkeit zwischen -0.5L und +0.5L
            v.push_back(tempv);
            vek tempF;
            F.push_back(tempF);
        }

    }

    vek sum;
    for(int i=0; i<global_N; i++) { // ermittele Durchschnittsgeschwindigkeit
        sum += v[i]/global_N;

    }
    for(int i=0; i<global_N; i++) { // ziehe Durchschnittsgeschwindigkeit ab
        v[i] -= sum;
    }
}

void simulation::calculate_movement() {
    for(int i=0; i<global_N; i++) {
        // verlet: r+1 = 2r - r_(-1) + dt^2*a
        vek r_new = r[i]*2 - r_old[i] + F[i]*pow(t_step, 2);

        // calculate the new speed
        v[i] = get_conn_vector(r[i], r_new)/t_step;

		// loop around boundary
        r_new.SetX(r_new.GetX() - floor(r_new.GetX()/global_L)*global_L);
        r_new.SetY(r_new.GetY() - floor(r_new.GetY()/global_L)*global_L);

		// update old position
		r_old[i] = r[i];
        // set new position
        r[i] = r_new;
    }
}

void simulation::md_step() {
    calculate_forces();
    calculate_movement();
}

void simulation::equilibrate() {
	// equilibrate
    double t = 0;
    while(t < t_equi) {
        md_step();
        t += t_step;
    }
}

bool simulation::main_step() {
    // central MD-loop
    static double t = 0;

    if (t>t_max) {return false;}
	md_step();
	t += t_step;
	return true;
}

measurement simulation::measure() {
    measurement mes;
    mes.E_kin = get_E_kin();
    mes.E_pot = get_E_pot();
    return mes;
}

vector<vek> simulation::get_r() {
	return r;
}

int simulation::get_global_N(){
	return global_N;
}

double simulation::get_global_L(){
	return global_L;
}

void simulation::init_r_old() {
	// initialises the old positions from the initial position and the initial vetocity
	for(int i=0; i<global_N; i++) {
		vek r_oldt;
        r_oldt = r[i] - v[i]*t_step;
        r_old.push_back(r_oldt);
    }
}

double simulation::get_E_kin() {
	double ret = 0;
	for (int i=0;i<global_N;i++){
		ret += 0.5*global_m*(v[i]*v[i]);
	}
	return ret;
}

double simulation::get_E_pot() {
	double ret = 0;
	for(int i=0; i<global_N; i++) {
        for(int k=i+1; k<global_N; k++) {
			ret += get_potential(get_conn_vector(r[i],r[k]));
        }
    }
    return ret;
}

simulation::~simulation() {
	//dtor
}
