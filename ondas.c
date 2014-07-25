#include <math.h>

#include "constant.h"
#include "ondas.h"
#include "func.h"

double trapezoidal(double x)
{
  x=frac(x/(2*M_PI));
  if (x<0.083333)
    return -12*x;
  else if (x<0.416666)
    return -1.;
  else if (x<0.583333)
    return 12*x-6;
  else if (x<0.916666)
    return 1.;
  else
    return -12*(x-1);
}

double fluxotrap(double x)
{
  x/=2*M_PI;
  x=frac(x);
  if (x<0.0833333)
    return M_5PI_12-M_12PI*sqr(x);
  else if (x<0.4166666)
    return M_PI_2-M_2PI*x;
  else if (x<0.5833333)
    return M_31PI_12-M_12PI*x+M_12PI*sqr(x);
  else if (x<0.9166666)
    return -M_3PI_2+M_2PI*x;
  else
    return M_5PI_12-M_12PI*sqr(x-1);
}

double trap120(double x)
{
  x=frac(x/(2*M_PI));
  if (x<0.083333)
    return 12*x;
  else if (x<0.416666)
    return 1.;
  else if (x<0.583333)
    return -12*x+6;
  else if (x<0.916666)
    return -1.;
  else
    return 12*(x-1);
}

double trap60(double x)
{
  moduni(x);
  if (x<0.1666667)
    return 6*x;
  else if (x<0.3333333)
    return 1.;
  else if (x<0.6666667)
    return 3-6*x;
  else if (x<0.83333)
    return -1.;
  else
    return 6*x-6;
}

double trap90(double x)
{
  moduni(x);
  if (x < 0.125)
    return 8 * x;
  else if (x < 0.375)
    return 1.;
  else if (x < 0.625)
    return 4. - 8 * x;
  else if (x < 0.875)
    return -1.;
  else
    return 8 * x - 8;
}

double semitrap(double x)
{
  moduni(x);
  if (x<0.0833333)
    return 6*x;
  else if (x<0.25)
    return 3*x+0.25;
  else if (x<0.4166666)
    return 1.75-3*x;
  else if (x<0.5833333)
    return 3-6*x;
  else if (x<0.75)
    return 1.25-3*x;
  else if (x<0.9166666)
    return 3*x-3.25;
  else
    return 6*x-6;
}

double quad120(double theta)
{
  double x=theta-A30;
  moduni(x);
  if (x>=0 && x<0.33333)
    return 1;
  else if (x>=0.33333 && x<0.5)
    return 0;
  else if (x>=0.5 && x<0.83333)
    return -1;
  else if (x>=0.83333 && x<1.)
    return 0;
  else
    return 0;
}

double square_pulse(double theta, double theta_on, double theta_off) {
  if (theta == theta_on || theta == theta_off)
    return 0.5;
  else if (theta > theta_on && theta < theta_off)
    return 1;
  else
    return 0;
}

double flux_rad(double Bf, double theta1, double theta2, double theta)
{
  double s, x=theta;
  moddup(x);
  theta1/=M_PI;
  theta2/=M_PI;
  if (x>1) {
    s=-1;
    x=x-1;
    }
  else
    s=1;
  if (x>M_1_2)
    x=1-x;
  if (x>theta1 && x<=theta2)
    return s*Bf/(theta2-theta1)*(x-theta1);
  else if (x>theta2 && x<=M_1_2)
    return s*((1-Bf)/(M_1_2-theta2)*(x-theta2)+Bf);
  else
    return 0;
}

double trap120d(double theta, double d)
{
  double x=theta;
  moduni(x);
  x*=2;
  d/=M_PI;
  if (d<=0  || d>M_1_6)
    return 0;
  else if (x<M_1_6-d)
    return 0;
  else if (x<M_1_6)
    return  (x-M_1_6+d)/d;
  else if (x<M_5_6)
    return 1;
  else if (x<M_5_6+d)
    return (M_5_6+d-x)/d;
  else if (x<M_7_6-d)
    return 0;
  else if (x<M_7_6)
    return (M_7_6-d-x)/d;
  else if (x<M_11_6)
    return -1;
  else if (x<M_11_6+d)
    return (x-M_11_6-d)/d;
  else
    return 0;
}

double quad180(double theta)
{
  double x;
  x=frac(theta/(2*M_PI));
  if (x>=0 && x<0.5)
    return 1;
  else if (x>=0.5 && x<1)
    return -1;
  else
    return 0;
}
 
double trap(double theta)
{
  double x;
  x=frac((theta-A30)/(2*M_PI));
  if (x>=0 && x<0.33333)
    return 1;
  else if (x>=0.33333 && x<0.5)
    return 5-12*x;
  else if (x>=0.5 && x<0.83333)
    return -1;
  else if (x>=0.83333 && x<1.)
    return 12*x-11;
  else
    return 0;
}

double dis3(double IM, double theta)
{
  double x;
  x=frac(theta/(2*M_PI));
  if (x<0)
    x+=1;
  if (x<0.33333)
    return IM*cos(2*M_PI*(x-0.083333));
  else if (x<0.66666)
    return 0;
  else
    return IM*cos(2*M_PI*(x+0.083333));
}

double mvpwm(double IM, double theta)
{
  double x, t0, t1, t2;
  int plane;
  x=theta/(A60);
  plane=(int) floor(x) % 6;
  x=frac(x);
  if (x<0)
    x+=1;
  t1=IM*sin(A60*(1-x));
  t2=IM*sin(A60*x);
  t0=1-t1-t2;
  switch (plane) {
    case 0:
      return t2;
    case 1:
      return 1;
    case 2:
      return t2+t1;
    case 3:
      return t1+t0;
    case 4:
      return 0;
    default:
      return t0;
    }
}

