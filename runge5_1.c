/* runge5.c --- implements Fifth Order Runge-Kutta Method */

/* Copyright (C) 2003 José Roberto Boffino de Almeida Monteiro
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

#include <stdio.h>

#include "runge.h"

void RUNGE_settimes(RUNGE *runge, double tini, double deltat, double tf)
{
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
  double xn[MAXX];
  double k[5][MAXX];
  double tn, h; 

  ce->t = ce->t0;
  h = ce->dt;

  count=0;
  interval=(int)((ce->t - ce->tend)/(h * ce -> maxpoints)) + 1;

  if (input)
    input();

  if (state_eqn)
    state_eqn();

  if (output)
    output();
 
  if (state_eqn)
    while (ce->t <= ce->tend) {
      /* input function */
      if (inpcut)
	input();

      tn = ce->t;

      //      printf("tn=%g; h=%g\n", tn, h);

      for (i=0; i < ce->nx; i++)
	xn[i]=ce->x[i];

      state_eqn();                         // f(xn, tn)      
      for(i=0; i < ce->nx; i++)
	k[0][i]= h/3. * ce->px[i];          // k0=h/3*f(xn, tn)


      for(i=0; i < ce->nx; i++)
	ce->x[i] = xn[i] + k[0][i];        // xn+k0
      ce->t = tn + h/3.;                   // tn+h/3
      state_eqn();                         // f(xn+k0, tn+h/3)
      for(i=0; i < ce-> nx; i++)
	k[1][i]= h/3. * ce->px[i];          // k1=h/3*f

      for(i=0; i < ce-> nx; i++)
	ce->x[i] = xn[i] + 0.5*(k[0][i]+k[1][i]); // xn+0.5(k0+k1)
      // ce->t = tn + h/3.;                       // tn+h/3
      state_eqn();                                // f(xn+0.5(k0+k1), tn+h/3)
      for(i=0; i < ce->nx; i++)
	k[2][i] = h/3. * ce->px[i];               // k2=h/3*f

      for(i=0; i < ce-> nx; i++)
	ce->x[i] = xn[i] + 0.125*(3.*k[0][i]+9.*k[2][i]); 
                                               // xn+0.125(3k0+9k2)
      ce->t = tn + h/2.;                       // tn+h/2
      state_eqn();                             // f(xn+0.125(3k0+9k2), tn+h/2)
      for(i=0; i < ce->nx; i++)
	k[3][i] = h/3. * ce->px[i];            // k3=h/3*f
      
      for(i=0; i < ce-> nx; i++)
	ce->x[i] = xn[i] + 0.5*(3.*k[0][i]-9.*k[2][i]+12.*k[3][i]); 
                                               // xn+0.5(3k0-9k2+12k3)
      ce->t = tn + h;                          // tn+h
      state_eqn();                             // f(xn+0.5(3k0-9k2+12k3), tn+h)
      for(i=0; i < ce->nx; i++)
	k[4][i] = h/3. * ce->px[i];             // k4=h/3*f
      
      for(i=0; i < ce->nx; i++)
	ce->x[i] = xn[i] + k[0][i]/2. + 2.*k[3][i] + k[4][i]/2.;    
                                               // x = xn + k0/2 + 2k3 + k5/2
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

