/* runge5_1.h --- Rungekutta header file */

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

#ifndef runge51_h
#define runge51_h

#include "runge.h"

typedef struct {
  double t0, t, tend, dt;
  double x[MAXX], px[MAXX];
  int nx;
  void (* output)(void);
  double output_interval;
  void (* hash)(void);
} runge51_t;

/* typedef struct runge51 runge51_t; */

void runge51_init(runge51_t * self, double t0, double dt, double tend,
                  int numxeqs);
void runge51_fputs_x(runge51_t * self, FILE * stream);
int runge51_loadx(runge51_t * self, const char *filename);
void runge51_outputs(runge51_t * self, void (*output)(void),
                     double output_interval, void (*hash)(void));
int runge51_run(runge51_t * self, void (*input)(void), void (*state_eqn)(void));

#endif
