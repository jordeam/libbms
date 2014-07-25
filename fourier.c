#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constant.h"
#include "sysconst.h"
#include "func.h"
#include "fourier.h"
#include "strbms.h"

/*
  allocs space
*/
fourier * fourier_alloc(void) {
  return (fourier *) malloc(sizeof(fourier));
}

/*
  Initializes all coeficients = 0
*/
fourier * fourier_init(fourier * self, int nharm) {
  int i;
  self -> nharm = (nharm >= MAXHARM) ? MAXHARM - 1 : nharm;
  for (i = 0; i < MAXHARM; i++)
    self -> a[i] = self -> b[i] = self -> c[i] = self -> c_[i] = 0;
    
  return self;
}

/*
  Initializes all coeficients from data
*/
fourier * fourier_initWithWav(fourier * self, int nharm, double * data, int data_size) {
  fourier_init(self, nharm);
  fourier_calc_all(self, data, data_size);
  return self;
}

/* 
   makes a[i]=0 and b[i]=0 
*/
void fourier_zero(fourier * self)
{
  int i;
  for(i = 0; i < MAXHARM; i++)
    self -> a[i] = self -> b[i] = 0;
  self -> nharm = 0;
}

/*
  load coeficients from a file
*/
int fourier_load(fourier * self, const char * name)  {
  FILE  *f;
  char  *s, c[STRSIZE];
  double ai, bi, lixo;
  int   i;
  
  for(i = 0; i < MAXHARM; i++)
    self -> a[i] = self -> b[i]=0;
  
  if(!(f = fopen(name,"r")))
    return 0;
  i = self -> nharm = 0;
  
  s = fgets(c, STRSIZE, f);
  while(s != 0 && i < MAXHARM) {
    sscanf(c, "%lf %lf %lf", &lixo, &ai, &bi);
    s=fgets(c, STRSIZE, f);
    if (frac(lixo)==0) {
      i=(int) lixo;
      if (i<MAXHARM) {
	if (ai!=0) self->a[i]=ai;
	if (bi!=0) self->b[i]=bi;
	if (self->nharm<i) self->nharm=i;
      }
    }
  }
  fclose(f);
  return 1;
}

/*
  returns the norm of a waveform
*/
double fourier_get_norm(fourier * self) {
  int i; 
  double sum;
  sum = sqr(self -> a[0]);
  for (i = 1; i < self -> nharm; i++) 
    sum += sqr(self -> a[i]) + sqr(self -> b[i]);
  return sqrt(sum);
}

/*
  Returns the value of f(x), considering all coeficients
*/
double fourier_eval(fourier * self, double x)
{
  double sum;
  int   i;
  sum=self->a[0];
  for (i=0; i <= self -> nharm; i++)
    sum+=self->a[i]*cos(i*x)+self->b[i]*sin(i*x);
  return sum;
}
 
/*
  Returns the value of f(x), considering the serie of sines
*/
double fourier_eval_sin(fourier * self, double x)
{
  double sum;
  int i;
  sum = 0;
  for (i = 0; i < self -> nharm; i++)
    sum += self -> b[i] * sin(i * x);
  return sum;
}

/*
  Returns the value of f(x), considering the serie of cosines
*/
double fourier_eval_cos(fourier * self, double x)
{
  double sum;
  int   i;
  sum = self -> a[0];
  for (i = 0; i <= self -> nharm; i++)
    sum += self -> a[i] * cos(i * x);
  return sum;
}

/*
  Evaluates all coeficients of Fourie serie
  enter with a vector 'data' of size 'size'
*/
void fourier_calc_all(fourier * self, double *data, int size) {
  int i;
  self -> a[0] = fourier_0(data, size);
  self -> b[0] = 0;
  for (i = 1; i <= self -> nharm; i++) {
    self -> a[i] = fourier_cos(data, size, i);
    self -> b[i] = fourier_sin(data, size, i);
  }
}

/*
  Evaluates $a_n$ coeficientes of Fourier serie
*/
void fourier_calc_an(fourier * self, double *data, int size)
{
  int i;
  self->a[0]=fourier_0(data, size);
  for (i = 1; i < self -> nharm; i++) 
    self -> a[i]=fourier_cos(data, size, i);
  self -> nharm = MAXHARM - 1;
}

