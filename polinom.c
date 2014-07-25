#include <math.h>

#include "constant.h"
#include "func.h"
#include "polinom.h"

double pln_FN(pln *p, double x, int i)
{
  double NM;
  int j;
  NM=1;
  for(j=1; j<=p->N; j++)
    if (i!=j)
      NM*=(x-p->xi[j]);
  return NM;
}

double pln_FD(pln *p, int i)
{
  double NM;
  int j;
  NM=1;
  for (j=1; j<=p->N; j++)
    if (i!=j)
      NM*=(p->xi[i]-p->xi[j]);
  return NM;
}

double pln_P(pln *p, double x)
{
  int i;
  double PS;
  PS=0;
  for (i=1; i<=p->N; i++)
    PS+=pln_FN(p, x, i)*p->yi[i]/pln_FD(p, i);
  return PS;
}

double Pol(double x)
{
  pln pol1, pol4, pol5, pol6, pol7, pol8;

    pol1.N=10;
    pol1.xi[1]=41.25;
    pol1.xi[2]=43.125;
    pol1.xi[3]=45;
    pol1.xi[4]=46.875;
    pol1.xi[5]=48.75;
    pol1.xi[6]=53.25;
    pol1.xi[7]=54.675;
    pol1.xi[8]=56.1;
    pol1.xi[9]=57.525;
    pol1.xi[10]=58.95;

    pol1.yi[1]=0.1;
    pol1.yi[2]=0.025;
    pol1.yi[3]=0.0;
    pol1.yi[4]=0.025;
    pol1.yi[5]=0.1;
    pol1.yi[6]=0.8;
    pol1.yi[7]=0.96;
    pol1.yi[8]=1;
    pol1.yi[9]=0.96;
    pol1.yi[10]=0.8;
   
    pol4.N=2;
    pol4.xi[1]=56.1;
    pol4.xi[2]=63.6;
    pol4.yi[1]=1;
    pol4.yi[2]=1;

    pol5.N=3;
    pol5.xi[1]=56.1;
    pol5.xi[2]=63.6;
    pol5.xi[3]=69.4;
    pol5.yi[1]=1;
    pol5.yi[2]=1;
    pol5.yi[3]=0.8;

    pol6.N=3;
    pol6.xi[1]=69.4;
    pol6.xi[2]=75.6;
    pol6.xi[3]=82.4;
    pol6.yi[1]=0.8;
    pol6.yi[2]=1;
    pol6.yi[3]=1;

    pol7.N=2;
    pol7.xi[1]=75.6;
    pol7.xi[2]=82.4;
    pol7.yi[1]=1;
    pol7.yi[2]=1;

    pol8.N=3;
    pol8.xi[1]=75.6;
    pol8.xi[2]=82.4;
    pol8.xi[3]=90;
    pol8.yi[1]=1;
    pol8.yi[2]=1;
    pol8.yi[3]=0.8;

  if (x<=56.1)
    return pln_P(&pol1, x);
  else if (x<63.6)
    return pln_P(&pol4, x);
  else if (x<69.4)
    return pln_P(&pol5, x);
  else if (x<75.6)
    return pln_P(&pol6, x);
  else if (x<82.4)
    return pln_P(&pol7, x);
  else
    return pln_P(&pol8, x);
}

double trapezoidal1(double theta)
{
  double x;
  x=frac(theta/(2*M_PI));
  if (x<0)
    theta+=1;
  if (x<0.25)
    return Pol(180*x+45);
  else if (x<0.5)
    return Pol(135-180*x);
  else if (x<0.75)
    return -Pol(180*x-45);
  else
    return -Pol(225-180*x);
}

