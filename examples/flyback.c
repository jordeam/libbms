/*
  flyback.c --- flyback switchmode power supply example
*/

/*
 *
 * Copyright (C) 2003 José Roberto Boffino de Almeida Monteiro
 *   <jrm@sel.eesc.usp.br>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 *
 */

/* $Id:$ */

/*
  Compile with: gcc flyback.c runge5.c -o flyback -lm
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "runge.h"

#define sqr(x) ((x)*(x))

#define iL1 trans.x[0]
#define vCF trans.x[1]

#define diL1 trans.px[0]
#define dvCF trans.px[1]

#define ROFF 100e3
#define RON 0.1
#define VDMAX 1

#define duty_MAX 0.45

RUNGE trans;

FILE *fout;

double v, a, N1, N2, v1, v2,
  i1, i2, iCF, iL, 
  L1, L2,
  CF,
  RL, RD, RQ1;

/* PWM */
double PWM_T, PWM_0, PWM_duty, duty;

void calc_PWM_duty()
{
  if (trans.t<PWM_0)
    PWM_duty=1.0;
  else {
    double x = (trans.t-PWM_0)/PWM_T;
    PWM_duty=x-floor(x);
  }
}

void trans_initial() {
  iL1=0;
  vCF=0;

  v=180*sqrt(2);
  N1=161; N2=4;
  a=N1/N2;

  L1=5.18e-3; // AL=200e-9
  L2=L1/sqr(a);

  CF=1000e-6;

  RD=ROFF;
  RQ1=ROFF;

  /* PWM */
  PWM_T=33e-6;
  PWM_0=0.;
}

void trans_input(void) {
  v1 = L1*diL1;
  v2 = -v1/a;

  RL=(vCF>0.1)? vCF/2 : ROFF;

  RD= v2<0 ? ROFF : RON;
  if (i2<0) RD=ROFF;
  if (i2*RD > VDMAX) RD=VDMAX/i2;

  calc_PWM_duty();

  duty=10*(5-vCF);
  if(duty>duty_MAX)
    duty=duty_MAX;
  if (duty<0)
    duty=0;

  if (duty<PWM_duty) {
    RQ1+=2e3;
    if (RQ1>ROFF) RQ1=ROFF;
  }
  else {
    RQ1=RQ1-1e3;
    if(RQ1<RON) RQ1=RON;
  }
}

void trans_state_eqn(void) {

  i1=(sqr(a)*RD*iL1+a*vCF+v)/(RQ1+sqr(a)*RD);
  i2=(iL1-i1)*a;
  iCF=i2-vCF/RL;
 
  diL1=(v-i1*RQ1)/L1;
  dvCF=iCF/CF;
}

void trans_output(void) {
  iL=vCF/RL;
  fprintf(fout, "%g %g %g %g %g %g %g %g %g\n", trans.t, v-v1, vCF, i1, i2, iL1, RD, duty, PWM_duty);
}

void hash()
{
  printf("."); fflush(stdout);
}

int main() {
  
  printf("Beginning...\n");

  fout=fopen("flyblack.dat", "w");
  
  //  t0=0;           // tempo inicial
  //  dt=1.e-9;      // delta t
  //  tend=200e-6;      // tempo final
  //  nx=2;          // numero de variaveis de estado

  RUNGE_settimes(&trans, 0, 5.e-9, 1600e-6);
  RUNGE_setxeqs(&trans, 2);
  trans_initial();
  RUNGE_run(&trans, trans_input, trans_state_eqn, trans_output, hash);

  fclose(fout);
  printf("\nfinished !\n");

  exit(0);
}
