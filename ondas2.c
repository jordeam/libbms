#include <math.h>

#include "constant.h"
#include "func.h"
#include "ondas2.h"

void dflux1_init(DFLUX1 *self, double B1, double theta0, double theta1) {
  self->B1 = B1;
  self->theta0 = theta0;
  self->theta1 = theta1;
}

double dflux1_eval(DFLUX1 *self, double theta) {
  double fact, x;

  x=frac(theta/(2*M_PI));
  if (x<0)
    x+= 1;
  x*=2;

  if (x<M_1_2)
    fact=1;
  else if (x<1) {
    fact=1;
    x=1-x;
    }
  else if (x<M_3_2) {
    fact=-1;
    x=x-1;
    }
  else {
    fact=-1;
    x=2-x;
    }
  if (x*M_PI < self->theta0)
    return 0;
  else if (x*M_PI < self->theta1)
    return fact*self->B1/(self->theta1-self->theta0)*(x*M_PI-self->theta0);
  else
    return fact*((1-self->B1)/(M_PI_2-self->theta1)*(x*M_PI-self->theta1)+self->B1);
}

void dflux2_init(DFLUX2 *self, double theta0, double theta1) {
  self->theta0 = theta0;
  self->theta1 = theta1;
}

double dflux2_eval(DFLUX2 *self, double theta) {
  double fact, x;

  x = frac(theta/(2*M_PI));
  if (x < 0)
    x += 1;
  x *= 2;

  if (x < M_1_2)
    fact = 1;
  else if (x < 1) {
    fact = 1;
    x = 1-x;
    }
  else if (x < M_3_2) {
    fact = -1;
    x = x-1;
    }
  else {
    fact = -1;
    x = 2-x;
    }
  if (x*M_PI < self->theta0)
    return 0;
  else if (x*M_PI < self->theta1)
    return fact*(x*M_PI-self->theta0)/(self->theta1-self->theta0);
  else
    return fact;
  }
