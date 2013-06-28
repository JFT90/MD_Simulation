#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;


//#########################################################################
int global_L=8;            //Kastenlänge
int global_N=16;           //Anzahl der Teilchen
int global_T=300;          //Temperatur
double m=1.674927351e-27;            //Teilchenmasse in kg (Proton)
const double kB=1.3806488e-23;         //Boltzmannkonstante
struct base         //Structschablone für Ort oder Geschwindigkeit
{
    double x,y;
};

vector<base> r;
vector<base> v;

//Init()
//plaziert N Teilchen gleichmäßig im quadratischen Kasten mit Kantenlänge L,
//gibt ihnen eine zufällige Geschwindigkeit zwischen -0.5 und +0.5 L pro Zeiteinheit
//und zieht die Schwerpunktsgeschwindigkeit des Systems ab
void init()
{
    double sqr=sqrt(global_N);
    double step= global_L/sqr; // Feld muss Platz für verschiebung bieten--> Teilchen von 0 und L liegen sonst aufeinander
    for(int i=0; i*step<global_L; i++)
    {
        base tempr;
        tempr.x=i*step;
        for(int k=0; k*step<global_L; k++)
        {
            tempr.y=k*step;
            r.push_back(tempr);
            base tempv;
            tempv.x= 1.0*rand()/RAND_MAX*global_L-global_L*0.5; //Zufallsgeschwindigkeit zwischen -0.5L und +0.5L
            tempv.y= 1.0*rand()/RAND_MAX*global_L-global_L*0.5; //Zufallsgeschwindigkeit zwischen -0.5L und +0.5L
            v.push_back(tempv);

        }

    }

    base sum= {0,0};
    for(int i=0; i<global_N; i++) //ermittele Durchschnittsgeschwindigkeit
    {
        sum.x+=v[i].x/global_N;
        sum.y+=v[i].y/global_N;

    }
    for(int i=0; i<global_N; i++) //ziehe Durchschnittsgeschwindigkeit ab
    {
        v[i].x-=sum.x;
        v[i].y-=sum.y;

    }


    sum.x=0;
    sum.y=0;
    for(int i=0; i<global_N; i++) //Testausgabe
    {
        sum.x+=v[i].x/global_N;
        sum.y+=v[i].y/global_N;

    }
    printf("Vsx=\t%f\nVsy=\t%f\n",sum.x,sum.y);

}

//gibt aktuelle Temperatur des Systems zurück in Kelvin
double gettemp() {
    base sum= {0,0};
    for(int i=0 ; i<global_N ; i++)
    {
        sum.x += abs(v[i].x);
        sum.y += abs(v[i].y);
    }
    return 0.5*m/kB*((sum.x*sum.x)+(sum.y*sum.y))/global_N;
}

//ändert Teilchen Geschwindigkeiten so, dass E=kB*T
void settemp(float T){
    double T_akt=gettemp();
    for(int i=0 ; i<global_N ; i++)
    {
        v[i].x/=sqrt(T_akt/T);
        v[i].y/=sqrt(T_akt/T);
    }

}

int main()
{
    srand(time(NULL));
    init();
    cout <<gettemp()<<endl;
    settemp(global_T);
    cout << gettemp();
    return 0;
}