double trap_ab(double theta, double a, double b)
{
  double x=theta;
  moduni(x);
  x*=2;
  a/=M_PI;
  b/=M_PI;
  if (a+b<0 || x<M_1_2-a-b)
    return 0;
  else if (x<M_1_2-b)
    return (x-M_1_2+a+b)/a;
  else if (x<M_1_2+b)
    return 1;
  else if (x<M_1_2+b+a)
    return (M_1_2+b+a-x)/a;
  else if (x<M_3_2-b-a)
    return 0;
  else if (x<M_3_2-b)
    return (M_3_2-b-a-x)/a;
  else if (x<M_3_2+b)
    return -1;
  else if (x<M_3_2+b+a)
    return (x-M_3_2-b-a)/a;
  else
    return 0;
}

double flux_open(double theta) {
  double s, x=theta;
  double xi[6], yi[6];

  moddup(x);
  if (x>1) {
    s=-1;
    x=x-1;
    }
  else
    s=1;
  if (x>M_1_2)
    x=1-x;

  xi[0]=M_5_36;    yi[0]=0;
  xi[1]=0.16;      yi[1]=0.03;
  xi[2]=M_1_6;     yi[2]=0.065;
  xi[3]=M_1_6;     yi[3]=0.935;
  xi[4]=0.1733;     yi[4]=0.97;
  xi[5]=M_7_36;    yi[5]=1;

  if (x>xi[0] && x<=xi[2])
    return s*((x-xi[1])/(xi[0]-xi[1])*(x-xi[2])/(xi[0]-xi[2])*yi[0]+
              (x-xi[0])/(xi[1]-xi[0])*(x-xi[2])/(xi[1]-xi[2])*yi[1]+
              (x-xi[0])/(xi[2]-xi[0])*(x-xi[1])/(xi[2]-xi[1])*yi[2]);
  else if (x>xi[3] && x<=xi[5])
    return s*((x-xi[4])/(xi[3]-xi[4])*(x-xi[5])/(xi[3]-xi[5])*yi[3]+
              (x-xi[3])/(xi[4]-xi[3])*(x-xi[5])/(xi[4]-xi[5])*yi[4]+
              (x-xi[3])/(xi[5]-xi[3])*(x-xi[4])/(xi[5]-xi[4])*yi[5]);
  else if (x>xi[5])
    return s;
  else
    return 0;
}

double pwmdiss1(double M, double ang) {
  double fli;
  ang /= M_A360;
  ang -= floor(ang);
  ang *= M_A360;

  if (ang >= M_A300)
    fli = 0.5 * M * cos(ang + M_A30) + 0.5;
  else if (ang >= M_A240)
    fli = 0.5 * sqrt(3) * M * cos(ang) + 0.5;
  else if (ang >= M_A180)
    fli = 0.5 * M * cos(ang - M_A30) + 0.5;
  else if (ang >= M_A120)
    fli = 0.5 * M * cos(ang + M_A30) + 0.5;
  else if (ang >= M_A60)
    fli = 0.5 * sqrt(3) * M * cos(ang) + 0.5;
  else
    fli = 0.5 * M * cos(ang - M_A30) + 0.5;

  if (fli < 0)
    fli = 0;
  else if (fli > 1)
    fli = 1;

  return fli;
}

double pwmdiss2(double M, double ang) {
  double fli;
  ang /= M_A360;
  ang -= floor(ang);
  ang *= M_A360;

  if (ang >= M_A330)
    fli = 1;
  else if (ang >= M_A270)
    fli = M * cos(ang + M_A30);
  else if (ang >= M_A210)
    fli = M * cos(ang - M_A30) + 1;
  else if (ang >= M_A150)
    fli = 0;
  else if (ang >= M_A90)
    fli = M * cos(ang + M_A30) + 1;
  else if (ang >= M_A30)
    fli = M * cos(ang - M_A30);
  else
    fli = 1;

  if (fli < 0)
    fli = 0;
  else if (fli > 1)
    fli = 1;

  return fli;
}

double pwmdiss3(double M, double ang) {
  double fli;
  ang /= M_A360;
  ang -= floor(ang);
  ang *= M_A360;

  if (ang >= M_A300)
    fli = M * cos(ang + M_A30);
  else if (ang >= M_A240)
    fli = M * cos(ang - M_A30) + 1;
  else if (ang >= M_A180)
    fli = 0;
  else if (ang >= M_A120)
    fli = M * cos(ang + M_A30) + 1;
  else if (ang >= M_A60)
    fli = M * cos(ang - M_A30);
  else
    fli = 1;

  if (fli < 0)
    fli = 0;
  else if (fli > 1)
    fli = 1;

  return fli;
}

double pwmdiss4(double M, double ang) {
  double fli;
  ang /= M_A360;
  ang -= floor(ang);
  ang *= M_A360;
  if (ang >= M_A240)
    fli = M * cos(ang + M_A30);
  else if (ang >= M_A120)
    fli = 0;
  else
    fli = M * cos(ang - M_A30);

  if (fli < 0)
    fli = 0;
  else if (fli > 1)
    fli = 1;

  return fli;
}

double mcos_w_sat(double M, double ang) {
  double fli;
  fli = M * 0.5 * cos(ang) + 0.5;
  if (fli > 1) fli = 1;
  else if (fli < 0) fli = 0;
  return fli;
}
