#ifndef _fourier_h
#define _fourier_h

#include <math.h>
#include <complex.h>

#define MAXHARM 50      // número máximo de harmônicos

typedef struct {
  double a[MAXHARM], b[MAXHARM];
  __complex__ double c[MAXHARM], c_[MAXHARM];
  int   nharm;
} fourier;

double fourier_sin(double *, int, int);
double fourier_0(double *, int);
double fourier_cos(double *, int, int);

fourier * fourier_alloc(void);
void fourier_free(fourier * self);
fourier * fourier_init(fourier * self, int nharm);
fourier * fourier_initWithWav(fourier * self, int nharm, double * data, int data_size);

void fourier_zero(fourier *frr);
int fourier_load(fourier *frr, const char *);
void fourier_write(fourier *frr, char *);
void fourier_calc_all(fourier *frr, double * , int size);
void fourier_calc_an(fourier *frr, double * data, int size);
void fourier_calc_bn(fourier *frr, double * data, int size);
double fourier_get_norm(fourier *frr);
double fourier_eval(fourier *frr, double x);
double fourier_eval_cos(fourier *frr, double x);
double fourier_eval_sin(fourier *frr, double x);
  
int fourier_puto(fourier * self, FILE * stream);
int fourier_geto(fourier * self, FILE * stream);

void fourier_calcComplexCoefs(fourier * self);
void fourier_calcRealCoefs(fourier * self);

void putRealCoefs(fourier * self, FILE * stream);
void putComplexCoefs(fourier * self, FILE * stream);

#endif
