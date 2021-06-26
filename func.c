#include <math.h>
#include <stdlib.h>

#include "func.h"

int inCicle(int i, int SIZE) {
  int n;
  n = i % SIZE;
  return (n < 0) ? n + SIZE : n;
}

double in_range(double x, double period) {
  x /= period;
  x=frac(x);
  if (x>0.5)
    x-=1.0;
  return period * x;
}

double RMS(double *D, int N)
{
  double dt, sum;
  int i;
  dt=1/(double) N;
  sum=0;
  for (i=0; i<N; i++)
    sum+= dt*(*(D+i))*(*(D+i));
  return sqrt(sum);
}

double media(double *D, int N)
{
  double dt, sum;
  int i;
  dt=1/(double) N;
  sum=0;
  for(i=0; i<N; i++)
    sum+=dt*(*(D+i));
  return sum;
}

double medabs(double *D, int N)
{
  double dt, sum;
  int i;
  dt=1/(double) N;
  sum=0;
  for(i=0; i<N; i++)
    sum+=dt*fabs(*(D+i));
  return sum;
}