/*
  Evaluates $b_n$ coeficientes of Fourier serie
*/
void fourier_calc_bn(fourier * self, double *data, int size)
{
  int i;
  self->b[0]=0;
  for (i=1; i<MAXHARM; i++) 
    self->b[i]=fourier_sin(data, size, i);
  self->nharm=MAXHARM-1;
}

/*
  Write coeficients to a file
  This file can be easily plotted
*/
void fourier_write(fourier * self, char *name)
{
  int i;
  FILE * fout;
  
  fout = fopen(name, "w");
  
  fprintf(fout, "0 0 0 \n");
  fprintf(fout, "0 %g 0 \n", self -> a[0]);
  fprintf(fout, "0.2 %g 0 \n", self -> a[0]);
  fprintf(fout, "0.2 0 0 \n");
  
  for (i=1; i<self->nharm; i++) {
    fprintf(fout, "%g %g %g\n", i - 0.2, 0.00, 0.00);
    fprintf(fout, "%g %g %g\n", i - 0.2, 0.00, self -> b[i]);
    fprintf(fout, "%g %g %g\n", i - 0.0, 0.00, self -> b[i]);
    fprintf(fout, "%g %g %g\n", i + 0.0, self -> a[i], 0.00);
    fprintf(fout, "%g %g %g\n", i + 0.2, self -> a[i], 0.00);
    fprintf(fout, "%g %g %g\n", i + 0.2, 0.00, 0.00);
  }
  
  fclose(fout);
}

/*
  evaluates a0 -> internal use
*/
double fourier_0(double *buff, int size)
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
double fourier_cos(double * buff, int size, int n)
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
double fourier_sin(double * buff, int size, int n)
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
void fourier_free(fourier * self) {
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
int fourier_puto(fourier * self, FILE * stream) {
  int i;
  for (i = 0; i < self -> nharm; i++)
    fprintf(stream, "%d %g %g %g %g\n", i, self -> a[i], self -> b[i]
	    , sqrt(sqr(self -> a[i]) + sqr(self -> b[i]))
	    , atan2(self -> b[i], self -> a[i])
	    );
  return self -> nharm;
}

/*
  Reads the coeficients of the series, in the same format of fourier_put
*/
int fourier_geto(fourier * self, FILE * stream) {
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
	if (line > self -> nharm)
	  self -> nharm = line;
      }
      else if (i == 1)
	self -> a[line] = atof(si);
      else if (i == 2)
	self -> b[line] = atof(si);
      else
	break;
      i++;
    }
  }
  return line;
}

void fourier_calcComplexCoefs(fourier * self) {
  int i;
  self -> c[0] = self -> a[0];
  self -> c_[0] = 0;
  for (i = 1; i < self -> nharm; i++) {
    self -> c[i] = 0.5 * (self -> a[i] - I * self -> b[i]);
    self -> c_[i] = 0.5 * (self -> a[i] + I * self -> b[i]);
  }
}

void fourier_putComplexCoefs(fourier * self, FILE * stream) {
  int i;
  for (i = 0; i < self -> nharm; i++)
    fprintf(stream, "%02d (%g,%g) (%g,%g)\n", i
	    , creal(self -> c[i]), cimag(self -> c[i])
	    , creal(self -> c_[i]), cimag(self -> c_[i]));
}

void fourier_calcRealCoefs(fourier * self) {
  int i;
  _Complex double x;
  self -> a[0] = self -> c[0];
  self -> b[0] = 0;
  for (i = 1; i < self -> nharm; i++) {
    x = self -> c[i] + self -> c_[i];
    self -> a[i] = creal(x);
    x = I * (self -> c[i] - self -> c_[i]);
    self -> b[i] = creal(x);
  }
}

void fourier_putRealCoefs(fourier * self, FILE * stream) {
  int i;
  for (i = 0; i < self -> nharm; i++)
    fprintf(stream, "%02d %g %g\n", i
	    , self -> a[i], self -> b[i]);
}

