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
  Alocates dynamic memory for data of an rvector object.
  Initializes data of an rvector object with 0.
*/
rvector_t * rvector_new(int n) {
  int blocks;
  rvector_t * self = (rvector_t *) calloc(1, sizeof(rvector_t));
  
  if (!self)
    /* error occurred allocating data */
    return NULL;
  
  blocks = n / VVECTOR_BLKSIZE; 
  blocks++; // minimum of 1 block

  //  printf("blocks = %d\n", blocks);
  self -> n = n;
  self -> maxn = blocks * VVECTOR_BLKSIZE;
  self -> elm_size = sizeof(double);
  
  //  printf("elm_size = %d    maxn = %d    n = %d\n", elm_size, self->maxn, n);
  if (!(self -> data = (void *) calloc(self -> maxn, self -> elm_size))) {
    free(self);
    return NULL;
  }
  //  printf(" elm_size = %d\n", elm_size);
  return self;
}

/*
  Get an element from data. If i is greater than n, it will cycle and pick the i % n element
*/
double rvector_get(rvector_t * self, int i) {
  if (i >= self->n || i < 0)
    i %= self->n;
  if (i < 0)
    i += self->n;
  return ((double*)self->data)[i];
}

/*
  Put en element in data. If i is greater than size n, it will cycle to the beggining, the same if i is negative.
*/
void rvector_put(rvector_t * self, int i, double x) {
  if (i >= self->n || i < 0)
    i %= self->n;
  if (i < 0)
    i += self->n;
  ((double*)self->data)[i] = x;
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
  size_t len = 0;

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
  Creates an  rvector from a string  with data separeted by  spaces ot
  tabs. The returned value, a new rvector, must be freed.
*/
rvector_t * rvector_create_from_string(const char *str) {
  char * si, * sn;
  rvector_t * r = rvector_new(0);
  sn = (char*) str;
  while((si = strsep(&sn, " \t\n\r")) && strlen(si))
    rvector_append(r, atof(si));
  return r;
}

/* 
   Get a rvector from a single line  in a file, with data separated by
   spaces or  tabs, line ranging  from 0  to n-1.  The  returned value
   must  be freed.  This returns  NULL  if there  is no  vector to  be
   created.
*/
rvector_t *rvector_from_file_line(const char *filename, int line) {
  FILE *f = fopen(filename, "r");
  char *s = NULL;
  size_t n = 0;
  int i = 0;
  while(getline(&s, &n, f) > 0) {
    if (i == line)
      return rvector_create_from_string(s);
    i++;
  }
  free(s);
  return NULL;
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
    while((si = strsep(&sn, " \t\n\r"))) {
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
