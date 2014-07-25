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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "strbms.h"
#include "runge51.h"

void runge51_init(runge51_t * self, double t0, double dt, double tend, int numxeqs) {
  self -> t0 = t0;
  self -> dt = dt;
  self -> tend = tend;
  self -> nx = numxeqs;
  self -> output = NULL;
  self -> hash = NULL;
  self -> output_interval = (tend - t0) / MAXPOINTS;
}

/*
  Read state vars from stream in the format:
  x1
  x2
  x3
  ...
  xn
*/
int runge51_fgets_x(runge51_t * self, FILE * stream) {
  size_t len = 0;
  int i = 0;
  char * s = NULL;
  while (getline(& s, & len, stream) > 0) {
    s = strtrim(s);
    if (strlen(s) > 0) {
      self -> x[i] = atof(s);
      i++;
    }
  }
  len = 0;
  free(s);
  return i;
}

/*
  Load state vars from filename using fgets_x function.
*/
int runge51_loadx(runge51_t * self, const char *filename) {
  FILE *fin;
  int res;

  if ((fin=fopen(filename, "r")) == 0) {
    res = runge51_fgets_x(self, fin);
    fclose(fin);
  }
  else
    res = 0;
  return res;
}

void runge51_fputs_x(runge51_t * self, FILE * stream) {
  int i;
  for (i = 0; i < self -> nx ;i++)
    fprintf(stream, "%g\n", self -> x[i]);
}

void runge51_outputs(runge51_t * self, void (* output)(),
                     double output_interval, void (* hash)()) {
  self -> output = output;
  self -> hash = hash;
  self -> output_interval = output_interval;
}

/*
  Return 0 on Error
*/
int runge51_run(runge51_t * self, void (* input)(void), void (* state_eqn)(void)) {
  double tout = self -> t0;
  int i;
  double xn[MAXX];
  double k[5][MAXX];
  double tn, h; 

  self -> t = self -> t0;
  h = self -> dt;

  if (input)
    input();

  if (state_eqn)
    state_eqn();
  else {
    fprintf(stderr, "runge51_run: ERR: no state equation function\n");
    return 0;
  }

  if (self -> output)
    self -> output();
 
  while (self->t <= self->tend) {
    /* input function */
    if (input)
      input();

    tn = self->t;

    //      printf("tn=%g; h=%g\n", tn, h);

    for (i=0; i < self->nx; i++)
      xn[i]=self->x[i];

    state_eqn();                         // f(xn, tn)      
    for(i=0; i < self->nx; i++)
      k[0][i]= h/3. * self->px[i];          // k0=h/3*f(xn, tn)


    for(i=0; i < self->nx; i++)
      self->x[i] = xn[i] + k[0][i];        // xn+k0
    self->t = tn + h/3.;                   // tn+h/3
    state_eqn();                         // f(xn+k0, tn+h/3)
    for(i=0; i < self-> nx; i++)
      k[1][i]= h/3. * self->px[i];          // k1=h/3*f

    for(i=0; i < self-> nx; i++)
      self->x[i] = xn[i] + 0.5*(k[0][i]+k[1][i]); // xn+0.5(k0+k1)
    // self->t = tn + h/3.;                       // tn+h/3
    state_eqn();                                // f(xn+0.5(k0+k1), tn+h/3)
    for(i=0; i < self->nx; i++)
      k[2][i] = h/3. * self->px[i];               // k2=h/3*f

    for(i=0; i < self-> nx; i++)
      self->x[i] = xn[i] + 0.125*(3.*k[0][i]+9.*k[2][i]); 
                                               // xn+0.125(3k0+9k2)
    self->t = tn + h/2.;                       // tn+h/2
    state_eqn();                             // f(xn+0.125(3k0+9k2), tn+h/2)
    for(i=0; i < self->nx; i++)
      k[3][i] = h/3. * self->px[i];            // k3=h/3*f
      
    for(i=0; i < self-> nx; i++)
      self->x[i] = xn[i] + 0.5*(3.*k[0][i]-9.*k[2][i]+12.*k[3][i]); 
                                               // xn+0.5(3k0-9k2+12k3)
    self->t = tn + h;                          // tn+h
    state_eqn();                             // f(xn+0.5(3k0-9k2+12k3), tn+h)
    for(i=0; i < self->nx; i++)
      k[4][i] = h/3. * self->px[i];             // k4=h/3*f
      
    for(i=0; i < self->nx; i++)
      self->x[i] = xn[i] + k[0][i]/2. + 2.*k[3][i] + k[4][i]/2.;    
                                               // x = xn + k0/2 + 2k3 + k5/2
    /* output function */
    if (self -> t >= tout) {
      if (self -> output)
        self -> output();
      if (self -> hash)
        self -> hash();
      tout += self -> output_interval;
    }
  }
  return 1;
}

