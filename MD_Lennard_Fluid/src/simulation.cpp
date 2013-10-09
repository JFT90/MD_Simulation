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

vector<base> r;
vector<base> r_old;
vector<base> v;
vector<base> F;

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
    base sum= {0,0};
    for(int i=0 ; i<global_N ; i++) {
        sum.x += pow(v[i].x, 2);
        sum.y += pow(v[i].y, 2);
    }
    temp = 0.5*global_m/kB*((sum.x)+(sum.y))/(2*global_N-2);
    return temp;
}

void simulation::settemp(float T) {
    // ändert Teilchen Geschwindigkeiten so, dass E=kB*T
    double T_akt=gettemp();
    for(int i=0 ; i<global_N ; i++) {
        v[i].x/=sqrt(T_akt/T);
        v[i].y/=sqrt(T_akt/T);
    }

}

base simulation::get_conn_vector(base r1, base r2) {
	base r_temp = {r2.x - r1.x, r2.y-r1.y};
	// loop around boundary
	if(abs(r_temp.x)>global_L/2) { // get the distance around the boundary and switch direction
		if(r_temp.x>0){
			r_temp.x = r_temp.x - global_L;
		}
		else {
			r_temp.x = global_L + r_temp.x;
		}
	}
	if(abs(r_temp.y)>global_L/2) { // get the distance around the boundary and switch direction
		if(r_temp.y>0){
			r_temp.y = r_temp.y - global_L;
		}
		else {
			r_temp.y = global_L + r_temp.y;
		}
	}
	return r_temp;
}

base simulation::get_force(base x) {
    double mag = 0;
    // Lennard-Jones potential
    double epsilon = kB;
    double sigma = 1;
    double r_len = sqrt(x.x*x.x + x.y*x.y);
    x.x = x.x / r_len;
    x.y = x.y / r_len;
    mag = 48*(epsilon)*( -0.5*pow(sigma/r_len, 7) + pow(sigma/r_len, 13) );
    x.x *= mag;
    x.y *= mag;
    return x;
}

double simulation::get_potential(base x) {
    // Lennard-Jones potential
    double epsilon = kB;
    double sigma = 1;
    double r_len = sqrt(x.x*x.x + x.y*x.y);
    double retval = 4*(epsilon)*( pow(sigma/r_len, 12)        - pow(sigma/r_len, 6));
    // retval 		 -= 4*(epsilon)*( pow(sigma/(global_L/2), 12) - pow(sigma/(global_L/2), 6)); // Cutoff mitberechnen
    return retval;
}

void simulation::calculate_forces() {
    for(int i=0; i<global_N; i++) {
        F[i].x=0;
        F[i].y=0;
    }

    for(int i=0; i<global_N; i++) {
        for(int k=i+1; k<global_N; k++) {
			base F_temp = get_force(get_conn_vector(r[i],r[k]));
			F[k].x = F[k].x + F_temp.x;
			F[k].y = F[k].y + F_temp.y;
			F[i].x = F[i].x - F_temp.x;
			F[i].y = F[i].y - F_temp.y;
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
        base tempr;
        tempr.x=i*step;
        for(int k=0; k*step<global_L; k++) {
            tempr.y=k*step;
            r.push_back(tempr);
            base tempv;
            tempv.x= 1.0*rand()/RAND_MAX*global_L-global_L*0.5; // Zufallsgeschwindigkeit zwischen -0.5L und +0.5L
            tempv.y= 1.0*rand()/RAND_MAX*global_L-global_L*0.5; // Zufallsgeschwindigkeit zwischen -0.5L und +0.5L
            v.push_back(tempv);
            base tempF = {0,0};
            F.push_back(tempF);
        }

    }

    base sum= {0,0};
    for(int i=0; i<global_N; i++) { // ermittele Durchschnittsgeschwindigkeit
        sum.x+=v[i].x/global_N;
        sum.y+=v[i].y/global_N;

    }
    for(int i=0; i<global_N; i++) { // ziehe Durchschnittsgeschwindigkeit ab
        v[i].x-=sum.x;
        v[i].y-=sum.y;

    }

    sum.x=0;
    sum.y=0;
    for(int i=0; i<global_N; i++) { // Testausgabe
        sum.x+=v[i].x/global_N;
        sum.y+=v[i].y/global_N;

    }
}

void simulation::calculate_movement() {
    for(int i=0; i<global_N; i++) {
        base r_new = {0,0};
        // verlet: r+1 = 2r - r_(-1) + dt^2*a

        double temp = pow(t_step, 2);
        temp *= F[i].x;
        temp /= global_m;
        r_new.x = 2*r[i].x;
        r_new.x -= r_old[i].x;
        r_new.x += temp;
        temp = pow(t_step, 2);
        temp *= F[i].y;
        temp /= global_m;
        r_new.y = 2*r[i].y;
        r_new.y -= r_old[i].y;
        r_new.y += temp;

        // calculate the new speed
        v[i] = get_conn_vector(r[i], r_new);
        v[i].x /= t_step;
        v[i].y /= t_step;

		// loop around boundary
        r_new.x = r_new.x - floor(r_new.x/global_L)*global_L;
        r_new.y = r_new.y - floor(r_new.y/global_L)*global_L;

		// update old position
		r_old[i].x = r[i].x;
        r_old[i].y = r[i].y;
        // set new position
        r[i].x = r_new.x;
        r[i].y = r_new.y;
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

vector<base> simulation::get_r() {
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
		base r_oldt;
        r_oldt.x = r[i].x - t_step*v[i].x;
        r_oldt.y = r[i].y - t_step*v[i].y;
        r_old.push_back(r_oldt);
    }
}

double simulation::get_E_kin() {
	double ret = 0;
	for (int i=0;i<global_N;i++){
		ret += 0.5*global_m*(v[i].x*v[i].x+v[i].y*v[i].y);
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
