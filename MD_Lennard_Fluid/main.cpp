#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;


//#########################################################################
// Simulation parameters
int global_L=8;            // Kastenlänge
int global_N=16;           // Anzahl der Teilchen
int global_T=300;          // Temperatur
double global_m=1.674927351e-27;            // Teilchenmasse in kg (Proton)
double t_step = 0.0001;
double t_max = 10000*t_step;
double t_equi = 100*t_step;
double t_measure = 1000*t_step;
//#########################################################################


const double kB=1.3806488e-23;         // Boltzmannkonstante
struct base {       // Structschablone für Ort oder Geschwindigkeit
    double x,y;
};

struct measurement {
    double E;
};

vector<base> r;
vector<base> v;
vector<base> F;

void init() {
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
    printf("Vsx=\t%f\nVsy=\t%f\n",sum.x,sum.y);

}

double gettemp() {
    // gibt aktuelle Temperatur des Systems in Kelvin zurück
    base sum= {0,0};
    for(int i=0 ; i<global_N ; i++) {
        sum.x += abs(v[i].x);
        sum.y += abs(v[i].y);
    }
    return 0.5*global_m/kB*((sum.x*sum.x)+(sum.y*sum.y))/global_N;
}

void settemp(float T) {
    // ändert Teilchen Geschwindigkeiten so, dass E=kB*T
    double T_akt=gettemp();
    for(int i=0 ; i<global_N ; i++) {
        v[i].x/=sqrt(T_akt/T);
        v[i].y/=sqrt(T_akt/T);
    }

}

base get_force(base x) {
    double mag = 0;
    // Lennard-Jones potential
    double epsilon = 1.0;
    double sigma = 1.0;
    double r_len = sqrt(x.x*x.x + x.y*x.y);
    mag = 48*epsilon/(r_len*r_len)*( 0.5*pow(sigma/r_len, 6) - pow(sigma/r_len, 12));
    base retval = x;
    retval.x *= mag;
    retval.y *= mag;
    return retval;
}

void calculate_forces() {
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
            r_temp.x = r_temp.x - floor(1.0*r_temp.x/global_L);
            r_temp.y = r_temp.y - floor(1.0*r_temp.y/global_L);

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

void md_step() {
	calculate_forces();
	// TODO: calculate_movement();
}

measurement measure() {
    measurement mes = {0};
    return mes;
}

void save_measurement(measurement mes) {}

int main() {
    srand(time(NULL));
    init();
    cout <<gettemp()<<endl;
    settemp(global_T);
    cout << gettemp()<<endl;

    // equilibrate
    double t = 0;
    while(t < t_equi) {
        md_step();
        t += t_step;
    }

    // central MD-loop
    t = 0;
    while(t < t_max) {
        double t_temp = 0;
        while(t_temp < t_measure) {
            md_step();
            t_temp += t_step;
            t += t_step;
        }
        measurement mes = measure();
        save_measurement(mes);
    }

    return 0;
}
