#include <math.h>

#include "constant.h"
#include "sysconst.h"
#include "func.h"

#include "transfor.h"
#include "rvector.h"
#include "wav.h"

void abc_alpha(double a, double b, double c, double *alpha, double *beta, double *zero)
{
  *alpha=M_sqrt_2_3*(a-0.5*b-0.5*c);
  *beta=M_sqrt_2_3*(M_sqrt3_2*b-M_sqrt3_2*c);
  *zero=M_sqrt_1_3*(a+b+c);
}

void alpha_abc(double alpha, double beta, double zero, double *a, double *b, double *c)
{
  *a=M_sqrt_2_3*alpha+M_sqrt_1_3*zero;
  *b=-M_sqrt_2_3*0.5*alpha+M_sqrt_1_2*beta+M_sqrt_1_3*zero;
  *c=-M_sqrt_2_3*0.5*alpha-M_sqrt_1_2*beta+M_sqrt_1_3*zero;
}

void Park(double d, double q, double *alpha, double *beta, double theta)
{
  *alpha=d*cos(theta)-q*sin(theta);
  *beta=d*sin(theta)+q*cos(theta);
}

void axdq(double *xalpha, double *xbeta, double ax, double theta, double xd, double xq)
{
  *xalpha=ax*(xd*cos(theta)-xq*sin(theta));
  *xbeta=ax*(xd*sin(theta)+xq*cos(theta));
}

/*
  Eval  thetax, dthetax/dtheta,  ax,  dax/dtheta and  Frq  fom an  emf
  rvector and returns to an emf vector.
*/
int eval_axdq(rvector_t * emf, rvector_t * thetax, rvector_t * dthetax, rvector_t * ax, rvector_t * dax, rvector_t * Frq) {
  double k1;
  double Fra, Frb, Frc;
  double Fralpha, Frbeta, Fr0;
  double theta, dtheta;
  int emf_n, i;

  emf_n = rvector_n(emf);
  
  k1=M_sqrt_3_2; // Para calculo de ax

  dtheta=M_2PI/emf_n;
  for (i=0; i<emf_n; i++) {
    theta=i*dtheta;

    Fra = -rvector_get(emf, i);
    Frb = -rvector_get(emf, i - M_1_3 * emf_n);
    Frc = -rvector_get(emf, i + M_1_3 * emf_n);
    
    abc_alpha(Fra, Frb, Frc, &Fralpha, &Frbeta, &Fr0);

    rvector_put(thetax, i, in_range(atan2(-Fralpha, Frbeta) - theta, M_2PI));
    rvector_put(ax, i, k1 / sqrt(sqr(Fralpha) + sqr(Frbeta)));
    rvector_put(Frq, i, sqrt(sqr(Fralpha) + sqr(Frbeta)));
  }
  for (i = 0; i < emf_n; i++) {
    rvector_put(dthetax, i, rvector_deriv(thetax, i) / M_2PI);
/*     printf("eval_axdq: thetax[%d] = %g, thetax[%d] = %g, dthetax[%d] = %g\n" */
/* 	   , i, rvector_get(thetax, i) */
/* 	   , i, rvector_get(thetax, i + 1) */
/* 	   , i, rvector_get(dthetax, i) */
/* 	   ); */
    rvector_put(dax, i, rvector_deriv(ax, i) / M_2PI);
  }
  rvector_cutpeak(dthetax, 1000);
  rvector_cutpeak(dax, 1000);

  return 1;
}

/*
  Eval thetax, dthetax/dthetar, ax, dax/dthetar and Frq fom a function (trap120, trap60, etc...)
*/
int
eval_axdq_from_fcn(double (*fcn)(double), int n, rvector_t * thetax, rvector_t * dthetax, rvector_t * ax, rvector_t * dax, rvector_t * Frq)
{
  double Fra, Frb, Frc;
  double Fralpha, Frbeta, Fr0;
  double theta, dtheta;
  int i;  

  /* resize vectors */
  rvector_resize(thetax, n);
  rvector_resize(dthetax, n);
  rvector_resize(ax, n);
  rvector_resize(dax, n);
  rvector_resize(Frq, n);

  dtheta  =M_2PI/n;
  for (i = 0; i < n; i++) {
    theta=i*dtheta;

    Fra = -fcn(theta);
    Frb = -fcn(theta - A120);
    Frc = -fcn(theta + A120);
    
    abc_alpha(Fra, Frb, Frc, &Fralpha, &Frbeta, &Fr0);

    rvector_put(thetax, i, in_range(atan2(-Fralpha, Frbeta) - theta, M_2PI));
    rvector_put(ax, i, M_sqrt_3_2 / sqrt(sqr(Fralpha) + sqr(Frbeta)));
    rvector_put(Frq, i, sqrt(sqr(Fralpha) + sqr(Frbeta)));
  }
  for (i = 0; i < n; i++) {
    rvector_put(dthetax, i, rvector_deriv(thetax, i) / M_2PI);
    rvector_put(dax, i, rvector_deriv(ax, i) / M_2PI);
  }
  rvector_cutpeak(dthetax, 500);
  rvector_cutpeak(dax, 500);

  return 1;
}
