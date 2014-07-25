#include <stdio.h>
#include <math.h>

#define NUMPOINTS 11

typedef struct {
      double xi[NUMPOINTS], yi[NUMPOINTS];
      int N;
    } pln;

double Pol(double);

/* forma de onda da maquina do paper GRENIER */
double trapezoidal1(double);

double pln_FN(pln *p, double x, int i);

double pln_FD(pln *p, int i);

double pln_P(pln *p, double x);

double Pol(double x);

double trapezoidal1(double theta);
