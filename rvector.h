#ifndef _RVECTOR_H
#define _RVECTOR_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "wav.h"
#include "vvector.h"

/*
   Defining type 'rvec':
   maxn is the maximum number of elements of data. It is evaluated from
   RVECTOR_BLKSIZE, wich defines the size of each BLOCK;
   n is the actual number of elements in data;
*/

/*
   The son has an identical structure
*/
typedef struct vvec rvector_t;

/* Identical methods */

#define rvector_alloc vvector_alloc
#define rvector_init(self, n) vvector_init(self, n, sizeof(double))
#define rvector_destroy(self) vvector_destroy(self)
#define rvector_free(self) vvector_destroy(self)
#define rvector_resize(self, n) vvector_resize(self, n)
#define rvector_clear(self) vvector_resize(self, 0)
#define rvector_setDebug(debug_mode) vvector_setDebug(debug_mode)
#define rvector_getDebug() vvector_getDebug()
#define rvector_dup(self) vvector_dup(self)

rvector_t * rvector_new(int n);

/*
   Accessing elements
*/
#define RVECTOR(self,i) (*((double *)(self->data) + i))
#define rvector_n(self) vvector_n(self)
#define rvector_maxn(self) vvector_maxn(self)
// #define rvector_size(self) vvector_size(self) // not used
#define rvector_data(self) vvector_data(self)

/* Callback methods */

double rvector_get(rvector_t * self, int i);
double rvector_get_cycle(rvector_t * self, int i);
void rvector_put(rvector_t * self, int i, double x);
int rvector_append(rvector_t * self, double x);

/*
   Calling old wav object

   wav object is obsolet, the routines bellow need to be rewritten
   directly on rvector object
*/

#define rvector_cutpeak(self,dmax) wav_cutpeak(self->data, self->n, dmax)
#define rvector_deriv(self,i) wav_deriv(self->data, self->n, i)
#define rvector_integ(self,i1,i2) wav_integ(self->data, self->n, i1, i2)
#define rvector_normalize(self) wav_normalize(self->data, self->n)
#define rvector_getmax(self) wav_getmax(self->data, self->n)
#define rvector_getmin(self)  wav_getmin(self->data, self->n)
#define rvector_getin(self,t) wav_getin((double *)self->data, self->n, t)
#define rvector_putin(self,t,x) wav_putin(self->data, self->n, t, x)

/*
  New methods
*/

int rvector_load(rvector_t * self, const char *name);
int rvector_write(rvector_t * self, const char *name);
int rvector_fputs(rvector_t * self, FILE * stream);
int rvector_fgets(rvector_t * self, FILE * stream);
rvector_t * rvector_create_from_string(const char *str);
rvector_t *rvector_from_file_line(const char *filename, int line);
int rvector_load_dag(rvector_t * self, int column, const char* name);
int rvector_write_dag(rvector_t * self, const char *name);
int rvector_read_column(rvector_t * self, const char * filename, int column);
double rvector_sum(rvector_t *x);
double rvector_sqrsum (rvector_t *x);
double rvector_avg(rvector_t *x);
double rvector_rms(rvector_t *x);

#ifdef __cplusplus
}
#endif

#endif
