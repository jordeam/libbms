#ifndef _TRANSFOR_H
#define _TRANSFOR_H

#include "rvector.h"

void abc_alpha(double, double, double, double*, double*, double*);
void alpha_abc(double, double, double, double*, double*, double*);
void Park(double d, double q, double *alpha, double *beta, double theta);
void axdq(double *xalpha, double *xbeta, double ax, double theta, double xd, double xq);
int eval_axdq(rvector_t * emf, rvector_t * thetax, rvector_t * dthetax, rvector_t * ax, rvector_t * dax, rvector_t * Frq);
int eval_axdq_from_fcn(double (*fcn)(double), int n, rvector_t * thetax, rvector_t * dthetax, rvector_t * ax, rvector_t * dax, rvector_t * Frq);

#endif
