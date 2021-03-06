#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "constant.h"
#include "sysconst.h"
#include "func.h"
#include "strbms.h"

#include "log.h"
#include "fourier_serie.h"

/*
  alloc space and set default values
*/
fourse_t * fourse_new(int nharm) {
  fourse_t * self = malloc(sizeof(fourse_t));
  self -> a = rvector_new(nharm+1);
  self -> b = rvector_new(nharm+1);
  self -> n = nharm;
  return self;
}

/*
  alloc space and return a copy
*/
fourse_t * fourse_dup(fourse_t * self) {
  fourse_t * clone = fourse_new(self->n);
  /* copy a and b rvectors from self to clone */
  clone->a = rvector_dup(self->a);
  clone->b = rvector_dup(self->b);
  return clone;
}

/*
  add an angle to fourier series
*/
void fourse_add_angle(fourse_t * self, double gamma) {
  int i;
  double c0, c1, an, bn;
  for (i = 1; i <= self -> n; i++) {
    an = rvector_get(self->a, i);
    bn = rvector_get(self->b, i);
    c0 = sqrt(sqr(rvector_get(self->a, i)) + sqr(rvector_get(self->b, i)));
    rvector_put(self->a, i, an * cos(i * gamma) + bn * sin(i * gamma));
    rvector_put(self->b, i, bn * cos(i * gamma) - an * sin(i * gamma));
    c1 = sqrt(sqr(rvector_get(self->a, i)) + sqr(rvector_get(self->b, i)));
    if (c0 != c1)
      log_msg_3("in fourse_add_angle: i=%d, c0=%g, c1=%g", i, c0, c1);
  }
  log_msg("fourse_add_angle done");
}

/*
  Add two fourier series, four1 and four2, and put the result in res, which must be previously allocated.
*/
void fourse_add(fourse_t * res, fourse_t * four1, fourse_t * four2) {
  int i;
  /* get the minimum n */
  int n = four1 -> n;
  if (n > four2 -> n)
    n = four2 -> n;
  res -> n = n;
  for (i = 0; i <= n; i++) {
    rvector_put(res->a, i, rvector_get(four1->a, i) + rvector_get(four2->a, i));
    rvector_put(res->b, i, rvector_get(four1->b, i) + rvector_get(four2->b, i));
  }
}

/*
  Subtract two fourier series, four1 by four2, and put the result in res, which must be previously allocated.
*/
void fourse_sub(fourse_t * res, fourse_t * four1, fourse_t * four2) {
  int i;
  /* get the minimum n */
  int n = four1 -> n;
  if (n > four2 -> n)
    n = four2 -> n;
  res -> n = n;
  for (i = 0; i <= n; i++) {
    rvector_put(res->a, i, fourse_a(four1,i) - fourse_a(four2,i));
    rvector_put(res->b, i, fourse_b(four1,i) - fourse_b(four2,i));
  }
}

/*
  Return c=sqrt(a^2+b^2) for ith coefficient
*/
double fourse_c(fourse_t * self, int i) {
  return sqrt(sqr(rvector_get(self->a, i)) + sqr(rvector_get(self->b, i)));
}

/*
  Return the angle of complex number formed by real and imaginary of i_th coefficients
*/
double fourse_phi(fourse_t * self, int i) {
  return atan2(rvector_get(self->b, i), rvector_get(self->a, i));
}

/*
  Initializes all coeficients from data
*/
fourse_t * fourse_initWithWav(fourse_t * self, int n, double * data, int data_size) {
  self -> n = n;
  fourse_calc_all(self, data, data_size);
  return self;
}

/*
  Put values in fourier serie 
*/
void fourse_put(fourse_t * self, int i, double val_a, double val_b) {
  if (i > self->n)
    self->n = i;
  if (i >= self->a->n)
    rvector_resize(self->a, i+1);
  rvector_put(self->a, i, val_a);
  if (i >= self->b->n)
    rvector_resize(self->b, i+1);
  rvector_put(self->b, i, val_b);
}

double fourse_get_a(fourse_t * self, int n) {
  if (n > self->n)
    return 0;
  else
    return rvector_get(self->a, n);
}

double fourse_get_b(fourse_t * self, int n) {
  if (n > self->n)
    return 0;
  else
    return rvector_get(self->b, n);
}

int fourse_size(fourse_t * self) {
  return self->n;
}

