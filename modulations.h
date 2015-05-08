#ifndef _modulations_h
#define _modulations_h

/**
   This structure defines a clue to function name as a C pointer, a text name and a brief description.
*/
struct modulation {
  /** Pointer to function. */
  double (*fcn)(double, double);
  /** The function name */
  char * name;
  /** A brief description of modulation */
  char * desc;
};

extern struct modulation modulations[];

double msin(double M, double theta);
double mcos(double M, double theta);
double mthird(double M, double theta);
double triang_sin(double x, void* parms);
double triang_cos(double x, void* parms);
double sawtooth(double x, void* parms);
double mk_modul(double M, double (* mdltd)(double, double), double (* carrier)(double, void*),
                double n, double theta, double phi, double phi_carrier);
double mmin(double M, double theta);
double triang_45(double x, void* parms);
double triang_60(double x, void* parms);
double msvpwm(double M, double theta);
double mdis1(double M, double theta);
double mdis2(double M, double theta);
double mdis3(double IM, double theta);
double msquare(double M, double theta);
double msquare120(double M, double theta);
double mhcos(double M, double theta);
double mhsin(double M, double theta);
double msqr180(double M, double theta);
double msimple(double M, double theta);

#endif
