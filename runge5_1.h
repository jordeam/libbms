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

#define SZDATASIZE 256
#define MAXX 20
#define MAXPOINTS 2000

#define HASH 20 // each 100 of MAXPOINTS

typedef struct {
  double t0, t, tend, dt;
  double x[MAXX], px[MAXX];
  int nx, maxpoints;
} runge51_t;

void runge51_settimes(runge51_t *runge, double tini, double deltat, double tf);
void runge51_setmaxpoints(runge51_t * runge, int maxpoints);
void runge51_setxeqs(runge51_t *runge, int numxeqs);
int runge51_loadx(runge51_t *runge, const char *filename);
int runge51_savex(runge51_t *runge, const char *filename);
void runge51_run(runge51_t *ce, void (*input)(), void (*state_eqn)(), void (*output)(), void (*hash)());
