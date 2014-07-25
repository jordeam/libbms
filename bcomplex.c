#include <math.h>

#include "bcomplex.h"

COMPLEX complex(double real_part, double imag_part)
{
  COMPLEX z;
  z.r=real_part;
  z.i=imag_part;
  return z;
}

COMPLEX c_add(COMPLEX z1, COMPLEX z2)
{
  COMPLEX z;
  z.r=z1.r+z2.r;
  z.i=z1.i+z2.i;
  return z;
}

COMPLEX c_sub(COMPLEX z1, COMPLEX z2)
{
  COMPLEX z;
  z.r=z1.r-z2.r;
  z.i=z1.i-z2.i;
  return z;
}

COMPLEX c_mul(COMPLEX z1, COMPLEX z2)
{
  COMPLEX z;
  z.r = z1.r*z2.r - z1.i*z2.i;
  z.i = z1.i*z2.r + z1.r*z2.i;
  return z;
}

COMPLEX c_div(COMPLEX z1, COMPLEX z2)
{
  COMPLEX z;
  double hyp;
  hyp = z2.r*z2.r + z2.i*z2.i;
  z.r = (z1.r*z2.r + z1.i*z2.i)/hyp;
  z.i = (z1.i*z2.r - z1.r*z2.i)/hyp;
  return z;
}

COMPLEX c_exp(COMPLEX z1)
{
  COMPLEX z;
  z.r = exp(z1.r)*cos(z1.i);
  z.i = exp(z1.r)*sin(z1.i);
  return z;
}

double c_abs(COMPLEX z)
{
  return sqrt(z.r*z.r + z.i*z.i);
}
