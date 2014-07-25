/*
  This program implements Fourth Order Runge-Kutta Method
*/

#include <stdio.h>

#include "runge.h"

void RUNGE_settimes(RUNGE *runge, double tini, double deltat, double tf) {
  runge->t0=tini;
  runge->dt=deltat;
  runge->tend=tf;
  runge -> maxpoints = MAXPOINTS;
}

void RUNGE_setmaxpoints(RUNGE * runge, int maxpoints) {
  runge -> maxpoints = maxpoints;
}

void RUNGE_setxeqs(RUNGE *runge, int numxeqs)
{
  runge->nx=numxeqs;
}

int RUNGE_loadx(RUNGE *runge, const char *filename)
{
  FILE *fin;
  char szdata[SZDATASIZE]; 
  if ((fin=fopen(filename, "r"))) {
    if (fgets(szdata, SZDATASIZE, fin)!=NULL && 
	sscanf(szdata, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &runge->x[0], &runge->x[1], &runge->x[2], &runge->x[3], &runge->x[4], &runge->x[5], &runge->x[6], &runge->x[7], &runge->x[8], &runge->x[9])!=0)
      return 1;
    else
      return 0;
    fclose(fin);
  }
  else
    return 0;
}

int RUNGE_savex(RUNGE *runge, const char *filename)
{
  FILE *fout;
  int i;
  if ((fout=fopen(filename, "w"))) {
    for (i=0; i<runge->nx ;i++)
      fprintf(fout, "%g ", runge->x[i]);
    fprintf(fout, "\n");
    fclose(fout);
    return 1;
  }
  else
    return 0;
}

void RUNGE_run(RUNGE *ce, void (*input)(), void (*state_eqn)(), void (*output)(), void (*hash)())
{
  int count, interval, hash_count = 0;
  int i;
  double xo[MAXX], x1[MAXX];
  ce->t = ce->t0;

  count=0;
  interval=(int)((ce->t - ce->tend)/(ce->dt * ce -> maxpoints)) + 1;

  if (input)
    input();

  if (state_eqn)
    state_eqn();

  if (output)
    output();
 
  if (state_eqn)
    while (ce->t <= ce->tend) {
      /* input function */
      if (input)
	input();
      
      for(i=0; i < ce->nx; i++)
	x1[i] = xo[i] = ce->x[i];
      
      state_eqn();                // t, x
      
      for(i=0; i < ce->nx; i++) {
	ce->px[i] = ce->dt * ce->px[i] / 6;               // k1
	x1[i] += ce->px[i];
	ce->x[i] += 3 * ce->px[i];
      }
      
      ce->t += ce->dt / 2;
      state_eqn();                // t+dt/2, x+k1/2
      
      for(i=0; i < ce-> nx; i++) {
	ce->px[i] = ce->dt * ce->px[i];               // k2
	x1[i] += ce->px[i] / 3;
	ce->x[i] = xo[i] + ce->px[i] / 2;
      }
      
      state_eqn();                // t+dt/2, x+k2/2
      
      for(i=0; i < ce->nx; i++) {
	ce->px[i] = ce->dt * ce->px[i];            // k3
	x1[i] += ce->px[i] / 3;
	ce->x[i] = xo[i] + ce->px[i];
      }
      
      ce->t += ce->dt / 2;
      state_eqn();                // t+dt, x+k3
      
      for(i=0; i < ce->nx; i++)
	ce->x[i] = x1[i] + ce->dt * ce->px[i] / 6;    // x = x + dt*px/6
      
      /* output function */
      if (count++ % interval == 0 && output) {
	output();
	if (HASH) {
	  hash_count++;
	  if (hash_count % HASH == 0 && hash)
	    hash();
	}
      }
    }
}

