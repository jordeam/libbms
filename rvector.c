#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "constant.h"
#include "sysconst.h"
#include "vvector.h"
#include "rvector.h"
#include "strbms.h"

/*
  Identical methods
*/

/*
  Callback methods
*/

/*
  Get an element from data.
*/
double rvector_get(rvector_t * self, int i) {
  double x;
  vvector_get(self, i, &x);
  return x;
}

/*
  Put en element in data
*/
int rvector_put(rvector_t * self, int i, double xin) {
  double x = xin;
  if (vvector_getDebug() & 4) {
    printf("rvector_put %d %g\n", i, x);
  }
  return (int) vvector_put(self, i, &x);
}

/*
  Append an element in data
*/
int rvector_append(rvector_t * self, double x) {
  return vvector_append(self, &x);
}

/*
  get rvector object data from file 'name' in the format:
  x[0]
  x[1]
  x[2]
  ...
  x[n-1]
*/
int rvector_load(rvector_t * self, const char *name)
{
  FILE *f;
  char *s, c[STRSIZE];
  int i;
  double x;

  if(!(f=fopen(name, "r"))) {
    fprintf(stderr, "Can't open %s\n", name);
    return 0;
  }

  self->n = 0; // resets size

  s=fgets(c, STRSIZE, f);
  for (i = 0; s != 0; i++) {
    sscanf(c,"%lf", &x);
    if (!rvector_append(self, x)) {
      fclose(f);
      return 0;
    }
    s=fgets(c, STRSIZE, f);
  }
  fclose(f);
  return 1;
}

/*
  get rvector object data from stream in the format:
  x[0]
  x[1]
  x[2]
  ...
  x[n-1]
  Return 1 if success, 0 in case of failure
*/
int rvector_fgets(rvector_t * self, FILE * stream)
{
  char * s = NULL;
  unsigned int len = 0;

  self -> n = 0; /* resets size */

  while (getline(& s, & len, stream) > 0) {
    if (strlen(s) > 0)
      if (!rvector_append(self, atof(s))) {
        /* error */
        return 0;
      }
  }
  /* success */
  return 1;
}

/*
  Write 'w1 data' in file 'name' in the format:

  x[0]
  x[1]
  x[2]
  ...
  x[n-1]
*/
int rvector_write(rvector_t * self, const char *name)
{
  int i;
  FILE *f;
  if (!self->n) return 0;

  if(!(f = fopen(name,"w")))
    return 0;
  for (i = 0; i < self->n; i++)
    fprintf(f, "%g\n", rvector_get(self, i));
  fclose(f);
  return 1;
}

/*
  Write vector contents in stream, in the format:

  x[0]
  x[1]
  x[2]
  ...
  x[n-1]

  Return 1 if success, otherwise 0
*/
int rvector_fputs(rvector_t * self, FILE * stream)
{
  int i;
  if (! self -> n) return 0;

  for (i = 0; i < self -> n; i++)
    fprintf(stream, "%g\n", rvector_get(self, i));

  return 1;
}

/*
  writes 'data' in a file 'name' in the following format:

  column:  0   1  ...  n
  line0 : x00 x01 ... x0n
  line1 : x10 x11 ... x1n
  ...
  linem : xm0 xm1 ... xmn
*/
int rvector_write_dag(rvector_t * self, const char *name)
{
  FILE *f;
  double dtheta;
  int i;
  if (!self->n)
    return 0;
  if(!(f = fopen(name, "w")))
    return 0;
  dtheta = M_2PI/self->n;
  for (i = 0; i < self->n; i++)
    fprintf(f, "%g %g \n", i*dtheta, rvector_get(self, i));
  fclose(f);
  return 1;
}

/*
  reads data from a file, considering his column
*/
int rvector_read_column(rvector_t * self, const char * filename, int column) {
  FILE * f;
  char * s, * si, * sn;
  double val;
  rvector_clear(self);
  f = fopen(filename, "r");
  if(!f)
    return 0;
  while((s = newgetline(f)) && strlen(s)) {
    int i;
    /* printf("line = %s\n", s); */
    sn = s;
    i = 0;
    val = 0;
    while(si = strsep(&sn, " \t\n\r")) {
      if (strlen(si)) {
        if (i == column) {
          val = atof(si);
          /* printf("[%s]\n", si); */
          break;
        }
        i++;
      }
    }
    rvector_append(self, val);
  }
  fclose(f);
  return 1;
}

/* returns the expoent of x (base 2) */
/* FIXME: it only works for little endian architectures */
static int double_exp(double x)
{ 
  double _x = x;
  return (*(((uint32_t *) &_x) + 1) >> 20) & 0x3ff;
}

double
rvector_sum (rvector_t *x)
{
  rvector_t * accs = rvector_new(0);
  double acc = 0;
  int i;
#ifdef DEBUG
  printf("DEBUG: rvector_sum\n");
#endif

  for (i = 0; i < x->n; i++) {
    int acc_exp = double_exp(acc);
    int x_exp = double_exp(RVECTOR(x, i));
    if (abs(acc_exp - x_exp) > 18) {
      rvector_append(accs, acc);
      acc = 0;
    }
    acc += RVECTOR(x, i);
  }

  double res;

  if (accs->n == 0)
    /* no accumulator was created */
    res = acc;
  else if (accs->n == x->n) {
    /* must make a dummy sum: higher accuracy error */
    double sum = 0;
    for (i = 0; i < accs->n; i++)
      sum += RVECTOR(accs, i);
    res = sum;
  }
  else {
    /* let's call recursive to make another sum using accs */
    res = acc + rvector_sum(accs);
  }

#ifdef DEBUG
  printf("DEBUG: end rvector_sum\n");
#endif

  return res;
}

double
rvector_sqrsum (rvector_t *x)
{
  rvector_t * accs = rvector_new(0);
  double acc = 0;
  int i;
#ifdef DEBUG
  printf("DEBUG: rvector_sqrsum\n");
#endif

  for (i = 0; i < x->n; i++) {
    int acc_exp = double_exp(acc);
    double val = RVECTOR(x, i) * RVECTOR(x, i);
    int val_exp = double_exp(val);
    if (abs(acc_exp - val_exp) > 18) {
#ifdef DEBUG
      printf("DEBUG: new acc at i = = %012d\r", i);
#endif
      rvector_append(accs, acc);
      acc = 0;
    }
    acc += val;
  }

  double res;

  if (accs->n == 0)
    /* no accumulator was created */
    res = acc;
  else if (accs->n == x->n) {
    /* must make a dummy sum: higher accuracy error */
    double sum = 0;
    for (i = 0; i < accs->n; i++)
      sum += RVECTOR(accs, i);
    res = sum;
  }
  else {
    /* let's call recursive to make another sum using accs */
    res = acc + rvector_sum(accs);
  }

#ifdef DEBUG
  printf("DEBUG: end rvector_sqrsum\n");
#endif

  return res;
}

double rvector_avg(rvector_t *x) {
  return rvector_sum(x)/x->n;
}

double rvector_rms(rvector_t *x) {
  return sqrt(rvector_sqrsum(x)/x->n);
}
