/* 
   This program evaluates the zero of a function using the ``secant''
   numeric method.

   nsol_sec -> the function to find the zero
   err -> maximun error of function

   Algoritms errors:
   max_iter -> maximun number of iterations permited
   err_code -> returned error code of algoritm, vlues:
            - ITER_OVERFLOW -> number of iterations overflowed
*/

#include <math.h>
#include "nlsol.h"
#include "func.h"

static double err = 1e-4;
static int errcode, maxiter = 20;

static int maxbisect=100;          // maximun number of bisections
static BOOL valid;                 // used in bisections
static double value;               // value found

double nlsol_sec(double(*f)(double), double x, double xo) {
  double fx, fxo;
  double a, b;

  int n=0;

  nlsol_clear_errcode();

  fxo = f(xo);

  while (n++ < maxiter) {
    fx = f(x);
    if (fabs(fx) < err)
      return x;
    a=(fx-fxo)/(x-xo);
    b=fxo-a*xo;
    
    xo=x;
    fxo=fx;

    x=-b/a;
  }

  errcode = NLSOL_ITEROVERFLOW;
  return x;
}

void nlsol_set_maxiter(int iter) {
  maxiter=iter;
}

void nlsol_set_err(double err1) {
  err=err1;
}

int nlsol_get_errcode() {
  return errcode;
}

void nlsol_clear_errcode() {
  errcode=0;
}

void nlsol_set_maxbisect(int max) {
  maxbisect = max;
}

double nlsol_bisect(double (*f)(double), double x0, double x1) {
  double fx0, fx1;

  nlsol_clear_errcode();

  fx0 = f(x0);
  fx1 = f(x1);

  if (fabs(fx0) < err) return x0;
  if (fabs(fx1) < err) return x1;

  valid=0;
  bip2(f, x0, x1, 0);

  if (valid)
    return value;
  else {
    errcode = NLSOL_MAXBISECTREACH;
    return value;
  }
}


void bip2(double (*f)(double), double x0, double x1, int n) {
  double x2, fx0, fx1, fx2;

  fx0 = f(x0);
  fx1 = f(x1);

  if (n++ > maxbisect || valid)
    return;

  if (samesign(fx0,fx1)) {
    x2 = (x0 + x1)/2;
    bip2(f, x0, x2, n);
    bip2(f, x2, x1, n);
  }

  while (n++ < maxbisect) {
    x2=(x0+x1)/2;
    fx2=f(x2);
    if(fabs(fx2)< err) {
      valid = 1;
      value = x2;
    }
    if (samesign(fx0, fx2)) {
      x0=x2;
      fx0=fx2;
    }
    else {
      x1=x2;
      fx1=fx2;
    }
  }
}
