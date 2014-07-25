#ifndef _fourier_serie_h
#define _fourier_serie_h

#include <math.h>
#include <complex.h>

#include "rvector.h"

#define MAXHARM 50      // número máximo de harmônicos

struct fourse {
  rvector_t *a, *b;
  size_t n;
};

typedef struct fourse fourse_t;

#define fourse_a(fs, i) (*((double *)(fs->a->data) + i))
#define fourse_b(fs, i) (*((double *)(fs->b->data) + i))

double fourse_sin(double *, int, int);
double fourse_0(double *, int);
double fourse_cos(double *, int, int);

fourse_t * fourse_new(int nharm);
void fourse_free(fourse_t * self);
fourse_t * fourse_dup(fourse_t * self);
fourse_t * fourse_initWithWav(fourse_t * self, int nharm, double * data, int data_size);

double fourse_c(fourse_t * self, int i);
double fourse_phi(fourse_t * self, int i);

void fourse_add_angle(fourse_t * self, double gamma);
void fourse_add(fourse_t * res, fourse_t * four1, fourse_t * four2);
void fourse_sub(fourse_t * res, fourse_t * four1, fourse_t * four2);

int fourse_load_dat(fourse_t *frr, const char *);
void fourse_write_dat(fourse_t *frr, char *);
void fourse_calc_all(fourse_t *frr, double * , int size);
void fourse_calc_an(fourse_t *frr, double * data, int size);
void fourse_calc_bn(fourse_t *frr, double * data, int size);
double fourse_get_norm(fourse_t *frr);
double fourse_eval(fourse_t *frr, double x);
double fourse_eval_cos(fourse_t *frr, double x);
double fourse_eval_sin(fourse_t *frr, double x);
  
int fourse_puto(fourse_t * self, FILE * stream);
int fourse_geto(fourse_t * self, FILE * stream);

void fourse_calcComplexCoefs(fourse_t * self);
void fourse_calcRealCoefs(fourse_t * self);

void putRealCoefs(fourse_t * self, FILE * stream);
void putComplexCoefs(fourse_t * self, FILE * stream);

#endif
