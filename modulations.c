#include <stdlib.h>
#include <math.h>

#include "constant.h"
#include "func.h"

#include "modulations.h"

/**
   A list of modulations and its descriptions.
*/
struct modulation modulations[] = {
  { msin, "msin", "Sinusoidal" },
  { mcos, "mcos", "Sinusoidal" },
  { mthird, "mthird", "Third Harmonic Injection" },
  { mmin, "mmin", "Minimal Vaule Equal Zero" },
  { msvpwm, "msvpwm", "Space-Vector PWM" },
  { mdis1, "mdis1", "Discontinuous Mode 1" },
  { mdis2, "mdis2", "Discontinuous Mode 2" },
  { mdis3, "mdis3", "Discontinuous Mode 3" },
  { mhcos, "mhcos", "Half cosine" },
  { mhsin, "mhsin", "Half sine" },
  { msqr180, "msqr180", "Squarewave 180 degrees"},
  { msimple, "msimple", "Simple pulse width modulation"},
  { (void*) triang_cos, "triang_cos", "Triangular in cossine phase" },
  { (void*) triang_sin, "triang_sin", "Triangular in sine phase" },
  { NULL, NULL, NULL }
};

/**
   This function returns the PWM output for the modulated function (values 0 or 1).
   \param M modulation index
   \param mdltd function to be modulated by carrier function
   \param carrier the carrier function
   \param n the relative frequency of carrier function
   \param phi the initial angle
 */
double mk_modul(double M, double (* mdltd)(double, double), double (* carrier)(double, void*),
                double n, double theta, double phi, double phi_carrier) {
  return (mdltd(M, theta + phi) > carrier(n * (theta + phi - phi_carrier), NULL)) ? 1 : 0;
}


/*
  input 0 to 2PI
  output 0 to 1
*/
double triang_sin(double x, void *parms) {
  moduni(x);
  double y = 2 * x;
  if (y > 1.5)
    return y - 1.5;
  else if ( y > 0.5)
    return 1.5 - y;
  else
    return y + 0.5;
}

/*
  input 0 to 2PI
  output 0 to 1
*/
double triang_cos(double x, void *parms) {
  moduni(x);
  double y = 2 * x;
  if (y > 1)
    return y - 1;
  else
    return 1 - y;
}

double triang_45(double x, void *parms) {
  x -= M_A45;
  moduni(x);
  double y = 2 * x;
  if (y > 1)
    return y - 1;
  else
    return 1 - y;
}

double triang_60(double x, void *parms) {
  x -= M_A60;
  moduni(x);
  double y = 2 * x;
  if (y > 1)
    return y - 1;
  else
    return 1 - y;
}

double sawtooth(double x, void *parms) {
  return frac(x/M_2PI);
}

double msvpwm(double M, double theta) {
  double r;
  theta -= M_A90;
  while (theta < 0)
    theta += M_2PI;
  while (theta > M_2PI)
    theta -= M_2PI;
  if (theta <= A60)
    r = M * cos(theta - A30);
  else if (theta <= A120)
    r = sqrt(3) * M * cos(theta);
  else if (theta <= A180)
    r = M * cos(theta + A30);
  else if (theta <= A240)
    r = M * cos(theta - A30);
  else if (theta <= A300)
    r = sqrt(3) * M * cos(theta);
  else
    r = M * cos(theta + A30);
  return 0.5 + 0.5 * r;
}

double mdis1(double M, double theta) {
  double r;
  theta -= M_A90;
  while (theta < 0)
    theta += M_2PI;
  while (theta > M_2PI)
    theta -= M_2PI;
  if (theta <= A30)
    r = 1;
  else if (theta <= A90)
    r = 2 * M * cos(theta - A30) - 1;
  else if (theta <= A150)
    r = 2 * M * cos(theta + A30) + 1;
  else if (theta <= A210)
    r = -1;
  else if (theta <= A270)
    r = 2 * M * cos(theta - A30) + 1;
  else if (theta <= A330)
    r = 2 * M * cos(theta + A30) - 1;
  else
    r = 1;
  return 0.5 + 0.5 * r;
}

double mdis2(double M, double theta) {
  double r;
  theta -= M_A90;
  while (theta < 0)
    theta += M_2PI;
  while (theta > M_2PI)
    theta -= M_2PI;
  if (theta < A60)
    r = 1;
  else if (theta < A120)
    r = 2 * M * cos(theta - A30) - 1;
  else if (theta < A180)
    r = 2 * M * cos(theta + A30) + 1;
  else if (theta < A240)
    r = -1;
  else if (theta < A300)
    r = 2 * M * cos(theta - A30) + 1;
  else
    r = 2 * M * cos(theta + A30) - 1;
  return 0.5 + 0.5 * r;
}

double mdis3(double M, double theta) {
  double x;
  theta -= M_A90;
  x = frac(theta / (2 * M_PI));
  if (x < 0)
    x += 1;
  if (x < M_1_3)
    return M * cos(2 * M_PI * (x - 0.083333));
  else if (x < M_2_3)
    return 0;
  else
    return M * cos(2 * M_PI * (x + 0.083333));
}

double msin(double M, double theta) {
  return 0.5 + M * 0.5 * sin(theta);
}

double mcos(double M, double theta) {
  return 0.5 + M * 0.5 * cos(theta);
}

double mthird(double M, double theta) {
  theta -= M_A90;
  return 0.5 + M * (0.5 / M_sqrt3_2) * (cos(theta) - cos(3*theta)/6.0);
}

double mmin(double M, double theta) {
  double a, b, c, min;
  theta -= M_A90;
  a = 0.5 + M * 0.5 * cos(theta);
  b = 0.5 + M * 0.5 * cos(theta - M_A120);
  c = 0.5 + M * 0.5 * cos(theta + M_A120);
  min = a;
  if (b < min) min = b;
  if (c < min) min = c;
  return a -= min;
}

double msquare(double M, double theta) {
  if (sin(theta) >= 0)
    return 0.5 + 0.5 * M;
  else
    return 0.5 - 0.5 * M;
}

double msquare120(double M, double theta) {
  double r;
  while (theta < 0)
    theta += M_2PI;
  while (theta > M_2PI)
    theta -= M_2PI;
  if (theta <= A60)
    r = 1;
  else if (theta <= A120)
    r = 0;
  else if (theta <= A240)
    r = -1;
  else if (theta <= A300)
    r = 0;
  else
    r = 1;
  return 0.5 + 0.5 * M * r;
}

double mhcos(double M, double theta) {
  double x = cos(theta);
  if (x > 0)
    return M * x;
  else
    return 0;
}

double mhsin(double M, double theta) {
  double x = sin(theta);
  if (x > 0)
    return M * x;
  else
    return 0;
}

double msqr180(double M, double theta) {
  while (theta < 0)
    theta += M_2PI;
  while (theta >= M_2PI)
    theta -= M_2PI;
  if (theta == 0 || theta == M_PI)
    return 0;
  else if (theta < M_PI)
    return 1;
  else
    return -1;
}

/*
  M is the value of delta in rad
*/
double msimple(double M, double theta) {
  while (theta < 0)
    theta += M_2PI;
  while (theta >= M_2PI)
    theta -= M_2PI;
  /* it will always use half of delta */
  M /= 2;
  if (theta == M_A90 - M || theta == M_A90 + M)
    return 0.5;
  else if (theta == M_A270 - M || theta == M_A270 + M)
    return -0.5;
  else if (theta > M_A90 - M &&  theta < M_A90 + M)
    return 1;
  else if (theta > M_A270 - M && theta < M_A270 + M)
    return -1;
  else
    return 0;
}