/*
  load coeficients from a file DEPRECATED: DO NOT USE
*/
int fourse_load_dat(fourse_t * self, const char * name)  {
  FILE  *f;
  char  *s, c[STRSIZE];
  double ai, bi, lixo;
  int   i;

  rvector_resize(self->a, MAXHARM + 1);
  rvector_resize(self->b, MAXHARM + 1);
  
  for(i = 0; i < MAXHARM; i++) {
    rvector_put(self->a, i, 0);
    rvector_put(self->b, i, 0);
  }
  
  if(!(f = fopen(name,"r")))
    return 0;
  i = self -> n = 0;
  
  s = fgets(c, STRSIZE, f);
  while(s != 0 && i < MAXHARM) {
    sscanf(c, "%lf %lf %lf", &lixo, &ai, &bi);
    s=fgets(c, STRSIZE, f);
    if (frac(lixo)==0) {
      i=(int) lixo;
      if (i<MAXHARM) {
	if (ai!=0) rvector_put(self->a, i, ai);
	if (bi!=0) rvector_put(self->b, i, bi);
	if (self->n<i) self->n=i;
      }
    }
  }
  fclose(f);
  return 1;
}

/*
  returns the norm of a waveform
*/
double fourse_get_norm(fourse_t * self) {
  int i; 
  double sum;
  sum = sqr(fourse_a(self, 0));
  for (i = 1; i < self -> n; i++) 
    sum += sqr(rvector_get(self->a, i)) + sqr(rvector_get(self->b, i));
  return sqrt(sum);
}

/*
  Returns the value of f(x), considering all coeficients
*/
double fourse_eval(fourse_t * self, double x)
{
  double sum;
  int   i;
  sum = fourse_a(self,0);
  for (i=1; i <= self->n; i++)
    sum += rvector_get(self->a, i) * cos(i * x) + rvector_get(self->b, i) * sin(i * x);
  return sum;
}

/*
  Returns the value of f(x), considering the serie of sines
*/
double fourse_eval_sin(fourse_t * self, double x)
{
  double sum;
  int i;
  sum = 0;
  for (i = 0; i < self -> n; i++)
    sum += rvector_get(self->b, i) * sin(i * x);
  return sum;
}

/*
  Returns the value of f(x), considering the serie of cosines
*/
double fourse_eval_cos(fourse_t * self, double x)
{
  double sum;
  int   i;
  sum =fourse_a(self,0);
  for (i = 0; i <= self -> n; i++)
    sum += rvector_get(self->a, i) * cos(i * x);
  return sum;
}

/*
  Evaluates all coeficients of Fourie serie
  enter with a vector 'data' of size 'size'
*/
void fourse_calc_all(fourse_t * self, double *data, int size) {
  int i;
  rvector_put(self->a ,0, fourse_0(data, size));
  rvector_put(self->b, 0, 0);
  for (i = 1; i <= self -> n; i++) {
    rvector_put(self->a, i, fourse_cos(data, size, i));
    rvector_put(self->a, i, fourse_sin(data, size, i));
  }
}

/*
  Evaluates $a_n$ coeficientes of Fourier serie
*/
void fourse_calc_an(fourse_t * self, double *data, int size)
{
  int i;
  rvector_put(self->a, 0, fourse_0(data, size));
  for (i = 1; i < self -> n; i++) 
    rvector_put(self->a, i, fourse_cos(data, size, i));
}

/*
  Evaluates $b_n$ coeficientes of Fourier serie
*/
void fourse_calc_bn(fourse_t * self, double *data, int size)
{
  int i;
  rvector_put(self->b, 0, 0);
  for (i = 1; i < self->n; i++) 
    rvector_put(self->b, i, fourse_sin(data, size, i));
}

/*
  Write coeficients to a file
  This file can be easily plotted
*/
void fourse_write_dat(fourse_t * self, char *name)
{
  int i;
  FILE * fout;
  
  fout = fopen(name, "w");
  
  fprintf(fout, "0 0 0 \n");
  fprintf(fout, "0 %g 0 \n", fourse_a(self, 0));
  fprintf(fout, "0.2 %g 0 \n", fourse_a(self, 0));
  fprintf(fout, "0.2 0 0 \n");
  
  for (i=1; i<self->n; i++) {
    fprintf(fout, "%g %g %g\n", i - 0.2, 0.00, 0.00);
    fprintf(fout, "%g %g %g\n", i - 0.2, 0.00, rvector_get(self->b, i));
    fprintf(fout, "%g %g %g\n", i - 0.0, 0.00, rvector_get(self->b, i));
    fprintf(fout, "%g %g %g\n", i + 0.0, rvector_get(self->a, i), 0.00);
    fprintf(fout, "%g %g %g\n", i + 0.2, rvector_get(self->a, i), 0.00);
    fprintf(fout, "%g %g %g\n", i + 0.2, 0.00, 0.00);
  }
  
  fclose(fout);
}

