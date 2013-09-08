#include "../include/simulation.h"


//#########################################################################
// Simulation parameters
const int global_L=8; // Kastenlänge
const int global_N=16; // Anzahl der Teilchen
const int global_T=300; // Temperatur
const double global_m=6.6e-26; // Teilchenmasse in kg
const double t_step = 1e-7;
const double t_max = 1e7*t_step;
const double t_equi = 100*t_step;
const double t_measure = 1000*t_step;
//#########################################################################

const double kB=1.3806488e-23; // Boltzmannkonstante

vector<base> r;
vector<base> r_old;
vector<base> v;
vector<base> F;

simulation::simulation()
{
	//ctor

	// srand(time(NULL));
    // initialize random generator
    srand(4); //debug
    init();
    settemp(global_T);

    equilibrate();
}

double simulation::gettemp() {
    // gibt aktuelle Temperatur des Systems in Kelvin zurück
    base sum= {0,0};
    for(int i=0 ; i<global_N ; i++) {
        sum.x += pow(v[i].x, 2);
        sum.y += pow(v[i].y, 2);
    }
    return 0.5*global_m/kB*((sum.x)+(sum.y))/(2*global_N-2);
}

void simulation::settemp(float T) {
    // ändert Teilchen Geschwindigkeiten so, dass E=kB*T
    double T_akt=gettemp();
    for(int i=0 ; i<global_N ; i++) {
        v[i].x/=sqrt(T_akt/T);
        v[i].y/=sqrt(T_akt/T);
    }

}

base simulation::get_force(base x) {
    double mag = 0;
    // Lennard-Jones potential
    double epsilon = kB;
    double sigma = 3.4e-10;
    double r_len = sqrt(x.x*x.x + x.y*x.y);
    mag = 48*epsilon/(r_len*r_len)*( 0.5*pow(sigma/r_len, 6) - pow(sigma/r_len, 12));
    base retval = x;
    retval.x *= mag;
    retval.y *= mag;
    return retval;
}

void simulation::calculate_forces() {
    for(int i=0; i<global_N; i++) {
        F[i].x=0;
        F[i].y=0;
    }

    for(int i=0; i<global_N; i++) {
        for(int k=i+1; k<global_N; k++) {
            base r_1 = r[i];
            base r_2 = r[k];

            base r_temp = {r_1.x - r_2.x, r_1.y-r_2.y};
            // loop around boundary
            r_temp.x = r_temp.x - floor(r_temp.x/global_L)*global_L;;
            r_temp.y = r_temp.y - floor(r_temp.y/global_L)*global_L;;

            if (r_temp.x < global_L/2.0 and r_temp.y < global_L/2.0) {
                base F_temp = get_force(r_temp);
                F[i].x = F[i].x + F_temp.x;
                F[i].y = F[i].y + F_temp.y;
                F[k].x = F[k].x - F_temp.x;
                F[k].y = F[k].y - F_temp.y;
            }
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
        // verlet: r = 2r - r_(-1) + dt²*F
        base r_oldt = {0,0};
        r_oldt.x = r[i].x - t_step*v[i].x;
        r_oldt.y = r[i].y - t_step*v[i].y;
        double temp = pow(t_step, 2)*F[i].x/global_m;
        r_new.x = 2*r[i].x;
        r_new.x -= r_oldt.x;
        r_new.x += temp;
        temp = pow(t_step, 2)*F[i].y/global_m;
        r_new.y = 2*r[i].y;
        r_new.y -= r_oldt.y;
        r_new.y += temp;

        // calculate the new speed with actual positions
        v[i].x = (r_new.x - r[i].x)/(t_step);
        v[i].y = (r_new.y - r[i].y)/(t_step);

        r_new.x = r_new.x - floor(r_new.x/global_L)*global_L;
        r_new.y = r_new.y - floor(r_new.y/global_L)*global_L;


        //cout<<r_new.x<<endl;
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
    measurement mes = {0};
    return mes;
}

vector<base> simulation::get_r() {
	return r;
}

int simulation::get_global_N(){
	return global_N;
}

int simulation::get_global_L(){
	return global_L;
}

simulation::~simulation()
{
	//dtor
}
