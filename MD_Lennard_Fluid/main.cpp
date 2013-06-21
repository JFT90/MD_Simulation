#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


//#########################################################################
int L=8;
int N=16;
int T=300;
struct base
{
    double x,y;
};

vector<base> r;
vector<base> v;


void init()
{
    double sqr=sqrtf(N);
    double step= L/sqr; // Feld muss Platz für verschiebung bieten--> Teilchen von 0 und L liegen sonst aufeinander
    for(int i=0; i*step<L; i++)
    {
        base temp;
        temp.x=i*step;
        for(int k=0; k*step<L; k++)
        {
            temp.y=k*step;
            r.push_back(temp);
            base tempv;
            tempv.x= 1.0*rand()/RAND_MAX*L-L*0.5;
            tempv.y= 1.0*rand()/RAND_MAX*L-L*0.5;
            v.push_back(tempv);
        }

    }

    base sum={0,0};
    for(int i=0;i<N;i++)
    {
        sum.x+=v[i].x/N;
        sum.y+=v[i].y/N;

    }
    for(int i=0;i<N;i++)
    {
        v[i].x-=sum.x;
        v[i].y-=sum.y;

    }
    sum.x=0;
    sum.y=0;
    for(int i=0;i<N;i++)
    {
        sum.x+=v[i].x/N;
        sum.y+=v[i].y/N;

    }
    printf("Vsx=\t%f\nVsy=\t%f\n",sum.x,sum.y);

}

int main()
{

    init();
    cout << "Hello world!"<<sizeof(long double)<< endl;
    return 0;
}