/*
  evaluates a0 -> internal use
*/
double fourse_0(double *buff, int size)
{
  int i;
  double sum;
  
  sum = 0;
  for (i = 0; i < size; i++)
    sum += *(buff+i);
  return sum / size;
}

/*
  evaluates an -> internal use
*/
double fourse_cos(double * buff, int size, int n)
{
  int i;
  double w, sum;
  
  w = M_2PI / size;
  sum = 0;
  for (i = 0; i < size; i++)
    sum += *(buff + i) * cos(n * w * i);
  return 2 * sum / size;
}

/*
  evaluates bn -> internal use
*/
double fourse_sin(double * buff, int size, int n)
{
  int i;
  double w, sum;
  
  w = M_2PI / size;
  sum = 0;
  for (i = 0; i < size; i++)
    sum += *(buff + i) * sin(n * w * i);
  return 2 * sum / size;
}

/*
  frees structure dinamically allocated
*/
void fourse_free(fourse_t * self) {
  rvector_free(self->a);
  rvector_free(self->b);
  free(self);
}

/*
  Puts the series in a stream, according to the format:
  0 a0 b0 c0 Phi0
  1 a1 b1 c1 Phi1
  ...
  n an bn cn Phin
  
  Where cn = sqrt(an^2+bn^2) and Phin is the angle
*/
int fourse_puto(fourse_t * self, FILE * stream) {
  int i;
  for (i = 0; i < self -> n; i++)
    fprintf(stream, "%d %g %g %g %g\n", i, rvector_get(self->a, i), rvector_get(self->b, i)
	    , sqrt(sqr(rvector_get(self->a, i)) + sqr(rvector_get(self->b, i)))
	    , atan2(rvector_get(self->b, i), rvector_get(self->a, i))
	    );
  return self -> n;
}

/*
  Reads the coeficients of the series, in the same format of fourse_put
*/
int fourse_geto(fourse_t * self, FILE * stream) {
  char * s;
  int line = 0;
  while((s = newgetline(stream)) && strlen(s)) {
    int i;
    char * si, * sn;
    //    printf("line = %s\n", s);
    sn = s;
    i = 0;
    while((si = strsep(&sn, " \t\n\r")) && strlen(si)) {
      if (i == 0) {
	line = atoi(si); // printf("line = %d\n", line);
	if (line > self -> n)
	  self -> n = line;
      }
      else if (i == 1)
	rvector_put(self->a, line, atof(si));
      else if (i == 2)
	rvector_put(self->b, line, atof(si));
      else
	break;
      i++;
    }
  }
  return line;
}

/* void fourse_calcComplexCoefs(fourse_t * self) { */
/*   int i; */
/*   self -> c[0] =fourse_a(self,0); */
/*   self -> c_[0] = 0; */
/*   for (i = 1; i < self -> n; i++) { */
/*     self -> c[i] = 0.5 * (fourse_a(self, i) - I * fourse_b(self, i)); */
/*     self -> c_[i] = 0.5 * (fourse_a(self, i) + I * fourse_b(self, i)); */
/*   } */
/* } */

/* void fourse_putComplexCoefs(fourse_t * self, FILE * stream) { */
/*   int i; */
/*   for (i = 0; i < self -> n; i++) */
/*     fprintf(stream, "%02d (%g,%g) (%g,%g)\n", i */
/* 	    , creal(self -> c[i]), cimag(self -> c[i]) */
/* 	    , creal(self -> c_[i]), cimag(self -> c_[i])); */
/* } */

/* void fourse_calcRealCoefs(fourse_t * self) { */
/*   int i; */
/*   _Complex double x; */
/*fourse_a(self,0) = self -> c[0]; */
/*fourse_b(self,0) = 0; */
/*   for (i = 1; i < self -> n; i++) { */
/*     x = self -> c[i] + self -> c_[i]; */
/*     fourse_a(self, i) = creal(x); */
/*     x = I * (self -> c[i] - self -> c_[i]); */
/*     fourse_b(self, i) = creal(x); */
/*   } */
/* } */

/* void fourse_putRealCoefs(fourse_t * self, FILE * stream) { */
/*   int i; */
/*   for (i = 0; i < self -> n; i++) */
/*     fprintf(stream, "%02d %g %g\n", i */
/* 	    , fourse_a(self, i), fourse_b(self, i)); */
/* } */

